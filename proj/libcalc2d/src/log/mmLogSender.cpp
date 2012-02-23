#include <log\mmLogSender.h>

#include <mmStringUtilities.h>
#include <mmOperatingSystemCalls.h>

mmLog::mmLogSender::mmLogSender(mmString p_sClassName,mmLog::mmLogReceiverI* p_psLog)
{
	m_sClassName = p_sClassName;
	m_psLog = p_psLog;
}

void mmLog::mmLogSender::SendLogMessage(eLogMessagePriority p_ePriority,mmString p_sString)
{
	if(m_psLog != NULL)
	{
		mmString v_sLogMessage = mmStringUtilities::GetCurrentDateTimeString() +
														 mmString(L" ") +
														 mmStringUtilities::MMRealToString(mmOperatingSystem::GetApplicationTime(),3) +
														 mmString(L" ") +
														 mmStringUtilities::MMIntToString(mmOperatingSystem::GetCurrentThreadID()) +
														 mmString(L" ") +
														 m_sClassName +
														 mmString(L" ") +
														 mmStringUtilities::PointerToString(this) +
														 mmString(L" ") +
														 p_sString;

		m_psLog->SendLogMessage(p_ePriority,&v_sLogMessage);
	};
}

mmLog::mmLogReceiverI* mmLog::mmLogSender::GetLogReceiver(void)
{
	return m_psLog;
}

void mmLog::mmLogSender::SendLogMessage(void* p_pClassPointer,eLogMessagePriority p_ePriority,mmString p_sString)
{
	if(m_psLog != NULL)
	{
		mmString v_sLogMessage = mmStringUtilities::GetCurrentDateTimeString() +
														 mmString(L" ") +
														 mmStringUtilities::MMRealToString(mmOperatingSystem::GetApplicationTime(),3) +
														 mmString(L" ") +
														 mmStringUtilities::MMIntToString(mmOperatingSystem::GetCurrentThreadID()) +
														 mmString(L" ") +
														 m_sClassName +
														 mmString(L" ") +
														 mmStringUtilities::PointerToString(p_pClassPointer) +
														 mmString(L" ") +
														 p_sString;

		m_psLog->SendLogMessage(p_ePriority,&v_sLogMessage);
	};
}
