#include <mmExecutionContext.h>

mmExecutionContext::mmExecutionContext(mmFactories::mmUtilsFactoryI * const p_psUtilsFactory, mmLog::mmLogReceiverI * const p_psLogReceiver ) : 
	m_psUtilsFactory(p_psUtilsFactory), m_psLogReceiver(p_psLogReceiver) 
{}

mmLog::mmLogReceiverI* mmExecutionContext::GetLogReceiver(void) const {
	return m_psLogReceiver;
}

mmFactories::mmUtilsFactoryI * mmExecutionContext::GetUtilsFactory(void) const {
	return m_psUtilsFactory;
}


