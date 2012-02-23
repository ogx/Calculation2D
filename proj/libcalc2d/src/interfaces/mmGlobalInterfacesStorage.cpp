#include <interfaces\mmGlobalInterfacesStorage.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmInterfacesStorage::mmGlobalInterfacesStorage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//mmInterfacesStorage::mmGlobalInterfacesStorage::mmGlobalInterfacesStorage(mmMemory::mmMemoryManagerI* p_psMemoryManager,
//	mmLog::mmLogReceiverI* p_psLogReceiver ): mmLog::mmLogSender(L"mmInterfacesStorage::mmGlobalInterfacesStorage",p_psLogReceiver)
//
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));
//
//	m_psMemoryManager = p_psMemoryManager;
//
//	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
//}
//
//mmInterfacesStorage::mmGlobalInterfacesStorage::~mmGlobalInterfacesStorage()
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));
//
//	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
//}
//
//mmMemory::mmMemoryManagerI* mmInterfacesStorage::mmGlobalInterfacesStorage::GetMemoryManager(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetMemoryManager"));
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetMemoryManager"));
//
//	return m_psMemoryManager;
//}
//
//mmLog::mmLogReceiverI* mmInterfacesStorage::mmGlobalInterfacesStorage::GetLogManager(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetLogManager"));
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetLogManager"));
//
//	return this->GetLogReceiver();
//}
//
mmInterfacesStorage::mmGlobalInterfacesStorage::mmGlobalInterfacesStorage(mmLog::mmLogReceiverI* p_psLogReceiver ) : m_psLogReceiver(p_psLogReceiver) {}

mmLog::mmLogReceiverI* mmInterfacesStorage::mmGlobalInterfacesStorage::GetLogManager(void) {
	return m_psLogReceiver;
}

mmLog::mmLogReceiverI* mmInterfacesStorage::mmGlobalInterfacesStorage::GetLogReceiver(void) {
	return m_psLogReceiver;
}
