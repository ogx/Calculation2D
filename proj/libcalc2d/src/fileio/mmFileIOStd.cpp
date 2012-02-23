#include <fileio\mmFileIOStd.h>

#include <stack>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <mmStringUtilities.h>
#include <mmOperatingSystemCalls.h>
#include <mmInterfaceInitializers.h>
#include <mmStringUtilities.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmFileIO - funkcje globalne
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmInt mmFileIO::GetFilePosition(std::ifstream* p_psFile)
{
	return static_cast<mmInt>(p_psFile->tellg());
}

mmInt mmFileIO::GetFilePosition(std::wifstream* p_psFile)
{
	return static_cast<mmInt>(p_psFile->tellg());
}

mmInt mmFileIO::GetFilePosition(std::ofstream* p_psFile)
{
	return static_cast<mmInt>(p_psFile->tellp());
}

mmInt mmFileIO::GetFilePosition(std::wofstream* p_psFile)
{
	return static_cast<mmInt>(p_psFile->tellp());
}

void mmFileIO::SetFilePosition(std::ifstream* p_psFile,mmInt p_iFilePos)
{
	p_psFile->seekg(p_iFilePos,std::ios_base::beg);
}

void mmFileIO::SetFilePosition(std::wifstream* p_psFile,mmInt p_iFilePos)
{
	p_psFile->seekg(p_iFilePos,std::ios_base::beg);
}

void mmFileIO::SetFilePosition(std::ofstream* p_psFile,mmInt p_iFilePos)
{
	p_psFile->seekp(p_iFilePos,std::ios_base::beg);
}

void mmFileIO::SetFilePosition(std::wofstream* p_psFile,mmInt p_iFilePos)
{
	p_psFile->seekp(p_iFilePos,std::ios_base::beg);
}

mmInt mmFileIO::GetFileLenght(std::ifstream* p_psFile)
{
	// zapisujê aktualn¹ pozycjê
	mmInt v_iActFilePos = GetFilePosition(p_psFile);

	// idê na koniec pliku
	p_psFile->seekg(0,std::ios_base::end);
	// pobieram pozycjê koñca pliku
	mmInt v_iFileLenght = GetFilePosition(p_psFile);

	// wracam na po³o¿enie pocz¹tkowe
	p_psFile->seekg(v_iActFilePos,std::ios_base::beg);

	// zwaracam pozycjê koñca pliku
	return v_iFileLenght;
}

mmInt mmFileIO::GetFileLenght(std::wifstream* p_psFile)
{
	// zapisujê aktualn¹ pozycjê
	mmInt v_iActFilePos = GetFilePosition(p_psFile);

	// idê na koniec pliku
	p_psFile->seekg(0,std::ios_base::end);
	// pobieram pozycjê koñca pliku
	mmInt v_iFileLenght = GetFilePosition(p_psFile);

	// wracam na po³o¿enie pocz¹tkowe
	p_psFile->seekg(v_iActFilePos,std::ios_base::beg);

	// zwaracam pozycjê koñca pliku
	return v_iFileLenght;
}

mmInt mmFileIO::GetFileLenght(std::ofstream* p_psFile)
{
	// zapisujê aktualn¹ pozycjê
	mmInt v_iActFilePos = GetFilePosition(p_psFile);

	// idê na koniec pliku
	p_psFile->seekp(0,std::ios_base::end);
	// pobieram pozycjê koñca pliku
	mmInt v_iFileLenght = GetFilePosition(p_psFile);

	// wracam na po³o¿enie pocz¹tkowe
	p_psFile->seekp(v_iActFilePos,std::ios_base::beg);

	// zwaracam pozycjê koñca pliku
	return v_iFileLenght;
}

