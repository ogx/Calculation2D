#include <factories/mmUtilsFactory.h>

#include <mmInterfaceInitializers.h>

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

mmXML::mmXMLDocI* mmFactories::mmUtilsFactory::CreateXMLDocument(void)
{
	return mmInterfaceInitializers::CreateXMLDocument();
}

mmXML::mmXMLNodeI* mmFactories::mmUtilsFactory::CreateXMLNode(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateXMLNode();
}
mmThread::mmThreadI* mmFactories::CreateDefaultThread(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	return mmInterfaceInitializers::CreateDefaultThread(p_psLogReceiver);
}
