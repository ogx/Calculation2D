#include <log\mmLogFile.h>

#include <mmInterfaceInitializers.h>

mmLog::mmLogFile::mmLogFile(mmString p_sLogFileName)
{
	m_psLogFile = mmInterfaceInitializers::CreateTextFileForWrite();

	m_psLogFile->Open(p_sLogFileName,mmFileIO::trunc_for_write);
}

mmLog::mmLogFile::~mmLogFile()
{
	m_psLogFile->Flush();

	delete m_psLogFile;
}

void mmLog::mmLogFile::SendLogMessage(eLogMessagePriority p_ePriority,mmString* p_psString)
{
	switch(p_ePriority)
	{
		case mmLog::debug:
		{
			m_psLogFile->WriteChar('D');
		};
		break;
		case mmLog::info:
		{
			m_psLogFile->WriteChar('I');
		};
		break;
		case mmLog::user:
		{
			m_psLogFile->WriteChar('U');
		};
		break;
		case mmLog::critical:
		{
			m_psLogFile->WriteChar('C');
		};
		break;
	};

	m_psLogFile->WriteChar(' ');

	m_psLogFile->WriteLine(*p_psString);
}
