#include <synchronize\mmSynchronizeWindowsImpl.h>

#include <mmStringUtilities.h>
#include <interfaces\mmInterfaceInitializers.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmSynchronize::mmExclusiveLockCSWindows
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmSynchronize::mmExclusiveLockCSWindows::mmExclusiveLockCSWindows(mmLog::mmLogReceiverI *p_psLogReceiver):
																					 mmLog::mmLogSender(L"mmSynchronize::mmExclusiveLockCSWindows",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	::InitializeCriticalSectionAndSpinCount(&m_sCriticalSection, 300);

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmSynchronize::mmExclusiveLockCSWindows::~mmExclusiveLockCSWindows()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	if(m_sCriticalSection.LockCount > -1)
	{
		SendLogMessage(mmLog::critical,mmString(L"Destructor Locked"));

		throw mmError(mmeExclusiveLockLocked);
	};

	::DeleteCriticalSection(&m_sCriticalSection);

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

bool mmSynchronize::mmExclusiveLockCSWindows::TryLock(void)
{
	bool v_bIsLocked = true;

	SendLogMessage(mmLog::debug,mmString(L"Start TryLock"));

	if(::TryEnterCriticalSection(&m_sCriticalSection) == 0)
	{
		v_bIsLocked = false;
	};

	SendLogMessage(mmLog::debug,mmString(L"End TryLock Locked=") + mmStringUtilities::BoolToString(v_bIsLocked));

	return v_bIsLocked;
}

void mmSynchronize::mmExclusiveLockCSWindows::Lock(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Lock"));

	::EnterCriticalSection(&m_sCriticalSection);

	SendLogMessage(mmLog::debug,mmString(L"End Lock"));
}

void mmSynchronize::mmExclusiveLockCSWindows::Unlock(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Unlock"));

	::LeaveCriticalSection(&m_sCriticalSection);

	SendLogMessage(mmLog::debug,mmString(L"End Unlock"));
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmSynchronize::mmReadWriteLockCSWindows
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmSynchronize::mmReadWriteLockCSWindows::mmReadWriteLockCSWindows(mmLog::mmLogReceiverI *p_psLogReceiver):
																																	mmLog::mmLogSender(L"mmSynchronize::mmReadWriteLockCSWindows",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	try
	{
		m_psExclusiveLock = mmInterfaceInitializers::CreateExclusiveLock(p_psLogReceiver);
	}
	catch(mmError &v_sErr)
	{
		if(v_sErr.GetErrorCode() == mmeBadAlloc)
		{
			SendLogMessage(mmLog::critical,mmString(L"Constructor ErrorAllocationMemory"));
		};

		throw;
	};

	m_iReadWriteCount = 0;
	m_bWaitForWriteLock = false;

	SendLogMessage(mmLog::debug,mmString(L"End Constructor PtrToExclusiveLock=") + mmStringUtilities::PointerToString(m_psExclusiveLock));
}

mmSynchronize::mmReadWriteLockCSWindows::~mmReadWriteLockCSWindows()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	delete m_psExclusiveLock;

	if(m_iReadWriteCount != 0)
	{
		SendLogMessage(mmLog::critical,mmString(L"Destructor BadEndConditions RWCount=") +
																	 mmStringUtilities::MMIntToString(m_iReadWriteCount));

		throw mmError(mmeReadWriteLockBadEndLockCount);
	};
	if(m_bWaitForWriteLock)
	{
		SendLogMessage(mmLog::critical,mmString(L"Destructor BadEndConditions WaitForWrite=") +
																	 mmStringUtilities::BoolToString(m_bWaitForWriteLock));

		throw mmError(mmeReadWriteLockWaitForWrite);
	};

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

bool mmSynchronize::mmReadWriteLockCSWindows::TryLockForRead(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start TryLockForRead"));

	bool v_bIsLocked = false;

	m_psExclusiveLock->Lock();
		if( (m_iReadWriteCount >= 0) && (!m_bWaitForWriteLock) )
		{
			m_iReadWriteCount++;
			v_bIsLocked = true;
		};
	m_psExclusiveLock->Unlock();

	SendLogMessage(mmLog::debug,mmString(L"End TryLockForRead LockCount=") +
															mmStringUtilities::MMIntToString(m_iReadWriteCount) +
															mmString(L" Locked=") +
															mmStringUtilities::BoolToString(v_bIsLocked));

	return v_bIsLocked;
}

void mmSynchronize::mmReadWriteLockCSWindows::LockForRead(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start LockForRead"));

	bool v_bIsLocked = false;

	while(!v_bIsLocked)
	{
		if(TryLockForRead())
		{
			v_bIsLocked = true;
		}
		else
		{
			Sleep(1);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End LockForRead LockCount=") +
															mmStringUtilities::MMIntToString(m_iReadWriteCount));
}

void mmSynchronize::mmReadWriteLockCSWindows::UnlockFromRead(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UnlockFromRead"));

	bool v_bIsReadWriteLockDeSynchronized = false;

	m_psExclusiveLock->Lock();
		if(m_iReadWriteCount > 0)
		{
			m_iReadWriteCount--;
		}
		else
		{
			v_bIsReadWriteLockDeSynchronized = true;
		};
	m_psExclusiveLock->Unlock();

	if(v_bIsReadWriteLockDeSynchronized)
	{
		SendLogMessage(mmLog::critical,mmString(L"UnlockFromRead TooManyUnlocksFromRead"));

		throw mmError(mmeReadWriteLockReadUnlocked);
	};

	SendLogMessage(mmLog::debug,mmString(L"End UnlockFromRead LockCount=") +
															mmStringUtilities::MMIntToString(m_iReadWriteCount));
}

bool mmSynchronize::mmReadWriteLockCSWindows::TryLockForWrite(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start TryLockForWrite"));

	bool v_bIsLocked = false;

	m_psExclusiveLock->Lock();
		if(m_iReadWriteCount == 0)
		{
			m_iReadWriteCount = -1;
			v_bIsLocked = true;
		};
	m_psExclusiveLock->Unlock();

	SendLogMessage(mmLog::debug,mmString(L"End TryLockForWrite LockCount=") +
															mmStringUtilities::MMIntToString(m_iReadWriteCount) +
															mmString(L" Locked=") +
															mmStringUtilities::BoolToString(v_bIsLocked));

	return v_bIsLocked;
}

void mmSynchronize::mmReadWriteLockCSWindows::LockForWrite(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start LockForWrite"));

	bool v_bIsLocked = false;

	while(!v_bIsLocked)
	{
		m_psExclusiveLock->Lock();
			if(m_iReadWriteCount == 0)
			{
				m_iReadWriteCount = -1;
				v_bIsLocked = true;
			}
			else
			{
				if( (m_iReadWriteCount > 0) && (!m_bWaitForWriteLock) )
				{
					m_bWaitForWriteLock = true;
				};
			};
		m_psExclusiveLock->Unlock();

		if(!v_bIsLocked)
		{
			Sleep(1);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End LockForWrite LockCount=") +
															mmStringUtilities::MMIntToString(m_iReadWriteCount));
}

void mmSynchronize::mmReadWriteLockCSWindows::UnlockFromWrite(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UnlockFromWrite"));

	bool v_bIsReadWriteLockDeSynchronized = false;

	m_psExclusiveLock->Lock();
		if(m_iReadWriteCount == -1)
		{
			m_iReadWriteCount = 0;
			m_bWaitForWriteLock = false;
		}
		else
		{
			v_bIsReadWriteLockDeSynchronized = true;
		};
	m_psExclusiveLock->Unlock();

	if(v_bIsReadWriteLockDeSynchronized)
	{
		SendLogMessage(mmLog::critical,mmString(L"UnlockFromWrite TooManyUnlocksFromWrite"));

		throw mmError(mmeReadWriteLockWriteUnlocked);
	};

	SendLogMessage(mmLog::debug,mmString(L"End UnlockFromWrite LockCount=") +
															mmStringUtilities::MMIntToString(m_iReadWriteCount));
}

mmInt mmSynchronize::mmReadWriteLockCSWindows::GetLockState(bool* p_pbWaitForWrite)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetLockState"));

	mmInt v_iLockState = m_iReadWriteCount;
	*p_pbWaitForWrite = m_bWaitForWriteLock;

	this->SendLogMessage(mmLog::debug,mmString(L"End GetLockState"));

	return v_iLockState;
}