mmInt mmFileIO::GetFileLenght(std::wofstream* p_psFile)
{
	// zapisujê aktualn¹ pozycjê
	mmInt v_iActFilePos = GetFilePosition(p_psFile);

	// idê na koniec pliku
	p_psFile->seekp(0,std::ios_base::end);
	// pobieram pozycjê koñca pliku
	mmInt v_iFileLenght = GetFilePosition(p_psFile);

	// wracam na po³o¿enie pocz¹tkowe
	p_psFile->seekp(v_iActFilePos,std::ios_base::beg);

	// zwaracam pozycjê koñca pliku
	return v_iFileLenght;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmFileIO::mmFileUtilsSTD
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmFileIO::mmFileUtilsSTD::mmFileUtilsSTD(mmLog::mmLogReceiverI *p_psLogReceiver):
																				 mmLog::mmLogSender(L"mmFileIO::mmFileUtilsSTD",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

  SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmFileIO::mmFileUtilsSTD::~mmFileUtilsSTD()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

void mmFileIO::mmFileUtilsSTD::CreateDir(mmString p_sDirName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start CreateDir Name=")+
															p_sDirName);

	try
	{
		mmOperatingSystem::CreateDir(p_sDirName);
	}
	catch(mmError &v_sErr)
	{
		switch(v_sErr.GetErrorCode())
		{
			case mmeFileIOPermissionToFileDenied:
			{
				SendLogMessage(mmLog::critical,mmString(L"CreateDir PermissionDenied"));
			};
			break;
			case mmeFileIONoSuchFileOrDirectory:
			{
				SendLogMessage(mmLog::critical,mmString(L"CreateDir NoSuchFileOrDirectory"));
			};
			break;
			case mmeFileIOUnknownError:
			{
				SendLogMessage(mmLog::critical,mmString(L"CreateDir UnknownError"));
			};
			break;
		};

		throw v_sErr;
	};

	SendLogMessage(mmLog::debug,mmString(L"End CreateDir"));
}

void mmFileIO::mmFileUtilsSTD::CreateDirStructure(mmString p_sDirName)
{
	std::stack<mmString> v_sDirStack;
	mmString v_sActDir = p_sDirName;

	SendLogMessage(mmLog::debug,mmString(L"Start CreateDirStructure Name=")+
															p_sDirName);

	// tworzê stos katalogów do utworzenia
	while(!IsExistingDir(v_sActDir))
	{
		v_sDirStack.push(v_sActDir);

		v_sActDir.resize(v_sActDir.find_last_of(L"\\"));
	};

	// tworzê katalogi ze stosu
	while(!v_sDirStack.empty())
	{
		CreateDir(v_sDirStack.top());

		v_sDirStack.pop();
	};

	SendLogMessage(mmLog::debug,mmString(L"End CreateDirStructure"));
}

void mmFileIO::mmFileUtilsSTD::ClearDir(mmString p_sDirName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start ClearDir Name=")+
															p_sDirName);

	// pobieram informacjê o zawartoœci wybranego katalogu
	std::vector<mmFileUtilsI::sDirElement> v_sDirectoryElements = GetDirElements(p_sDirName,L"*.*");

	// usuwam wszystkie elementy wektora v_sDirectoryElements
	for(mmInt v_i=0;v_i<static_cast<mmInt>(v_sDirectoryElements.size());v_i++)
	{
		if(v_sDirectoryElements[v_i].bFile)
		{
			SendLogMessage(mmLog::debug,mmString(L"ClearDir RemovingFile=") +
																	v_sDirectoryElements[v_i].sName);

			RemoveFile(p_sDirName +
								 mmString(L"\\") +
								 v_sDirectoryElements[v_i].sName);
		}
		else
		{
			SendLogMessage(mmLog::debug,mmString(L"ClearDir RemovingDir=") +
																	v_sDirectoryElements[v_i].sName);

			RemoveDir(p_sDirName +
								mmString(L"\\") +
								v_sDirectoryElements[v_i].sName,
								true);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End ClearDir"));
}

void mmFileIO::mmFileUtilsSTD::RemoveDir(mmString p_sDirName,
																				 bool p_bWithElements)
{
	SendLogMessage(mmLog::debug,mmString(L"Start RemoveDir Name=")+
															p_sDirName);

	if(p_bWithElements)
	{
		ClearDir(p_sDirName);
	};

	try
	{
		mmOperatingSystem::RemoveDir(p_sDirName);
	}
	catch(mmError &v_sErr)
	{
		switch(v_sErr.GetErrorCode())
		{
			case mmeFileIOPermissionToFileDenied:
			{
				SendLogMessage(mmLog::critical,mmString(L"RemoveDir PermissionDenied"));
			};
			break;
			case mmeFileIONoSuchFileOrDirectory:
			{
				SendLogMessage(mmLog::critical,mmString(L"RemoveDir NoSuchFileOrDirectory"));
			};
			break;
			case mmeFileIOUnknownError:
			{
				SendLogMessage(mmLog::critical,mmString(L"RemoveDir UnknownError"));
			};
			break;
		};

		throw v_sErr;
	};

	SendLogMessage(mmLog::debug,mmString(L"End RemoveDir"));
}

bool mmFileIO::mmFileUtilsSTD::IsExistingDir(mmString p_sDirName)
{
	bool v_bExist;

	SendLogMessage(mmLog::debug,mmString(L"Start IsExistingDir Name=")+
															p_sDirName);

	try
	{
		v_bExist = mmOperatingSystem::IsExistingDir(p_sDirName);
	}
	catch(mmError &v_sErr)
	{
		switch(v_sErr.GetErrorCode())
		{
			case mmeFileIOUnknownError:
			{
				SendLogMessage(mmLog::critical,mmString(L"IsExistingDir UnknownError"));
			};
			break;
		};

		throw v_sErr;
	};

	SendLogMessage(mmLog::debug,mmString(L"End IsExistingDir Res=") +
															mmStringUtilities::BoolToString(v_bExist));

	return v_bExist;
}

mmString mmFileIO::mmFileUtilsSTD::GetTemporaryDir(void)
{
	SendLogMessage(mmLog::debug, mmString(L"Start GetTemporaryDir"));

	wchar_t * v_pcTemp = NULL;
	std::size_t v_iTempSize = 0;
	_wdupenv_s(&v_pcTemp, &v_iTempSize, L"TEMP");

	if(v_pcTemp == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetTemporaryDir No_TEMP_EnvironmentVariable"));

		throw mmError(mmeFileIONoTemporaryDirectory);
	};

	mmString v_sTempDir = v_pcTemp;

	SendLogMessage(mmLog::debug, mmString(L"End GetTemporaryDir Dir=") + v_sTempDir);

	free(v_pcTemp);

	return v_sTempDir;
}

mmString mmFileIO::mmFileUtilsSTD::GetApplicationDir(void)
{
	mmString v_sAppDir;

	SendLogMessage(mmLog::debug,mmString(L"Start GetApplicationDir"));

	v_sAppDir = mmOperatingSystem::GetApplicationDirectory();

	SendLogMessage(mmLog::debug,mmString(L"End GetApplicationDir"));

	return v_sAppDir;
}

std::vector<mmFileIO::mmFileUtilsI::sDirElement> mmFileIO::mmFileUtilsSTD::GetDirElements(mmString p_sDirName,
																																													mmString p_sDirWildcards)
{
	std::vector<mmFileIO::mmFileUtilsI::sDirElement> v_sDirElements;

	SendLogMessage(mmLog::debug,mmString(L"Start GetDirElements Name=")+
															p_sDirName);

	try
	{
		v_sDirElements = mmOperatingSystem::GetDirectoryElements(p_sDirName,p_sDirWildcards);
	}
	catch(mmError &v_sErr)
	{
		switch(v_sErr.GetErrorCode())
		{
			case mmeFileIONoSuchFileOrDirectory:
			{
				SendLogMessage(mmLog::critical,mmString(L"GetDirElements NoSuchFileOrDirectory"));
			};
			break;
			case mmeFileIOUnknownError:
			{
				SendLogMessage(mmLog::critical,mmString(L"GetDirElements UnknownError"));
			};
			break;
		};

		throw v_sErr;
	};

 SendLogMessage(mmLog::debug,mmString(L"End GetDirElements"));

	return v_sDirElements;
}

bool mmFileIO::mmFileUtilsSTD::IsEmptyDir(mmString p_sDirName)
{
	bool v_bIsEmpty = false;

	SendLogMessage(mmLog::debug,mmString(L"Start IsEmptyDir Name=")+
															p_sDirName);

	std::vector<mmFileIO::mmFileUtilsI::sDirElement> v_sDirElements = GetDirElements(p_sDirName,L"*.*");
	if(v_sDirElements.size() == 0)
	{
		v_bIsEmpty = true;
	};

	SendLogMessage(mmLog::debug,mmString(L"End IsEmptyDir Res=") +
															mmStringUtilities::BoolToString(v_bIsEmpty));

	return v_bIsEmpty;
}

void mmFileIO::mmFileUtilsSTD::RemoveFile(mmString p_sFileName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start RemoveFile Name=")+
															p_sFileName);

	int v_iRes = _wremove(p_sFileName.c_str());
	if(v_iRes != 0)
	{
		switch(errno)
		{
			case EACCES:
			{
				SendLogMessage(mmLog::critical,mmString(L"RemoveFile PermissionDenied"));

				throw mmError(mmeFileIOPermissionToFileDenied);
			};
			case ENOENT:
			{
				SendLogMessage(mmLog::critical,mmString(L"RemoveFile NoSuchFileOrDirectory"));

				throw mmError(mmeFileIONoSuchFileOrDirectory);
			};
			default:
			{
				SendLogMessage(mmLog::critical,mmString(L"RemoveFile UnknownError"));

				throw mmError(mmeFileIOUnknownError);
			};
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End RemoveFile"));
}

bool mmFileIO::mmFileUtilsSTD::IsExistingFile(mmString p_sFileName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start IsExistingFile Name=")+
															p_sFileName);

	bool v_bIsExisting = mmOperatingSystem::IsExistingFile(p_sFileName);

	SendLogMessage(mmLog::debug,mmString(L"End IsExistingFile Res=") +
															mmStringUtilities::BoolToString(v_bIsExisting));

	return v_bIsExisting;
}

mmString mmFileIO::mmFileUtilsSTD::GetPathToFile(mmString p_sFileName)
{
	mmString v_sPath;

	SendLogMessage(mmLog::debug,mmString(L"Start GetPathToFile Name=")+
															p_sFileName);

	v_sPath = p_sFileName.substr(0,p_sFileName.find_last_of(L"\\"));

	SendLogMessage(mmLog::debug,mmString(L"End GetPathToFile Path=")+
															v_sPath);

	return v_sPath;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmFileIO::mmTextFileReadSTD
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmFileIO::mmTextFileReadSTD::mmTextFileReadSTD(mmLog::mmLogReceiverI *p_psLogReceiver):
																							 mmLog::mmLogSender(L"mmFileIO::mmTextFileReadSTD",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	// inicjalizujê pustym plikiem i niewykorzystan¹ pamiêci¹
	m_psFile = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmFileIO::mmTextFileReadSTD::~mmTextFileReadSTD()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	this->Close();

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

void mmFileIO::mmTextFileReadSTD::Open(mmString p_sFileName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Open FileName=") +
															p_sFileName);

	// jeœli poprzednio by³ otwarty to zamykam
	this->Close();

	// ustalam stan obiektu
	m_sFileName = p_sFileName;

	// otwieram plik
	try
	{
		m_psFile = new std::wifstream(p_sFileName.c_str());
	}
	catch(std::bad_alloc)
	{
		SendLogMessage(mmLog::critical,mmString(L"Open ErrorAllocationMemory"));

		throw mmError(mmeBadAlloc);
	};

	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"Open BadOpen"));

		throw mmError(mmeFileIOCantOpenFile);
	};

	SendLogMessage(mmLog::debug,mmString(L"End Open"));
}

