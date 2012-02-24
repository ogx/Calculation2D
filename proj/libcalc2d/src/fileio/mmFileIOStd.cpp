#include <fileio\mmFileIOStd.h>

#include <stack>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <mmStringUtilities.h>
#include <mmOperatingSystemCalls.h>
#include <interfaces\mmInterfaceInitializers.h>
#include <mmStringUtilities.h>

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