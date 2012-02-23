
#pragma hdrstop

#include <mmOperatingSystemCalls.h>
#include <mmStringUtilities.h>

#include <direct.h>
#include <io.h>
#include <stdlib.h>
#include <dos.h>
#include <errno.h>
#include <mmError.h>
#include <stdio.h>
#include <ctime>
#include <Windows.h>
#include <strsafe.h>

mmInt mmOperatingSystem::GetCurrentThreadID(void)
{
	return GetCurrentThreadId();
}

void mmOperatingSystem::CreateDir(mmString p_sDirName)
{
	int v_iRes = _wmkdir(p_sDirName.c_str());
	if(v_iRes != 0)
	{
		switch(errno)
		{
			case EACCES:
			{
				throw mmError(mmeFileIOPermissionToFileDenied);
			};
			case ENOENT:
			{
				throw mmError(mmeFileIONoSuchFileOrDirectory);
			};
			default:
			{
				throw mmError(mmeUnknownError);
			};
		};
	};
}

void mmOperatingSystem::RemoveDir(mmString p_sDirName)
{
	int v_iRes = _wrmdir(p_sDirName.c_str());
	if(v_iRes != 0)
	{
		switch(errno)
		{
			case EACCES:
			{
				throw mmError(mmeFileIOPermissionToFileDenied);
			};
			case ENOENT:
			{
				throw mmError(mmeFileIONoSuchFileOrDirectory);
			};
			default:
			{
				throw mmError(mmeUnknownError);
			};
		};
	};
}

bool mmOperatingSystem::IsExistingDir(mmString p_sDirName)
{
	bool v_bRes = false;

	// pobieram aktywny dysk i œcie¿kê
	int v_iDriveNo = _getdrive();
	wchar_t* v_pcCurDir = _wgetcwd(NULL,_MAX_PATH);
	if(v_pcCurDir == NULL)
	{
		throw mmError(mmeBadAlloc);
	};

	// prubujê przejœæ do œcie¿ki p_sDirName
	if(_wchdir(p_sDirName.c_str()) == 0)
	{
		v_bRes = true;
	};

	// przywracam poprzedni dysk i œcie¿kê
	_chdrive(v_iDriveNo);
	if(_wchdir(v_pcCurDir) != 0)
	{
		throw mmError(mmeUnknownError);
	};

	// zwalniam pamiêæ przydzielon¹ przez getcwd
	free(v_pcCurDir);

	return v_bRes;
}

mmString mmOperatingSystem::GetApplicationDirectory(void)
{


    TCHAR v_ptcAppPath[MAX_PATH + 1];

	// Clear destination buffer
    ::ZeroMemory( v_ptcAppPath, sizeof(v_ptcAppPath) );
    
    // Get fully-qualified path for the executable
    if ( ! ::GetModuleFileName(NULL, v_ptcAppPath, MAX_PATH ) )
    {
        // Failure
        return (L"");
    }
	mmString v_sAppPath( v_ptcAppPath );

	v_sAppPath.resize(v_sAppPath.find_last_of(L"\\"));

	return v_sAppPath;
}

mmString mmOperatingSystem::GetApplicationName(void)
{

	
	MAX_PATH;
    TCHAR v_ptcAppPath[MAX_PATH + 1];
   

	// Clear destination buffer
    ::ZeroMemory( v_ptcAppPath, sizeof(v_ptcAppPath) );
    
    // Get fully-qualified path for the executable
    if ( ! ::GetModuleFileName(NULL, v_ptcAppPath, MAX_PATH ) )
    {
        // Failure
        return (L"");
    }

	mmString v_sAppNameAndPath( v_ptcAppPath ); 

	mmString v_sAppName = v_sAppNameAndPath.substr(v_sAppNameAndPath.find_last_of(L"\\")+1,v_sAppNameAndPath.size()-v_sAppNameAndPath.find_last_of(L"\\")-1);

	return v_sAppName;

}

