//******************************************************************************
//******************************************************************************
//
//  Log Sender class
//
//
//  Description: This class implements mmLogReceiverI interface and supports
//							 all other classes with transparent logging capability.
//
//******************************************************************************
//******************************************************************************
#ifndef mmLogSenderH
#define mmLogSenderH

#include <interfaces\mmILog.h>

namespace mmLog
{
	////////////////////////////////////////////////////////////////////////////////
	/// This class support transparent use of logging capability. It should be
	///	added as ancestor to any class of the system.
	////////////////////////////////////////////////////////////////////////////////
	class mmLogSender : public mmLogSenderI
	{
		private:    // fields
			////////////////////////////////////////////////////////////////////////////////
			/// Name of class which is descendant of mmLogSender class and uses it for logging.
			////////////////////////////////////////////////////////////////////////////////
			mmString const m_sClassName;
			////////////////////////////////////////////////////////////////////////////////
			/// Pointer to mmLogReceiverI object.
			////////////////////////////////////////////////////////////////////////////////
			mmLogReceiverI * const m_psLog;
			////////////////////////////////////////////////////////////////////////////////
			/// Pointer to class.
			////////////////////////////////////////////////////////////////////////////////
			void * const m_pClassPointer;
		public:     // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Initializes fields. If p_psLog is NULL then logging support is disabled.
			///
			/// @param[in] p_sClassName - name of ancestor class.
			/// @param[in] p_psLog - pointer to mmLogReceiverI object which receives log messages
			////////////////////////////////////////////////////////////////////////////////
			mmLogSender(mmString const & p_sClassName, mmLogReceiverI * const p_psLog = NULL, void * const p_pClassPointer = NULL);

			////////////////////////////////////////////////////////////////////////////////
			/// Method sends a log message with defined priority. If m_psLog is equal NULL
			/// then do	nothing.
			///
			/// @param[in] p_pClassPointer pointer to class sending this message
			/// @param[in] p_ePriority type of log message
			/// @param[in] p_sString content of message
			////////////////////////////////////////////////////////////////////////////////
			virtual void SendLogMessage(void * const p_pClassPointer, eLogMessagePriority const p_ePriority, mmString const & p_sString);

			////////////////////////////////////////////////////////////////////////////////
			/// Return a pointer to mmLogReceiverI object stored in m_psLog field.
			///
			/// @return pointer to log receiver.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmLogReceiverI* GetLogReceiver(void);
		protected:  // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Method sends a log message with defined priority. If m_psLog is equal NULL
			/// then do	nothing.
			///
			/// @param[in] p_ePriority type of log message
			/// @param[in] p_sString content of message
			////////////////////////////////////////////////////////////////////////////////
			virtual void SendLogMessage(mmLog::eLogMessagePriority const p_ePriority, mmString const & p_sString);
	};
};

#endif
