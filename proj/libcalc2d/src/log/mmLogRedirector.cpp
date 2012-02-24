#include <log\mmLogRedirector.h>

#include <interfaces\mmInterfaceInitializers.h>

mmLog::mmLogRedirector::mmLogRedirector()
{
	m_psExclusiveLock = mmInterfaceInitializers::CreateExclusiveLock();
}

mmLog::mmLogRedirector::~mmLogRedirector()
{
	delete m_psExclusiveLock;
}

bool mmLog::mmLogRedirector::IsRegistered(mmLogReceiverI* p_pLogReceiver)
{
	mmInt v_iCount = static_cast<mmInt>(m_sReceiverTable.size());
	mmInt v_i;
	bool bIsFound = false;

	for(v_i=0;v_i<v_iCount;v_i++)
	{
		if(m_sReceiverTable[v_i].pReceiver == p_pLogReceiver)
		{
			bIsFound = true;
		};
	};

	return bIsFound;
}

mmInt mmLog::mmLogRedirector::GetFreeReceiverTableIndex(void)
{
	mmInt v_iCount = static_cast<mmInt>(m_sReceiverTable.size());
	mmInt v_i;

	// sprawdzam czy nie ma pustego elementu
	for(v_i=0;v_i<v_iCount;v_i++)
	{
		if(m_sReceiverTable[v_i].pReceiver == NULL)
		{
			return v_i;
		};
	};

	// inicjalizujê nowy i zwracam jego indeks
	sLogReceiver v_sLR;
	v_sLR.pReceiver = NULL;

	m_sReceiverTable.push_back(v_sLR);

	return static_cast<mmInt>(m_sReceiverTable.size()-1);
}

void mmLog::mmLogRedirector::RegisterLogReceiver(eLogMessagePriority p_eMinPriority,
																								 mmLogReceiverI* p_pLogReceiver)
{
	bool bIsRegistered;

	// jeœli jest ju¿ zarejestrowana to zg³aszam wyj¹tek
	m_psExclusiveLock->Lock();
		bIsRegistered = IsRegistered(p_pLogReceiver);
	m_psExclusiveLock->Unlock();
	if(bIsRegistered)
	{
		throw mmError(mmeLogReceiverObjectAlreadyRegistered);
	};

	// tworzê nowy element
  m_psExclusiveLock->Lock();
		mmInt v_iFreeIndex = GetFreeReceiverTableIndex();
		m_sReceiverTable[v_iFreeIndex].ePriority = p_eMinPriority;
    m_sReceiverTable[v_iFreeIndex].pReceiver = p_pLogReceiver;
  m_psExclusiveLock->Unlock();
}

void mmLog::mmLogRedirector::UnregisterLogReceiver(mmLogReceiverI* p_pLogReceiver)
{
	mmInt v_iUnregisterCount = 0;

	m_psExclusiveLock->Lock();
		mmInt v_iCount = static_cast<mmInt>(m_sReceiverTable.size());
		mmInt v_i;

		for(v_i=0;v_i<v_iCount;v_i++)
		{
			if(m_sReceiverTable[v_i].pReceiver == p_pLogReceiver)
			{
				m_sReceiverTable[v_i].pReceiver = NULL;
				v_iUnregisterCount++;
			};
		};
	m_psExclusiveLock->Unlock();

	if(v_iUnregisterCount != 1)
	{
		throw mmError(mmeLogReceiverObjectIsNotRegistered);
	};
}

void mmLog::mmLogRedirector::SendLogMessage(eLogMessagePriority p_ePriority,mmString* p_psString)
{
	m_psExclusiveLock->Lock();
		mmInt v_iCount = static_cast<mmInt>(m_sReceiverTable.size());
		mmInt v_i;

		for(v_i=0;v_i<v_iCount;v_i++)
		{
			if(m_sReceiverTable[v_i].pReceiver != NULL)
			{
				if(m_sReceiverTable[v_i].ePriority <= p_ePriority)
				{
					m_sReceiverTable[v_i].pReceiver->SendLogMessage(p_ePriority,p_psString);
				};
			};
		};
	m_psExclusiveLock->Unlock();
}

