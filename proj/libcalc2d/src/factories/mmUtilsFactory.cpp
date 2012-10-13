#include <factories/mmUtilsFactory.h>

#include <interfaces/mmInterfaceInitializers.h>

mmSynchronize::mmExclusiveLockI* mmFactories::mmUtilsFactory::CreateExclusiveLock(mmLog::mmLogReceiverI *p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateExclusiveLock(p_psLogReceiver);
}

mmSynchronize::mmReadWriteLockI* mmFactories::mmUtilsFactory::CreateReadWriteLock(mmLog::mmLogReceiverI *p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateReadWriteLock(p_psLogReceiver);
}

mmFileIO::mmFileUtilsI* mmFactories::mmUtilsFactory::CreateFileUtils(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateFileUtils(p_psLogReceiver);
}

mmLog::mmLogReceiverI* mmFactories::mmUtilsFactory::CreateLogFile(mmString const & p_sLogFileName)
{
	return mmInterfaceInitializers::CreateLogFile(p_sLogFileName);
}

mmLog::mmLogSenderI* mmFactories::mmUtilsFactory::CreateLogSender(mmString const & p_sClassName, void * const p_pClassPointer, mmLog::mmLogReceiverI * const p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateLogSender(p_sClassName, p_pClassPointer, p_psLogReceiver);
}

mmXML::mmXMLDocI* mmFactories::mmUtilsFactory::CreateXMLDocument(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateXMLDocument();
}

mmXML::mmXMLNodeI* mmFactories::mmUtilsFactory::CreateXMLNode(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateXMLNode();
}

mmThread::mmThreadI* mmFactories::mmUtilsFactory::CreateDefaultThread(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateDefaultThread(p_psLogReceiver);
}