void mmFileIO::mmTextFileReadSTD::Close(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Close"));

	delete m_psFile;

	// inicjalizujê pustym plikiem
	m_psFile = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Close"));
}

bool mmFileIO::mmTextFileReadSTD::ReadChar(wchar_t* p_pcChar)
{
	SendLogMessage(mmLog::debug,mmString(L"Start ReadChar"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"ReadChar NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"ReadChar FileError"));

		throw mmError(mmeFileIOCantReadFromFile);
	}
	else
	{
		// czytam znak
		m_psFile->get(*p_pcChar);

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::debug,mmString(L"End ReadChar Failed"));

			return false;
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End ReadChar Success"));

	return true;
}

bool mmFileIO::mmTextFileReadSTD::ReadLine(mmString* p_psFileLine)
{
	SendLogMessage(mmLog::debug,mmString(L"Start ReadLine"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"ReadLine NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"ReadLine FileError"));

		throw mmError(mmeFileIOCantReadFromFile);
	}
	else
	{
		wchar_t v_tcTempBuf[2048];

		// czytam liniê
		m_psFile->getline(v_tcTempBuf,2048);
		*p_psFileLine = v_tcTempBuf;

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::debug,mmString(L"End ReadLine Failed"));

			return false;
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End ReadLine Success"));

	return true;
}

mmInt mmFileIO::mmTextFileReadSTD::ReadLines(std::vector<mmString>* p_psLines,
																						 mmInt p_iLinesCount)
{
	mmString v_sTempString;
	mmInt v_iLinesRead = 0;

	SendLogMessage(mmLog::debug,mmString(L"Start ReadLines No=") +
															mmStringUtilities::MMIntToString(p_iLinesCount));

	while(v_iLinesRead < p_iLinesCount)
	{
		if(this->ReadLine(&v_sTempString))
		{
			v_iLinesRead++;
			p_psLines->push_back(v_sTempString);
		}
		else
		{
			break;
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End ReadLines No=") +
															mmStringUtilities::MMIntToString(v_iLinesRead));

	return v_iLinesRead;
}

mmInt mmFileIO::mmTextFileReadSTD::GetLenght(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetLenght"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetLenght NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	mmInt v_iFileLenght = GetFileLenght(m_psFile);

	SendLogMessage(mmLog::debug,mmString(L"End GetLenght"));

	return v_iFileLenght;
}

mmInt mmFileIO::mmTextFileReadSTD::ReadWholeFile(wchar_t* p_pcFileContent)
{
	mmInt v_iReadCount;

	SendLogMessage(mmLog::debug,mmString(L"Start ReadWholeFile"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"ReadWholeFile NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"ReadWholeFile FileErrorBefore"));

		// b³¹d - plik w stanie niew³aœciwym
		throw mmError(mmeFileIOCantReadFromFile);
	}
	else
	{
		// przechodzê na pocz¹tek pliku
		m_psFile->seekg(0);

		// czytam plik do bufora
		//v_iReadCount = m_psFile->readsome(p_pcFileContent,GetFileLenght(m_psFile));
		v_iReadCount = GetFileLenght(m_psFile);
		m_psFile->read(p_pcFileContent,v_iReadCount);

		// sprawdzam stan pliku po odczycie
		if( ( (!(*m_psFile)) && (v_iReadCount == 0) ) || (m_psFile->bad()) )
		{
			SendLogMessage(mmLog::critical,mmString(L"ReadWholeFile FileError"));

			throw mmError(mmeFileIOCantReadFromFile);
		}
		else
		{
			// usuwam failbit ze stanu strumienia
			if(m_psFile->fail())
			{
				m_psFile->setstate(m_psFile->rdstate()-std::ios_base::failbit);
			};
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End ReadWholeFile CharsReaded=") +
															mmStringUtilities::MMIntToString(v_iReadCount));

	return v_iReadCount;
}

bool mmFileIO::mmTextFileReadSTD::Eof(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Eof"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"Eof NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	bool v_bIsEof = m_psFile->eof();

	SendLogMessage(mmLog::debug,mmString(L"End Eof Res=")+
															mmStringUtilities::BoolToString(v_bIsEof));

	return v_bIsEof;
}

bool mmFileIO::mmTextFileReadSTD::IsActiveFile(void)
{
	bool v_bIsActive = true;

	SendLogMessage(mmLog::debug,mmString(L"IsActiveFile"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		v_bIsActive = false;
	};

	SendLogMessage(mmLog::debug,mmString(L"End IsActiveFile Res=")+
															mmStringUtilities::BoolToString(v_bIsActive));

	return v_bIsActive;
}

mmInt mmFileIO::mmTextFileReadSTD::GetActualFilePosition(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetActualFilePosition"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetActualFilePosition NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	mmInt v_iActFilePos = GetFilePosition(m_psFile);

	SendLogMessage(mmLog::debug,mmString(L"End GetActualFilePosition"));

	return v_iActFilePos;
}

mmString mmFileIO::mmTextFileReadSTD::GetFileName(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetFileName"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetFileName NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetFileName"));

	return m_sFileName;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmFileIO::mmTextFileWriteSTD
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmFileIO::mmTextFileWriteSTD::mmTextFileWriteSTD(mmLog::mmLogReceiverI *p_psLogReceiver):
																								 mmLog::mmLogSender(L"mmFileIO::mmTextFileWriteSTD",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	m_psFile = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmFileIO::mmTextFileWriteSTD::~mmTextFileWriteSTD()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	this->Close();

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

void mmFileIO::mmTextFileWriteSTD::Open(mmString p_sFileName,
																				eOpenFileMode p_eOpenMode)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Open FileName=") +
															p_sFileName);

	// jeœli poprzednio by³ otwarty to zamykam
	this->Close();

	// ustalam stan obiektu
	m_sFileName = p_sFileName;

	std::string v_sFileName = mmStringUtilities::MMStringToCharString(m_sFileName);

	// otwieram plik
	try
	{
		// jeœli zapisywanie od koñca pliku
		if(p_eOpenMode == append_for_write)
		{
			m_psFile = new std::wofstream(v_sFileName.c_str(),std::ios_base::app);

			SendLogMessage(mmLog::debug,mmString(L"Open MoveToEof"));
		}
		else
		{
			m_psFile = new std::wofstream(v_sFileName.c_str());
		};
	}
	catch(std::bad_alloc)
	{
		SendLogMessage(mmLog::critical,mmString(L"Open ErrorAllocationMemory"));

		throw mmError(mmeBadAlloc);
	};

	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"Open BadOpen"));

		throw mmError(mmeFileIOCantOpenFile);
	};

	SendLogMessage(mmLog::debug,mmString(L"End Open"));
}

void mmFileIO::mmTextFileWriteSTD::Close(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Close"));

	try
	{
		if(m_psFile != NULL)
		{
			Flush();

			m_psFile->close();
		};
	}
	catch(mmError &v_sErr)
	{
		// zamykam plik
		delete m_psFile;

		throw v_sErr;
	};

	// zamykam plik
	delete m_psFile;

	// inicjalizujê stan zmiennych
	m_psFile = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Close"));
}

void mmFileIO::mmTextFileWriteSTD::WriteChar(wchar_t p_cChar)
{
	SendLogMessage(mmLog::debug,mmString(L"Start WriteChar"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteChar NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteChar FileError"));

		throw mmError(mmeFileIOCantWriteToFile);
	}
	else
	{
		// zapisujê znak
		m_psFile->write(&p_cChar,1);

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::critical,mmString(L"WriteChar Failed"));

			throw mmError(mmeFileIOCantWriteToFile);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End WriteChar"));
}

void mmFileIO::mmTextFileWriteSTD::WriteLine(mmString p_sFileLine)
{
	SendLogMessage(mmLog::debug,mmString(L"Start WriteLine"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteLine NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteLine FileError"));

		// b³¹d - plik w stanie niew³aœciwym
		throw mmError(mmeFileIOCantWriteToFile);
	}
	else
	{
		// zapisujê liniê
		(*m_psFile) << p_sFileLine;
		(*m_psFile) << std::endl;

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::critical,mmString(L"WriteLine Failed"));

			throw mmError(mmeFileIOCantWriteToFile);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End WriteLine"));
}

void mmFileIO::mmTextFileWriteSTD::WriteLines(std::vector<mmString>* p_psLines)
{
	SendLogMessage(mmLog::debug,mmString(L"Start WriteLines"));

	// sprawdzam czy wskaŸnik pusty
	if(p_psLines == NULL)
	{
		SendLogMessage(mmLog::debug,mmString(L"WriteLines NoLinesToSave"));

		return;
	};

	mmInt v_iLinesToWrite = static_cast<mmInt>(p_psLines->size());
	mmInt v_i;

	for(v_i=0;v_i<v_iLinesToWrite;v_i++)
	{
		this->WriteLine((*p_psLines)[v_i]);
	};

	SendLogMessage(mmLog::debug,mmString(L"End WriteLinesSuccess No=") +
															mmStringUtilities::MMIntToString(v_iLinesToWrite));
}

void mmFileIO::mmTextFileWriteSTD::WriteBuffer(wchar_t const* p_pcBuffer,mmInt p_iBufferSize)
{
	SendLogMessage(mmLog::debug,mmString(L"Start WriteBuffer"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteBuffer NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteBuffer FileErrorBefore"));

		throw mmError(mmeFileIOCantWriteToFile);
	}
	else
	{
		// zapisujê bufor do pliku
		m_psFile->write(p_pcBuffer,p_iBufferSize);

		// sprawdzam stan pliku po zapisie
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::critical,mmString(L"WriteBuffer FileError"));

			throw mmError(mmeFileIOCantWriteToFile);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End WriteBuffer"));
}

void mmFileIO::mmTextFileWriteSTD::WriteEol(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start WriteEol"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteEol NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteEol FileError"));

		throw mmError(mmeFileIOCantWriteToFile);
	}
	else
	{
		// zapisujê koniec linii
		(*m_psFile) << std::endl;

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::critical,mmString(L"WriteEol Failed"));

			throw mmError(mmeFileIOCantWriteToFile);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End WriteEol"));
}

void mmFileIO::mmTextFileWriteSTD::Flush(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Flush"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"Flush NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"Flush FileError"));

		throw mmError(mmeFileIOCantWriteToFile);
	}
	else
	{
		// zapisujê na dysk bufory
		m_psFile->flush();

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::critical,mmString(L"Flush Failed"));

			throw mmError(mmeFileIOCantWriteToFile);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End Flush"));
}

