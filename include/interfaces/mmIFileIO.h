//******************************************************************************
//******************************************************************************
//
//  FileIO Interface
//
//
//  Description: This interface unifies file access/read/write capabilities.
//
//******************************************************************************
//******************************************************************************
#ifndef mmIFileIOH
#define mmIFileIOH

#include <vector>

#include <mmGlobalDefs.h>
#include <mmError.h>

////////////////////////////////////////////////////////////////////////////////
/// This namespace unifies Input/Output operations to hard drive resources.
////////////////////////////////////////////////////////////////////////////////
namespace mmFileIO
{
	////////////////////////////////////////////////////////////////////////////////
	/// This enumeration defines open access to file.
	////////////////////////////////////////////////////////////////////////////////
	typedef enum
	{
		////////////////////////////////////////////////////////////////////////////////
		/// Read access.
		////////////////////////////////////////////////////////////////////////////////
		for_read          = 0,
		////////////////////////////////////////////////////////////////////////////////
		/// Write access.
		////////////////////////////////////////////////////////////////////////////////
		trunc_for_write   = 1,
		////////////////////////////////////////////////////////////////////////////////
		/// Open for write and move to end of file.
		////////////////////////////////////////////////////////////////////////////////
		append_for_write  = 2
	} eOpenFileMode;

	////////////////////////////////////////////////////////////////////////////////
	/// Interface for global methods supports hsrd drive/directories operations.
	////////////////////////////////////////////////////////////////////////////////
	class mmFileUtilsI
	{
		public:   // type definitions
			////////////////////////////////////////////////////////////////////////////////
			/// Structure defining file/directory on hard drive.
			////////////////////////////////////////////////////////////////////////////////
			typedef struct
			{
				////////////////////////////////////////////////////////////////////////////////
				/// Name of file/directory.
				////////////////////////////////////////////////////////////////////////////////
				mmString sName;
				////////////////////////////////////////////////////////////////////////////////
				/// TRUE if file, FALSE if directory.
				////////////////////////////////////////////////////////////////////////////////
				bool bFile;
			} sDirElement;
		public:		// methods
			////////////////////////////////////////////////////////////////////////////////
			/// This function creates directory. In case of error it throws
			/// mmeFileIONoSuchFileOrDirectory, mmeFileIOPermissionToFileDenied or
			/// mmeFileIOUnknownError.
			///
			/// @param[in] p_sDirName name of directory with path.
			////////////////////////////////////////////////////////////////////////////////
			virtual void CreateDir(mmString p_sDirName) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// Method creates directory(ies) in a given location.
			///
			/// @param[in] p_sDirName path to creation.
			////////////////////////////////////////////////////////////////////////////////
			virtual void CreateDirStructure(mmString p_sDirName) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// This method deletes all directory content. In case of error it throws
			/// mmeFileIONoSuchFileOrDirectory, mmeFileIOPermissionToFileDenied or
			/// mmeFileIOUnknownError.
			///
			/// @param[in] p_sDirName directory name with path.
			////////////////////////////////////////////////////////////////////////////////
			virtual void ClearDir(mmString p_sDirName) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// This method deletes directory. In case of error it throws
			/// mmeFileIONoSuchFileOrDirectory, mmeFileIOPermissionToFileDenied or
			/// mmeFileIOUnknownError.
			///
			/// @param[in] p_sDirName directory name with path,
			/// @param[in] p_bWithElements if TRUE it deletes also directory content,
			///            if FALSE deletes only if directory is empty.
			////////////////////////////////////////////////////////////////////////////////
			virtual void RemoveDir(mmString p_sDirName,bool p_bWithElements) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method checks if given directory exists. In case of error it throws
			/// mmeFileIOUnknownError.
			///
			/// @param[in] p_sDirName directory name with full path,
			/// @return if directory exists it returns TRUE, FALSE otherwise.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool IsExistingDir(mmString p_sDirName) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// Method returns path do temporary directory. In case of error it throws
			/// mmeFileIONoTemporaryDirectory.
			///
			/// @return full path to temporary directory
			////////////////////////////////////////////////////////////////////////////////
			virtual mmString GetTemporaryDir(void) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// This method returns full path to application directory.
			///
			/// @return full path to application
			////////////////////////////////////////////////////////////////////////////////
			virtual mmString GetApplicationDir(void) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// Method returns elements belonging to directory p_sDirName.
			///
			/// @param[in] p_sDirName full path to directory
			/// @param[in] p_sDirWildcards string defining search criterion eg "*.txt" or
			///							 "*.*",
			/// @return vector with directory elements.
			////////////////////////////////////////////////////////////////////////////////
			virtual std::vector<sDirElement> GetDirElements(mmString p_sDirName, mmString p_sDirWildcards) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// Method checks if given directory is empty or not.
			///
			/// @param[in] p_sDirName full path to directory,
			/// @return TRUE if directory is empty, FALSE otherwise.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool IsEmptyDir(mmString p_sDirName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method removes file from hard drive. In case of error it throws
			/// mmeFileIONoSuchFileOrDirectory, mmeFileIOPermissionToFileDenied or
			/// mmeFileIOUnknownError.
			///
			/// @param[in] p_sFileName name with full path of file to remove
			////////////////////////////////////////////////////////////////////////////////
			virtual void RemoveFile(mmString p_sFileName) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// This method checks if file exists.
			///
			/// @param[in] p_sFileName name with full path to checked file
			/// @return TRUE if file exists, FALSE otherwise
			////////////////////////////////////////////////////////////////////////////////
			virtual bool IsExistingFile(mmString p_sFileName) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// Method extracts path to file from full path with name.
			///
			/// @param[in] p_sFileName file name with full path,
			/// @return path to file.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmString GetPathToFile(mmString p_sFileName) = 0;

			virtual ~mmFileUtilsI() {}
	};
};

#endif
