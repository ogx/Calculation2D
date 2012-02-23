//******************************************************************************
//******************************************************************************
//
//  Operating system calls
//
//  Description: All operating system calls for the system.
//
//******************************************************************************
//******************************************************************************
#ifndef mmOperatingSystemCallsH
#define mmOperatingSystemCallsH
#include <Windows.h>
#include <time.h>

#include <vector>

#include <mmGlobalDefs.h>
#include <mmError.h>
#include <interfaces\mmIFileIO.h>

#define OS_NAME "WindowsNT"

////////////////////////////////////////////////////////////////////////////////
/// Namespace with all operating system calls. Hides system dependency for
/// the system.
////////////////////////////////////////////////////////////////////////////////
namespace mmOperatingSystem
{
	////////////////////////////////////////////////////////////////////////////////
	/// Returns current thread identifier.
	///
	/// @return thread ID
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetCurrentThreadID(void);

	////////////////////////////////////////////////////////////////////////////////
	/// Function creates directory in specified location. In case of error it throws
	/// mmeFileIONoSuchFileOrDirectory, mmeFileIOPermissionToFileDenied
	/// or mmeUnknownError.
	///
	/// @param[in] p_sDirName name with full path for new directory
	////////////////////////////////////////////////////////////////////////////////
	void CreateDir(mmString p_sDirName);
	////////////////////////////////////////////////////////////////////////////////
	/// Function deletes directory from specified location. In case of error it throws
	/// mmeFileIONoSuchFileOrDirectory, mmeFileIOPermissionToFileDenied
	/// or mmeUnknownError.
	///
	/// @param[in] p_sDirName name with full path for deleted directory
	////////////////////////////////////////////////////////////////////////////////
	void RemoveDir(mmString p_sDirName);
	////////////////////////////////////////////////////////////////////////////////
	/// Function checks if specified directory exist.
	///
	/// @param[in] p_sDirName path to checked directory
	/// @return TRUE if directory exists, FALSE otherwise.
	////////////////////////////////////////////////////////////////////////////////
	bool IsExistingDir(mmString p_sDirName);
	////////////////////////////////////////////////////////////////////////////////
	/// Function returns directory to application executable file.
	///
	/// @return full path to application executable.
	////////////////////////////////////////////////////////////////////////////////
	mmString GetApplicationDirectory(void);
	////////////////////////////////////////////////////////////////////////////////
	/// Function returns application executable file name.
	///
	/// @return name only of application executable.
	////////////////////////////////////////////////////////////////////////////////
	mmString GetApplicationName(void);
	////////////////////////////////////////////////////////////////////////////////
	/// Function returns full path to current working directory. In case of error
	/// it throws mmeUnknownError.
	///
	/// @return path to current directory
	////////////////////////////////////////////////////////////////////////////////
	mmString GetCurrentWorkingDir(void);

	////////////////////////////////////////////////////////////////////////////////
	/// Method returns vector with elemens located in specified directory.
	/// In case of error it throws mmeFileIONoSuchFileOrDirectory or mmeUnknownError.
	///
	/// @param[in] p_sDirName path to directory
	/// @param[in] p_sDirWildcards string with wildcards definition
	/// @return vector with directory content.
	////////////////////////////////////////////////////////////////////////////////
	std::vector<mmFileIO::mmFileUtilsI::sDirElement> GetDirectoryElements(mmString p_sDirName,
																																				mmString p_sDirWildcards);

	////////////////////////////////////////////////////////////////////////////////
	/// Function checks if requested file exist.
	///
	/// @param[in] p_sFileName name with full path to file
	/// @return TRUE if file exists, FALSE otherwise.
	////////////////////////////////////////////////////////////////////////////////
	bool IsExistingFile(mmString p_sFileName);

	////////////////////////////////////////////////////////////////////////////////
	/// Function returns time in seconds from application start. Returned time
	/// can be used for measuring calculation times.
	///
	/// @return time in seconds from application start
	////////////////////////////////////////////////////////////////////////////////
	mmReal GetApplicationTime(void);

	////////////////////////////////////////////////////////////////////////////////
	/// This function stops thread execution for p_iMiliSeconds.
	///
	/// @param[in] p_iMiliSeconds miliseconds
	////////////////////////////////////////////////////////////////////////////////
	void StopThread(mmInt p_iMiliSeconds);

	////////////////////////////////////////////////////////////////////////////////
	/// Method returns string with unique ID.
	///
	/// @return ID string
	////////////////////////////////////////////////////////////////////////////////
	mmString GetUniqueIDString(void);
};

#endif