bool mmFileIO::mmTextFileWriteSTD::IsActiveFile(void)
{
	bool v_bIsActive = true;

	SendLogMessage(mmLog::debug,mmString(L"Start IsActiveFile"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		v_bIsActive = false;
	};

	SendLogMessage(mmLog::debug,mmString(L"End IsActiveFile Res=")+
															mmStringUtilities::BoolToString(v_bIsActive));

	return v_bIsActive;
}

mmString mmFileIO::mmTextFileWriteSTD::GetFileName(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetFileName"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetFileName NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetFileName"));

	return m_sFileName;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmFileIO::mmBinaryFileReadSTD
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmFileIO::mmBinaryFileReadSTD::mmBinaryFileReadSTD(mmLog::mmLogReceiverI *p_psLogReceiver):
																									 mmLog::mmLogSender(L"mmFileIO::mmBinaryFileReadSTD",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	// inicjalizujê pustym plikiem i niewykorzystan¹ pamiêci¹
	m_psFile = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmFileIO::mmBinaryFileReadSTD::~mmBinaryFileReadSTD()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	this->Close();

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

void mmFileIO::mmBinaryFileReadSTD::Open(mmString p_sFileName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Open FileName=") +
															p_sFileName);

	// jeœli poprzednio by³ otwarty to zamykam
	this->Close();

	// ustalam stan obiektu
	m_sFileName = p_sFileName;

	// otwieram plik
	try
	{
		m_psFile = new std::ifstream(mmStringUtilities::MMStringToCharString(p_sFileName).c_str(),std::ios_base::binary);
	}
	catch(std::bad_alloc)
	{
		SendLogMessage(mmLog::critical,mmString(L"Open ErrorAllocationMemory"));

		throw mmError(mmeBadAlloc);
	};

	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"Open BadOpen"));

		throw mmError(mmeFileIOCantOpenFile);
	};

	SendLogMessage(mmLog::debug,mmString(L"End Open"));
}

