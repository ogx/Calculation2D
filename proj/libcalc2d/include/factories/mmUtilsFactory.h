//******************************************************************************
//******************************************************************************
//
//  Interface factories
//
//
//  Description: Implmentations of factories
//
//******************************************************************************
//******************************************************************************
#ifndef mmInterfaceFactoriesH
#define mmInterfaceFactoriesH

#include <interfaces\mmIUtilsFactory.h>

////////////////////////////////////////////////////////////////////////////////
/// Namespace for interface factories.
////////////////////////////////////////////////////////////////////////////////
namespace mmFactories
{
	class mmUtilsFactory : public mmUtilsFactoryI {
	public:
		virtual mmSynchronize::mmExclusiveLockI* CreateExclusiveLock(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);
		virtual mmSynchronize::mmReadWriteLockI* CreateReadWriteLock(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);
		virtual mmFileIO::mmFileUtilsI* CreateFileUtils(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);
		virtual mmLog::mmLogReceiverI* CreateLogFile(mmString const & p_sLogFileName);
		virtual mmLog::mmLogSenderI* CreateLogSender(mmString const & p_sClassName, void * const p_pClassPointer, mmLog::mmLogReceiverI * const p_psLogReceiver);
		virtual mmXML::mmXMLDocI* CreateXMLDocument(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);
		virtual mmXML::mmXMLNodeI* CreateXMLNode(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);
		virtual mmThread::mmThreadI* CreateDefaultThread(mmLog::mmLogReceiverI* p_psLogReceiver = NULL);
	};
};

#endif
