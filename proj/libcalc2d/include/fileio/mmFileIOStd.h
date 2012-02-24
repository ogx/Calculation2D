//******************************************************************************
//******************************************************************************
//
//  FileIO STD implementation
//
//
//  Description: This header defines implementation of all interfaces from
//							 FileIO namespace.
//
//******************************************************************************
//******************************************************************************
#ifndef mmFileIOSTDH
#define mmFileIOSTDH

#include <fstream>

#include <interfaces\mmIFileIO.h>
#include <log\mmLogSender.h>

namespace mmFileIO
{
	////////////////////////////////////////////////////////////////////////////////
	/// This class implements mmFileIO::mmFileUtilsI interface with use of
	/// C++ STD library. It defines functions supporting file/directory manipulation.
	////////////////////////////////////////////////////////////////////////////////
	class mmFileUtilsSTD: public mmFileUtilsI, mmLog::mmLogSender
	{
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			/// @param[in] p_psLogReceiver pointer to log object.
			////////////////////////////////////////////////////////////////////////////////
			mmFileUtilsSTD(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmFileUtilsSTD();

			void CreateDir(mmString p_sDirName);
			void CreateDirStructure(mmString p_sDirName);
			void ClearDir(mmString p_sDirName);
			void RemoveDir(mmString p_sDirName,bool p_bWithElements);

			bool IsExistingDir(mmString p_sDirName);
			mmString GetTemporaryDir(void);
			mmString GetApplicationDir(void);
			std::vector<sDirElement> GetDirElements(mmString p_sDirName, mmString p_sDirWildcards);
			bool IsEmptyDir(mmString p_sDirName);

			void RemoveFile(mmString p_sFileName);
			bool IsExistingFile(mmString p_sFileName);
			mmString GetPathToFile(mmString p_sFileName);
	};

};

#endif