void mmFileIO::mmBinaryFileReadSTD::Close(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Close"));

	delete m_psFile;

	// inicjalizujê pustym plikiem
	m_psFile = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Close"));
}

bool mmFileIO::mmBinaryFileReadSTD::ReadBuffer(void* p_pBuffer,mmInt p_iSize)
{
	SendLogMessage(mmLog::debug,mmString(L"Start ReadBuffer"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"ReadBuffer NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"ReadBuffer FileError"));

		throw mmError(mmeFileIOCantReadFromFile);
	}
	else
	{
		// czytam bufor
		m_psFile->read(reinterpret_cast<char*>(p_pBuffer),p_iSize);

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::debug,mmString(L"End ReadBuffer Failed"));

			return false;
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End ReadBuffer Success"));

	return true;
}

mmInt mmFileIO::mmBinaryFileReadSTD::GetLenght(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetLenght"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetLenght NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	mmInt v_iFileLenght = GetFileLenght(m_psFile);

	SendLogMessage(mmLog::debug,mmString(L"End GetLenght"));

	return v_iFileLenght;
}

bool mmFileIO::mmBinaryFileReadSTD::Eof(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Eof"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"Eof NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	bool v_bIsEof = m_psFile->eof();

	SendLogMessage(mmLog::debug,mmString(L"End Eof Res=")+
															mmStringUtilities::BoolToString(v_bIsEof));

	return v_bIsEof;
}

bool mmFileIO::mmBinaryFileReadSTD::IsActiveFile(void)
{
	bool v_bIsActive = true;

	SendLogMessage(mmLog::debug,mmString(L"IsActiveFile"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		v_bIsActive = false;
	};

	SendLogMessage(mmLog::debug,mmString(L"End IsActiveFile Res=")+
															mmStringUtilities::BoolToString(v_bIsActive));

	return v_bIsActive;
}

mmInt mmFileIO::mmBinaryFileReadSTD::GetActualFilePosition(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetActualFilePosition"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetActualFilePosition NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	mmInt v_iActFilePos = GetFilePosition(m_psFile);

	SendLogMessage(mmLog::debug,mmString(L"End GetActualFilePosition"));

	return v_iActFilePos;
}

void mmFileIO::mmBinaryFileReadSTD::SetActualFilePosition(mmInt p_iFilePos)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetActualFilePosition"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"SetActualFilePosition NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	SetFilePosition(m_psFile,p_iFilePos);

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"SetActualFilePosition NoSuchPosition"));

		throw mmError(mmeFileIOBadFilePosition);
	};

	SendLogMessage(mmLog::debug,mmString(L"End SetActualFilePosition"));
}

