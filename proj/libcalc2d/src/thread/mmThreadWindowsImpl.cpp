#include <thread\mmThreadWindowsImpl.h>

#include <vector>

#include <interfaces\mmInterfaceInitializers.h>
#include <mmStringUtilities.h>
#include <mmOperatingSystemCalls.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmThreads::mmThreadWindows
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmThread::mmThreadWindows::mmThreadWindows(mmLog::mmLogReceiverI *p_psLogReceiver):
																					 mmLog::mmLogSender(L"mmThread::mmThreadWindows",p_psLogReceiver)
{
	mmInt v_iTemp;

	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	try
	{
		//m_psExclusiveLock = mmInterfaceInitializers::CreateExclusiveLock(p_psLogReceiver);
		m_psExclusiveLock = mmInterfaceInitializers::CreateExclusiveLock(NULL);
	}
	catch(mmError &v_sErr)
	{
		if(v_sErr.GetErrorCode() == mmeBadAlloc)
    {
			SendLogMessage(mmLog::critical,mmString(L"Constructor ErrorAllocationMemory"));
		};

		throw v_sErr;
	};

	m_hWinThreadHandle = CreateThread(NULL,0,ThreadRedirectFunction,this,CREATE_SUSPENDED,reinterpret_cast<unsigned long*>(&v_iTemp));
	//SetThreadPriority(m_hWinThreadHandle,THREAD_PRIORITY_HIGHEST);

	// jeœli utworzenie nowego w¹tka siê nie powiedzie
	if(m_hWinThreadHandle == NULL)
	{
		delete m_psExclusiveLock;

		SendLogMessage(mmLog::critical,mmString(L"Constructor CreationThreadError"));

		throw mmError(mmeThreadCreateThreadError);
  };

	m_eThreadStatus = created;
	SendLogMessage(mmLog::debug,mmString(L"Constructor Status=created"));

	m_psThreadExecutionClass = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Constructor PtrToExclusiveLock=") +
															mmStringUtilities::PointerToString(m_psExclusiveLock) +
															mmString(L"; ThreadID=") +
                              mmStringUtilities::MMIntToString(v_iTemp));
}

mmThread::mmThreadWindows::~mmThreadWindows()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	// zatrzymujê w¹tek
	this->Stop();

	if(WaitForSingleObject(m_hWinThreadHandle,INFINITE) != WAIT_OBJECT_0)
	{
		SendLogMessage(mmLog::critical,mmString(L"Destructor BadStateOfThread"));
	};

	CloseHandle(m_hWinThreadHandle);

	delete m_psExclusiveLock;

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

DWORD WINAPI mmThread::mmThreadWindows::ThreadRedirectFunction(LPVOID p_pThreadClassPointer)
{
	mmThread::mmThreadWindows* v_pThreadWindows = reinterpret_cast<mmThread::mmThreadWindows*>(p_pThreadClassPointer);

	v_pThreadWindows->ThreadExecute();

	return 0;
}

void mmThread::mmThreadWindows::ThreadExecute(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start ThreadExecute"));


	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	bool v_bIsCalculated = m_psThreadExecutionClass->Execute();
	CoUninitialize();

	m_psExclusiveLock->Lock();
		if(v_bIsCalculated)
		{
			m_eThreadStatus = finished;
			SendLogMessage(mmLog::debug,mmString(L"ThreadExecute Status=finished"));
		}
		else
		{
			m_eThreadStatus = stopped;
			SendLogMessage(mmLog::debug,mmString(L"ThreadExecute Status=stopped"));
		};

		// to wywo³anie jest w sekcji krytycznej dlatego, ¿e czêsto po tej metodzie
		// wywo³uje siê destruktora i wtedy to wywo³anie jest ju¿ w nie istniej¹cym
    // obiekcie
		SendLogMessage(mmLog::debug,mmString(L"End ThreadExecute"));

	m_psExclusiveLock->Unlock();
}

