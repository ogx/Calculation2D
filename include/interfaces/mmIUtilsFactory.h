#ifndef mmIUtilsFactoryH
#define mmIUtilsFactoryH

#include <interfaces\mmILog.h>
#include <interfaces\mmISynchronize.h>
#include <interfaces\mmIFileIO.h>
#include <interfaces\mmIXML.h>
#include <interfaces\mmIThread.h>

namespace mmFactories {
	class mmUtilsFactoryI {
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Initializes interface mmSynchronize::mmExclusiveLockI. In case of bad alloc
		/// error throws mmError(mmeBadAlloc).
		///
		/// @param[in] p_psLogReceiver pointer to log object
		/// @return pointer to initialized object
		////////////////////////////////////////////////////////////////////////////////
		virtual mmSynchronize::mmExclusiveLockI* CreateExclusiveLock(mmLog::mmLogReceiverI* p_psLogReceiver = NULL) = 0;

		////////////////////////////////////////////////////////////////////////////////
		/// Initializes interface mmSynchronize::mmReadWriteLockI. In case of
		/// bad alloc error throws mmError(mmeBadAlloc).
		///
		/// @param[in] p_psLogReceiver pointer to log object
		/// @return pointer to initialized object
		////////////////////////////////////////////////////////////////////////////////
		virtual mmSynchronize::mmReadWriteLockI* CreateReadWriteLock(mmLog::mmLogReceiverI* p_psLogReceiver = NULL) = 0;

		////////////////////////////////////////////////////////////////////////////////
		/// Initializes interface mmFileIO::mmFileUtilsI. In case of
		/// bad alloc error throws mmError(mmeBadAlloc).
		///
		/// @param[in] p_psLogReceiver pointer to log object
		/// @return pointer to initialized object
		////////////////////////////////////////////////////////////////////////////////
		virtual mmFileIO::mmFileUtilsI* CreateFileUtils(mmLog::mmLogReceiverI* p_psLogReceiver = NULL) = 0;

		////////////////////////////////////////////////////////////////////////////////
		/// Initializes interface mmLog::mmLogSenderI. In case of
		/// bad alloc error throws mmError(mmeBadAlloc).
		///
		/// @param[in] p_sClassName name of class sending logs
		/// @param[in] p_pClassPointer pointer to class sending logs
		/// @param[in] p_psLogRecever log receiver
		/// @return pointer to initialized object
		////////////////////////////////////////////////////////////////////////////////
		virtual mmLog::mmLogSenderI* CreateLogSender(mmString const & p_sClassName, void * const p_pClassPointer, mmLog::mmLogReceiverI * const p_psLogReceiver) = 0;

		////////////////////////////////////////////////////////////////////////////////
		/// Initializes interface mmLog::mmLogReceiverI. In case of
		/// bad alloc error throws mmError(mmeBadAlloc).
		///
		/// @param[in] p_sLogFileName name of log file
		/// @return pointer to initialized object
		////////////////////////////////////////////////////////////////////////////////
		virtual mmLog::mmLogReceiverI* CreateLogFile(mmString const & p_sLogFileName) = 0;

		////////////////////////////////////////////////////////////////////////////////
		/// Initializes interface mmXML::mmXMLDocI. In case of
		/// bad alloc error throws mmError(mmeBadAlloc).
		///
		/// @param[in] p_psLogReceiver pointer to log object
		/// @return pointer to initialized object
		////////////////////////////////////////////////////////////////////////////////
		virtual mmXML::mmXMLDocI* CreateXMLDocument(mmLog::mmLogReceiverI* p_psLogReceiver = NULL) = 0;

		////////////////////////////////////////////////////////////////////////////////
		/// Initializes interface mmXML::mmXMLNodeI. In case of
		/// bad alloc error throws mmError(mmeBadAlloc).
		///
		/// @param[in] p_psLogReceiver pointer to log object
		/// @return pointer to initialized object
		////////////////////////////////////////////////////////////////////////////////
		virtual mmXML::mmXMLNodeI* CreateXMLNode(mmLog::mmLogReceiverI* p_psLogReceiver = NULL) = 0;

		////////////////////////////////////////////////////////////////////////////////
		/// Initializes interface mmThread::mmThreadI. In case of
		/// bad alloc error throws mmError(mmeBadAlloc).
		///
		/// @param[in] p_psLogReceiver pointer to log object
		/// @return pointer to initialized object
		////////////////////////////////////////////////////////////////////////////////
		virtual mmThread::mmThreadI* CreateDefaultThread(mmLog::mmLogReceiverI* p_psLogReceiver = NULL) = 0;

		virtual ~mmUtilsFactoryI(void) {};
	};
};

#endif