mmString mmFileIO::mmBinaryFileReadSTD::GetFileName(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetFileName"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetFileName NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetFileName"));

	return m_sFileName;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmFileIO::mmBinaryFileWriteSTD
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmFileIO::mmBinaryFileWriteSTD::mmBinaryFileWriteSTD(mmLog::mmLogReceiverI *p_psLogReceiver):
																										mmLog::mmLogSender(L"mmFileIO::mmBinaryFileWriteSTD",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	m_psFile = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmFileIO::mmBinaryFileWriteSTD::~mmBinaryFileWriteSTD()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	this->Close();

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

void mmFileIO::mmBinaryFileWriteSTD::Open(mmString p_sFileName,
																				 mmFileIO::eOpenFileMode p_eOpenMode)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Open FileName=") +
															p_sFileName);

	// jeœli poprzednio by³ otwarty to zamykam
	this->Close();

	// ustalam stan obiektu
	m_sFileName = p_sFileName;

	// otwieram plik
	try
	{
		// jeœli zapisywanie od koñca pliku
		if(p_eOpenMode == append_for_write)
		{
			m_psFile = new std::ofstream(p_sFileName.c_str(),std::ios_base::app|std::ios_base::binary);

			SendLogMessage(mmLog::debug,mmString(L"Open MoveToEof"));
		}
		else
		{
			m_psFile = new std::ofstream(p_sFileName.c_str(),std::ios_base::trunc|std::ios_base::binary);
		};
	}
	catch(std::bad_alloc)
	{
		SendLogMessage(mmLog::critical,mmString(L"Open ErrorAllocationMemory"));

		throw mmError(mmeBadAlloc);
	};

	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"Open BadOpen"));

		throw mmError(mmeFileIOCantOpenFile);
	};

	SendLogMessage(mmLog::debug,mmString(L"End Open"));
}

