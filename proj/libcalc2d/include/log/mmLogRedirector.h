//******************************************************************************
//******************************************************************************
//
//  Log Redirector class
//
//
//  Description: This class is used for redirecting log messages from the
//							 system to log receivers clients like console, file, network
//							 log server, etc.
//
//******************************************************************************
//******************************************************************************
#ifndef mmLogRedirectorH
#define mmLogRedirectorH

#include <vector>

#include <interfaces\mmILog.h>
#include <interfaces\mmISynchronize.h>
#include <mmError.h>

namespace mmLog
{
	////////////////////////////////////////////////////////////////////////////////
	/// Implements interface mmLog::mmLogReceiverI. It allows to register many log
	///	receivers with their own log priorities. When redirector receive a log
	/// message then it sends this message to all registered receivers.
	////////////////////////////////////////////////////////////////////////////////
	class mmLogRedirector: public mmLogReceiverI
	{
		private:  // definitions
			////////////////////////////////////////////////////////////////////////////////
			/// Structure defining log receiver to forward log messages into.
			////////////////////////////////////////////////////////////////////////////////
			typedef struct
			{
				////////////////////////////////////////////////////////////////////////////////
				/// Lowest priority for messages received by pReceiver object.
				////////////////////////////////////////////////////////////////////////////////
				eLogMessagePriority ePriority;
				////////////////////////////////////////////////////////////////////////////////
				/// Pointer to log receiver object, if NULL then receiver is not active.
				////////////////////////////////////////////////////////////////////////////////
				mmLogReceiverI* pReceiver;
			} sLogReceiver;
		private:  // fields
			////////////////////////////////////////////////////////////////////////////////
			/// Vector with registered log receivers.
			////////////////////////////////////////////////////////////////////////////////
			std::vector<sLogReceiver> m_sReceiverTable;
			////////////////////////////////////////////////////////////////////////////////
			/// Synchronize log redirector work.
			////////////////////////////////////////////////////////////////////////////////
			mmSynchronize::mmExclusiveLockI* const m_psExclusiveLock;
    private:  // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Checks if p_pLogReceiver is registered already.
			///
			/// @param[in] p_pLogReceiver pointer to log receiver object to check
			/// @return TRUE if is registered already, FALSE otherwise.
			////////////////////////////////////////////////////////////////////////////////
			bool IsRegistered(mmLogReceiverI* const p_pLogReceiver) const;
			////////////////////////////////////////////////////////////////////////////////
			/// Returns index with free field in m_sReceiverTable, if there is no free place
			/// it creates new one and return its index.
			///
			/// @return index of free field
			////////////////////////////////////////////////////////////////////////////////
			mmInt GetFreeReceiverTableIndex(void);
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			////////////////////////////////////////////////////////////////////////////////
			mmLogRedirector();
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmLogRedirector();

			////////////////////////////////////////////////////////////////////////////////
			/// Method registers new class for receiving log messages. If object is already
			/// registered then throws mmError(mmeLogReceiverObjectAlreadyRegistered).
			///
			/// @param[in] p_eMinPriority minimal priority of received messages for this log
			///													  receiver object
			/// @param[in] p_pLogReceiver pointer to log object
			////////////////////////////////////////////////////////////////////////////////
			void RegisterLogReceiver(eLogMessagePriority const p_eMinPriority, mmLogReceiverI * const p_pLogReceiver);
			////////////////////////////////////////////////////////////////////////////////
			/// Unregisters log receiver object. If object is not registered throws
			/// mmError(mmeLogReceiverObjectIsNotRegistered).
			///
			/// @param[in] p_pLogReceiver pointer to log receiver.
			////////////////////////////////////////////////////////////////////////////////
			void UnregisterLogReceiver(mmLogReceiverI* const p_pLogReceiver);

			virtual void SendLogMessage(eLogMessagePriority const p_ePriority, mmString const & p_sString);
	};
};

#endif
