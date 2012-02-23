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
	/// Function returns current file position.
	///
	/// @param[in] p_psFile pointer to file object
	/// @return file position in bytes counted from beginning.
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetFilePosition(std::ifstream* p_psFile);

  ////////////////////////////////////////////////////////////////////////////////
	/// Function returns current file position.
	///
	/// @param[in] p_psFile pointer to file object
	/// @return file position in bytes counted from beginning.
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetFilePosition(std::wifstream* p_psFile);

	////////////////////////////////////////////////////////////////////////////////
	/// Function returns current file position.
	///
	/// @param[in] p_psFile pointer to file object
	/// @return file position in bytes counted from beginning.
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetFilePosition(std::ofstream* p_psFile);

	////////////////////////////////////////////////////////////////////////////////
	/// Function returns current file position.
	///
	/// @param[in] p_psFile pointer to file object
	/// @return file position in bytes counted from beginning.
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetFilePosition(std::wofstream* p_psFile);

	////////////////////////////////////////////////////////////////////////////////
	/// Function sets current file position.
	///
	/// @param[in] p_psFile pointer to file object
	/// @param[in] p_iFilePos file position in bytes counted from beginning.
	////////////////////////////////////////////////////////////////////////////////
	void SetFilePosition(std::ifstream* p_psFile,mmInt p_iFilePos);

  ////////////////////////////////////////////////////////////////////////////////
	/// Function sets current file position.
	///
	/// @param[in] p_psFile pointer to file object
	/// @param[in] p_iFilePos file position in bytes counted from beginning.
	////////////////////////////////////////////////////////////////////////////////
	void SetFilePosition(std::wifstream* p_psFile,mmInt p_iFilePos);

	////////////////////////////////////////////////////////////////////////////////
	/// Function sets current file position.
	///
	/// @param[in] p_psFile pointer to file object
	/// @param[in] p_iFilePos file position in bytes counted from beginning.
	////////////////////////////////////////////////////////////////////////////////
	void SetFilePosition(std::ofstream* p_psFile,mmInt p_iFilePos);

  ////////////////////////////////////////////////////////////////////////////////
	/// Function sets current file position.
	///
	/// @param[in] p_psFile pointer to file object
	/// @param[in] p_iFilePos file position in bytes counted from beginning.
	////////////////////////////////////////////////////////////////////////////////
	void SetFilePosition(std::wofstream* p_psFile,mmInt p_iFilePos);

	////////////////////////////////////////////////////////////////////////////////
	/// Function returns file length in bytes.
	///
	/// @param[in] p_psFile pointer to file object
	/// @return length of file in bytes.
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetFileLenght(std::ifstream* p_psFile);

  ////////////////////////////////////////////////////////////////////////////////
	/// Function returns file length in bytes.
	///
	/// @param[in] p_psFile pointer to file object
	/// @return length of file in bytes.
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetFileLenght(std::wifstream* p_psFile);

	////////////////////////////////////////////////////////////////////////////////
	/// Function returns file length in bytes.
	///
	/// @param[in] p_psFile pointer to file object
	/// @return length of file in bytes.
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetFileLenght(std::ofstream* p_psFile);

	////////////////////////////////////////////////////////////////////////////////
	/// Function returns file length in bytes.
	///
	/// @param[in] p_psFile pointer to file object
	/// @return length of file in bytes.
	////////////////////////////////////////////////////////////////////////////////
	mmInt GetFileLenght(std::wofstream* p_psFile);

	////////////////////////////////////////////////////////////////////////////////
	/// This class implements mmFileIO::mmFileUtilsI interface with use of
	/// C++ STD library. It defines functions supporting file/directory manipulation.
	////////////////////////////////////////////////////////////////////////////////
	class mmFileUtilsSTD: public mmFileUtilsI,
												mmLog::mmLogSender
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
			std::vector<sDirElement> GetDirElements(mmString p_sDirName,
																							mmString p_sDirWildcards);
			bool IsEmptyDir(mmString p_sDirName);

			void RemoveFile(mmString p_sFileName);
			bool IsExistingFile(mmString p_sFileName);
			mmString GetPathToFile(mmString p_sFileName);
	};

	////////////////////////////////////////////////////////////////////////////////
	/// This class implements mmFileIO::mmTextFileReadI interface with use of
	/// C++ STD library.
	////////////////////////////////////////////////////////////////////////////////
	class mmTextFileReadSTD: public mmTextFileReadI,
																	mmLog::mmLogSender
	{
		private:  // fields
			////////////////////////////////////////////////////////////////////////////////
			/// Pointer to file object from STD library.
			////////////////////////////////////////////////////////////////////////////////
			std::wifstream* m_psFile;
			////////////////////////////////////////////////////////////////////////////////
			/// File name which is open.
			////////////////////////////////////////////////////////////////////////////////
			mmString m_sFileName;
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Class constructor.
			///
			/// @param[in] p_psLogReceiver pointer to log object.
			////////////////////////////////////////////////////////////////////////////////
			mmTextFileReadSTD(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmTextFileReadSTD();

			void Open(mmString p_sFileName);
			void Close(void);
			bool ReadChar(wchar_t* p_pcChar);
			bool ReadLine(mmString* p_psFileLine);
			mmInt ReadLines(std::vector<mmString>* p_psLines,mmInt p_iLinesCount);
			mmInt GetLenght(void);
			mmInt ReadWholeFile(wchar_t* p_pcFileContent);
			bool Eof(void);
			bool IsActiveFile(void);
			mmInt GetActualFilePosition(void);
			mmString GetFileName(void);
	};

	////////////////////////////////////////////////////////////////////////////////
	/// This class implements mmFileIO::mmTextFileWriteI interface with use of
	/// C++ STD library.
	////////////////////////////////////////////////////////////////////////////////
	class mmTextFileWriteSTD: public mmTextFileWriteI,
																	 mmLog::mmLogSender
	{
		private:  // fields
			////////////////////////////////////////////////////////////////////////////////
			/// pointer to file object from STD library.
			////////////////////////////////////////////////////////////////////////////////
			std::wofstream* m_psFile;
			////////////////////////////////////////////////////////////////////////////////
			/// File name.
			////////////////////////////////////////////////////////////////////////////////
			mmString m_sFileName;
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			///`@param[in] p_psLogReceiver pointer to log object.
			////////////////////////////////////////////////////////////////////////////////
			mmTextFileWriteSTD(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmTextFileWriteSTD();

			void Open(mmString p_sFileName,
								eOpenFileMode p_eOpenMode);
			void Close(void);
			void WriteChar(wchar_t p_cChar);
			void WriteLine(mmString p_sFileLine);
			void WriteLines(std::vector<mmString>* p_psLines);
			void WriteBuffer(wchar_t const* p_pcBuffer,mmInt p_iBufferSize);
			void WriteEol(void);
			void Flush(void);
			bool IsActiveFile(void);
			mmString GetFileName(void);
	};

	////////////////////////////////////////////////////////////////////////////////
	/// This class implements mmFileIO::mmBinaryFileReadI interface with use of
	/// C++ STD library.
	////////////////////////////////////////////////////////////////////////////////
	class mmBinaryFileReadSTD: public mmBinaryFileReadI,
																		mmLog::mmLogSender
	{
		private:  // fields
			////////////////////////////////////////////////////////////////////////////////
			/// Pointer to file object from STD library.
			////////////////////////////////////////////////////////////////////////////////
			std::ifstream* m_psFile;
			////////////////////////////////////////////////////////////////////////////////
			/// File name which is open.
			////////////////////////////////////////////////////////////////////////////////
			mmString m_sFileName;
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Class constructor.
			///
			/// @param[in] p_psLogReceiver pointer to log object.
			////////////////////////////////////////////////////////////////////////////////
			mmBinaryFileReadSTD(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmBinaryFileReadSTD();

			void Open(mmString p_sFileName);
			void Close(void);
			bool ReadBuffer(void* p_pBuffer,mmInt p_iSize);
			mmInt GetLenght(void);
			bool Eof(void);
			bool IsActiveFile(void);
			mmInt GetActualFilePosition(void);
			void SetActualFilePosition(mmInt p_iFilePos);
			mmString GetFileName(void);
	};

	////////////////////////////////////////////////////////////////////////////////
	/// This class implements mmFileIO::mmBinaryFileWriteI interface with use of
	/// C++ STD library.
	////////////////////////////////////////////////////////////////////////////////
	class mmBinaryFileWriteSTD: public mmBinaryFileWriteI,
																		 mmLog::mmLogSender
	{
		private:  // fields
			////////////////////////////////////////////////////////////////////////////////
			/// pointer to file object from STD library.
			////////////////////////////////////////////////////////////////////////////////
			std::ofstream* m_psFile;
			////////////////////////////////////////////////////////////////////////////////
			/// File name.
			////////////////////////////////////////////////////////////////////////////////
			mmString m_sFileName;
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			///`@param[in] p_psLogReceiver pointer to log object.
			////////////////////////////////////////////////////////////////////////////////
			mmBinaryFileWriteSTD(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmBinaryFileWriteSTD();

			void Open(mmString p_sFileName,
								mmFileIO::eOpenFileMode p_eOpenMode);
			void Close(void);
			bool WriteBuffer(void* p_pBuffer,mmInt p_iSize);
			mmInt GetLenght(void);
			bool IsActiveFile(void);
			mmInt GetActualFilePosition(void);
			void SetActualFilePosition(mmInt p_iFilePos);
			mmString GetFileName(void);
			void Flush(void);
	};
};

#endif