void mmThread::mmThreadWindows::RegisterThreadExecutionClass(mmThreadExecutionI* p_psThreadExecutionClass)
{
	SendLogMessage(mmLog::debug,mmString(L"Start RegisterThreadExecutionClass"));

	m_psExclusiveLock->Lock();
	try
	{
		if( (m_eThreadStatus == created) || (m_eThreadStatus == initialized) )
		{
			m_psThreadExecutionClass = p_psThreadExecutionClass;
			if(m_eThreadStatus != initialized)
			{
				m_eThreadStatus = initialized;
				SendLogMessage(mmLog::debug,mmString(L"RegisterThreadExecutionClass Status=initialized"));
			};

			SendLogMessage(mmLog::debug,mmString(L"RegisterThreadExecutionClass Registered PtrToExecutionObject=") +
																	mmStringUtilities::PointerToString(m_psThreadExecutionClass));
		}
		else
		{
			SendLogMessage(mmLog::critical,mmString(L"RegisterThreadExecutionClass CantRegister"));

			throw mmError(mmeThreadCantRegisterExecutionClass);
		};
	}
	catch(mmError &v_sErr)
  {
		m_psExclusiveLock->Unlock();

		throw v_sErr;
  };

	m_psExclusiveLock->Unlock();

	SendLogMessage(mmLog::debug,mmString(L"End RegisterThreadExecutionClass"));
}

void mmThread::mmThreadWindows::Run(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Run"));

	m_psExclusiveLock->Lock();
	try
	{
		// tylko kiedy w¹tek jest w stanie initialized mo¿e byæ wywo³ywana metoda Run
		if(this->m_eThreadStatus == initialized)
		{
			if(ResumeThread(m_hWinThreadHandle) == 1)
			{
				m_eThreadStatus = running;
				SendLogMessage(mmLog::debug,mmString(L"Run Status=running"));
			}
			else
			{
				SendLogMessage(mmLog::critical,mmString(L"Run ResumeThreadError"));

				throw mmError(mmeThreadResumeError);
			};
		}
		else
    {
			SendLogMessage(mmLog::critical,mmString(L"Run BadThreadStateError"));

			throw mmError(mmeThreadBadThreadState);
    };
  }
	catch(mmError &v_sErr)
	{
		m_psExclusiveLock->Unlock();

		throw v_sErr;
	};

	m_psExclusiveLock->Unlock();

  SendLogMessage(mmLog::debug,mmString(L"End Run"));
}

void mmThread::mmThreadWindows::Stop(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Stop"));

	bool bIsCreatedInitializedStoppedOrFinished = false;

	while(!bIsCreatedInitializedStoppedOrFinished)
	{
		m_psExclusiveLock->Lock();
			// w przypadku: kiedy jest uruchomiony ale nie wiadomo czy ju¿ liczy
			if(m_eThreadStatus == running)
			{
				if(m_psThreadExecutionClass->IsExecuting())
				{
					m_eThreadStatus = stopping;
					SendLogMessage(mmLog::debug,mmString(L"Stop Status=stopping"));

					// informujê o zatrzymaniu w¹tka klasê licz¹c¹
					m_psThreadExecutionClass->StopExecution();
				};
			}
			else
			{
				// w innych
				if(m_eThreadStatus != stopping)
				{
					bIsCreatedInitializedStoppedOrFinished = true;
				};
			};
		m_psExclusiveLock->Unlock();

		if(!bIsCreatedInitializedStoppedOrFinished)
		{
			Sleep(1);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End Stop"));
}

mmThread::mmThreadI::eStatus mmThread::mmThreadWindows::GetStatus(void)
{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetStatus"));

	eStatus v_eTempStat;

	m_psExclusiveLock->Lock();
		v_eTempStat = m_eThreadStatus;
	m_psExclusiveLock->Unlock();

	mmString v_sStatusString;

	switch(v_eTempStat)
	{
		case created:
		{
			v_sStatusString = L"created";
		};
		break;
		case initialized:
		{
			v_sStatusString = L"initialized";
		};
		break;
		case running:
		{
			v_sStatusString = L"running";
		};
		break;
		case stopping:
		{
			v_sStatusString = L"stopping";
		};
		break;
		case stopped:
		{
			v_sStatusString = L"stopped";
		};
		break;
		case finished:
		{
			v_sStatusString = L"finished";
		};
		break;
		default:
		{
			SendLogMessage(mmLog::critical,mmString(L"GetStatus UnknownThreadState"));

			throw mmError(mmeThreadUnknownThreadState);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetStatus Status=")+
															v_sStatusString);

	return v_eTempStat;
}

mmReal mmThread::mmThreadWindows::GetProgress(void)
{
	mmReal v_rProgress = -1.0;

	SendLogMessage(mmLog::debug,mmString(L"Start GetProgress"));

	m_psExclusiveLock->Lock();
	if(m_eThreadStatus == running)
	{
		v_rProgress = m_psThreadExecutionClass->GetProgress();
	};
	m_psExclusiveLock->Unlock();

	SendLogMessage(mmLog::debug,mmString(L"End GetProgress"));

	return v_rProgress;
}