void mmFileIO::mmBinaryFileWriteSTD::Close(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Close"));

	try
	{
		if(m_psFile != NULL)
		{
			Flush();
		};
	}
	catch(mmError &v_sErr)
	{
		// zamykam plik
		delete m_psFile;

		throw v_sErr;
	};

	// zamykam plik
	delete m_psFile;

	// inicjalizujê stan zmiennych
	m_psFile = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End Close"));
}

bool mmFileIO::mmBinaryFileWriteSTD::WriteBuffer(void* p_pBuffer,mmInt p_iSize)
{
	SendLogMessage(mmLog::debug,mmString(L"Start WriteBuffer"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteBuffer NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"WriteBuffer FileError"));

		throw mmError(mmeFileIOCantReadFromFile);
	}
	else
	{
		// zapisujê bufor
		m_psFile->write(reinterpret_cast<const char*>(p_pBuffer),p_iSize);

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::debug,mmString(L"End WriteBuffer Failed"));

			return false;
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End WriteBuffer Success"));

	return true;
}

mmInt mmFileIO::mmBinaryFileWriteSTD::GetLenght(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetLenght"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetLenght NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	mmInt v_iFileLenght = GetFileLenght(m_psFile);

	SendLogMessage(mmLog::debug,mmString(L"End GetLenght"));

	return v_iFileLenght;
}