mmString mmOperatingSystem::GetCurrentWorkingDir(void)
{
	wchar_t v_ctDirectory[_MAX_PATH];

	wchar_t v_cLetter[2];
	v_cLetter[0] = 'A'+_getdrive()-1;
	v_cLetter[1] = 0;
	if(_wgetcwd(v_ctDirectory,_MAX_PATH) == NULL)
	{
		throw mmError(mmeUnknownError);
	};

	mmString v_sWorkingDir = mmString(v_cLetter) +
													 mmString(L":\\") +
													 mmString(v_ctDirectory);

	return v_sWorkingDir;
}

std::vector<mmFileIO::mmFileUtilsI::sDirElement> mmOperatingSystem::GetDirectoryElements(mmString p_sDirName,
																																												 mmString p_sDirWildcards)
{
	std::vector<mmFileIO::mmFileUtilsI::sDirElement> v_sDirElements;
	_wfinddata_t v_sActFileInfo;
	mmFileIO::mmFileUtilsI::sDirElement v_sActElement;
	intptr_t v_hSearchHandle;
	int v_iRes = 0;

	// pobieram pierwszy element
	mmString v_sString((p_sDirName + mmString(L"\\") + p_sDirWildcards).c_str());
	v_hSearchHandle = _wfindfirst(const_cast<wchar_t*>(v_sString.c_str()), &v_sActFileInfo);
	if(v_hSearchHandle == -1)
	{
		//if( (_doserrno == ENOENT) || (_doserrno == EINVAL) )
		{
			throw mmError(mmeFileIONoSuchFileOrDirectory);
		}
		//else
		//{
		//	throw mmError(mmeUnknownError);
		//};
	}
	else
	{
		while(v_iRes != -1)
		{
			// przetwarzam element
			v_sActElement.sName = v_sActFileInfo.name;
			// nie przetwarzam elementów "." i ".."
			if( (v_sActElement.sName.compare(L".") != 0) && (v_sActElement.sName.compare(L"..") != 0) )
			{
				if(v_sActFileInfo.attrib & _A_SUBDIR)
				{
					v_sActElement.bFile = false;
				}
				else
				{
					v_sActElement.bFile = true;
				};
				v_sDirElements.push_back(v_sActElement);
			};
			// pobieram nastêpny
			v_iRes = _wfindnext(v_hSearchHandle,&v_sActFileInfo);
		};
	};

	_findclose(v_hSearchHandle);

	return v_sDirElements;
}

bool mmOperatingSystem::IsExistingFile(mmString p_sFileName)
{
	return ::_waccess(p_sFileName.c_str(), 0) != -1;
}

mmReal mmOperatingSystem::GetApplicationTime(void)
{
	std::clock_t v_sCurrentClock;

	v_sCurrentClock = clock();
	return (static_cast<mmReal>(v_sCurrentClock)/CLK_TCK);
}


void mmOperatingSystem::StopThread(mmInt p_iMiliSeconds)
{
	Sleep(p_iMiliSeconds);
}

mmString mmOperatingSystem::GetUniqueIDString(void)
{
	mmString v_sOutIDString;

  // Hard Drive ID
	DWORD v_sSN,v_sDW1,v_sDW2;
	wchar_t v_cS1[256];
	wchar_t v_cS2[256];

	wchar_t * v_pcWinDir = NULL;
	std::size_t v_iWinDirSize = 0;
	_wdupenv_s(&v_pcWinDir, &v_iWinDirSize, L"windir");

	mmString v_sDrive = mmString(v_pcWinDir).substr(0,3);
	GetVolumeInformation(v_sDrive.c_str(), v_cS1, 256, &v_sSN, &v_sDW1, &v_sDW2, v_cS2, 256);

	free(v_pcWinDir);

	wchar_t v_cHDD[256];
	swprintf_s(v_cHDD,L"%d",v_sSN);
	mmString v_sHDD = v_cHDD;

	// Program tick count
	std::clock_t v_sCurrentClock = v_sCurrentClock = clock();

	wchar_t v_cCLK[256];
	swprintf_s(v_cCLK,L"%d",v_sCurrentClock);
	mmString v_sCLK = v_cCLK;

	// date time
	mmString v_sDateTime = mmStringUtilities::GetCurrentDateTimeString();

	v_sOutIDString = v_sHDD + v_sCLK + mmString(L"_") + v_sDateTime;

	return v_sOutIDString;
}

