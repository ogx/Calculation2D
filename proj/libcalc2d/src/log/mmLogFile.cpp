#include <log\mmLogFile.h>

#include <interfaces\mmInterfaceInitializers.h>

mmLog::mmLogFile::mmLogFile(mmString const & p_sLogFileName)
{
	m_sLogFile.open(p_sLogFileName);
}

mmLog::mmLogFile::~mmLogFile()
{
	m_sLogFile.flush();
}

void mmLog::mmLogFile::SendLogMessage(eLogMessagePriority const p_ePriority, mmString const & p_sString)
{
	static wchar_t const v_pcPriority[] = L"DIUC";

	m_sLogFile << v_pcPriority[p_ePriority] << L" " << p_sString << std::endl;
	m_sLogFile.flush();
}
