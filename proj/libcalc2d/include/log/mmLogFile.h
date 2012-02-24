//******************************************************************************
//******************************************************************************
//
//  Log File class
//
//
//  Description: This class implements interface mmLogReceiverI and it supports
//               file logging.
//
//******************************************************************************
//******************************************************************************
#ifndef mmLogFileH
#define mmLogFileH

#include <interfaces\mmILog.h>
#include <interfaces\mmIFileIO.h>

#include <fstream>

namespace mmLog
{
	////////////////////////////////////////////////////////////////////////////////
	/// Implementation of log receiver interface for file logging.
	////////////////////////////////////////////////////////////////////////////////
	class mmLogFile: public mmLogReceiverI
	{
		private:	// fields
			////////////////////////////////////////////////////////////////////////////////
			/// Pointer to file object where log information is stored.
			////////////////////////////////////////////////////////////////////////////////
    		std::wofstream m_sLogFile;
		public:   // methods
    		////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			/// @param[in] p_sLogFileName name of log file to open
			////////////////////////////////////////////////////////////////////////////////
			mmLogFile(mmString p_sLogFileName);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmLogFile();

			void SendLogMessage(eLogMessagePriority p_ePriority,mmString* p_psString);
	};
};

#endif
