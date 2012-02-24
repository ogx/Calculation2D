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
	mmLog::mmLogReceiverI* CreateLogFile(mmString p_sLogFileName);

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
	/// Initializes interface mmImages::mmImagesStructureI. In case of
	/// bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmImages::mmImageStructureI* CreateDefaultImageStructure(mmImages::mmImageI::mmCallbackI * p_psCallback, 
																mmLog::mmLogReceiverI* p_psLogReceiver);

	////////////////////////////////////////////////////////////////////////////////
	/// Initializes interface mmImages::mmImagesCalculationMethodContainerI.
	/// In case of bad alloc error throws mmError(mmeBadAlloc).
	///
	/// @param[in] p_psUIMainWindow pointer to UI main window object
	/// @param[in] p_psLogReceiver pointer to log object
	/// @return pointer to initialized object
	////////////////////////////////////////////////////////////////////////////////
	mmImages::mmImagesCalculationMethodContainerI* CreateDefaultImagesCalculationMethodContainer(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);

};

#endif
