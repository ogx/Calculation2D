//******************************************************************************
//******************************************************************************
//
//  Interface initalizers
//
//
//  Description: Set of functions for unified interface initialization.
//
//******************************************************************************
//******************************************************************************
#ifndef mmInterfaceInitializersH
#define mmInterfaceInitializersH

#include <interfaces\mmILog.h>
#include <interfaces\mmISynchronize.h>
#include <interfaces\mmIFileIO.h>
#include <interfaces\mmIXML.h>

#include <interfaces\mmIThread.h>
#include <interfaces\mmIImages.h>
#include <interfaces\mmICalculationMethodContainer.h>
#include <interfaces\mmIUtilsFactory.h>

////////////////////////////////////////////////////////////////////////////////
/// Namespace for interface initializers.
////////////////////////////////////////////////////////////////////////////////
namespace mmInterfaceInitializers
{
	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmSynchronize::mmExclusiveLockI. In case of bad alloc
	/// error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmSynchronize::mmExclusiveLockI* CreateExclusiveLock(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmSynchronize::mmReadWriteLockI. In case of
	/// bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmSynchronize::mmReadWriteLockI* CreateReadWriteLock(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmFileIO::mmFileUtilsI. In case of
	/// bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmFileIO::mmFileUtilsI* CreateFileUtils(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmLog::mmLogReceiverI. In case of
	/// bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_sLogFileName name of log file
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmLog::mmLogReceiverI* CreateLogFile(mmString const & p_sLogFileName);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmLog::mmLogSenderI. In case of
	/// bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_sClassName name of class sending logs
	/// @param[in] p_pClassPointer pointer to class sending logs
	/// @param[in] p_psLogRecever log receiver
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmLog::mmLogSenderI* CreateLogSender(mmString const & p_sClassName, void * const p_pClassPointer, mmLog::mmLogReceiverI * const p_psLogReceiver);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmXML::mmXMLDocI. In case of
	/// bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmXML::mmXMLDocI* CreateXMLDocument(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmXML::mmXMLNodeI. In case of
	/// bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmXML::mmXMLNodeI* CreateXMLNode(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmThread::mmThreadI. In case of
	/// bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmThread::mmThreadI* CreateDefaultThread(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmImages::mmImagesCalculationMethodContainerI.
	/// In case of bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmImages::mmImagesCalculationMethodContainerI* CreateDefaultImagesCalculationMethodContainer(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);

	////////////////////////////////////////////////////////////////////////////////
	/// Creates a factory with utilities
	/// In case of bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmFactories::mmUtilsFactoryI* CreateUtilsFactory(void);

};

#endif