bool mmFileIO::mmBinaryFileWriteSTD::IsActiveFile(void)
{
	bool v_bIsActive = true;

	SendLogMessage(mmLog::debug,mmString(L"IsActiveFile"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		v_bIsActive = false;
	};

	SendLogMessage(mmLog::debug,mmString(L"End IsActiveFile Res=")+
															mmStringUtilities::BoolToString(v_bIsActive));

	return v_bIsActive;
}

mmInt mmFileIO::mmBinaryFileWriteSTD::GetActualFilePosition(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetActualFilePosition"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetActualFilePosition NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	mmInt v_iActFilePos = GetFilePosition(m_psFile);

	SendLogMessage(mmLog::debug,mmString(L"End GetActualFilePosition"));

	return v_iActFilePos;
}

void mmFileIO::mmBinaryFileWriteSTD::SetActualFilePosition(mmInt p_iFilePos)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetActualFilePosition"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"SetActualFilePosition NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	SetFilePosition(m_psFile,p_iFilePos);

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"SetActualFilePosition NoSuchPosition"));

		throw mmError(mmeFileIOBadFilePosition);
	};

	SendLogMessage(mmLog::debug,mmString(L"End SetActualFilePosition"));
}

mmString mmFileIO::mmBinaryFileWriteSTD::GetFileName(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetFileName"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetFileName NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetFileName"));

	return m_sFileName;
}

void mmFileIO::mmBinaryFileWriteSTD::Flush(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Flush"));

	// sprawdzam czy plik jest zainicjowany
	if(m_psFile == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"Flush NoFileOpened"));

		throw mmError(mmeFileIOFileIsNotOpened);
	};

	// sprawdzam stan pliku
	if(!(*m_psFile))
	{
		SendLogMessage(mmLog::critical,mmString(L"Flush FileError"));

		throw mmError(mmeFileIOCantWriteToFile);
	}
	else
	{
		// zapisujê na dysk bufory
		m_psFile->flush();

		// sprawdzam stan pliku
		if(!(*m_psFile))
		{
			SendLogMessage(mmLog::critical,mmString(L"Flush Failed"));

			throw mmError(mmeFileIOCantWriteToFile);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End Flush"));
}


