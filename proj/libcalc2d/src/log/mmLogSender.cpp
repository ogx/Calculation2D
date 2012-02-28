#include <log\mmLogSender.h>

#include <mmStringUtilities.h>
#include <mmOperatingSystemCalls.h>

mmLog::mmLogSender::mmLogSender(mmString const & p_sClassName, mmLog::mmLogReceiverI * const p_psLog, void * const p_pClassPointer) :
	m_sClassName(p_sClassName), 
	m_psLog(p_psLog), 
	m_pClassPointer(p_pClassPointer)
{}

void mmLog::mmLogSender::SendLogMessage(eLogMessagePriority const p_ePriority, mmString const & p_sString)
{
	if(m_psLog != NULL)
	{
		mmString const v_sLogMessage = mmStringUtilities::GetCurrentDateTimeString() +
														 mmString(L" ") +
														 mmStringUtilities::MMRealToString(mmOperatingSystem::GetApplicationTime(),3) +
														 mmString(L" ") +
														 mmStringUtilities::MMIntToString(mmOperatingSystem::GetCurrentThreadID()) +
														 mmString(L" ") +
														 m_sClassName +
														 mmString(L" ") +
														 mmStringUtilities::PointerToString(m_pClassPointer) +
														 mmString(L" ") +
														 p_sString;

		m_psLog->SendLogMessage(p_ePriority, v_sLogMessage);
	};
}

mmLog::mmLogReceiverI* mmLog::mmLogSender::GetLogReceiver(void)
{
	return m_psLog;
}

void mmLog::mmLogSender::SendLogMessage(void * const p_pClassPointer, eLogMessagePriority const p_ePriority, mmString const & p_sString)
{
	if(m_psLog != NULL)
	{
		mmString const v_sLogMessage = mmStringUtilities::GetCurrentDateTimeString() +
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

		m_psLog->SendLogMessage(p_ePriority, v_sLogMessage);
	};
}
