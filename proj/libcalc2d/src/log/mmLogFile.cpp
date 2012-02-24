#include <log\mmLogFile.h>

#include <mmInterfaceInitializers.h>

mmLog::mmLogFile::mmLogFile(mmString p_sLogFileName)
{
	m_sLogFile.open(p_sLogFileName);
}

mmLog::mmLogFile::~mmLogFile()
{
	m_sLogFile.flush();
}

void mmLog::mmLogFile::SendLogMessage(eLogMessagePriority p_ePriority, mmString* p_psString)
{
	static wchar_t const v_pcPriority[] = L"DIUC";

	m_sLogFile << v_pcPriority[p_ePriority] << L" " << *p_psString << std::endl;
	m_sLogFile.flush();
}
