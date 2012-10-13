//******************************************************************************
//******************************************************************************
//
//  Execution Context class
//
//
//  Description: Class with pointers to global interfaces initialized objects.
//				 It is used for exchange main interface objects between core of 
//				 the system and other components.
//
//******************************************************************************
//******************************************************************************
#ifndef mmExecutionContextH
#define mmExecutionContextH

#include <interfaces\mmILog.h>
#include <interfaces\mmIUtilsFactory.h>

////////////////////////////////////////////////////////////////////////////////
/// Namespace for interface initializers.
////////////////////////////////////////////////////////////////////////////////
class mmExecutionContext {
public:
	mmLog::mmLogReceiverI* m_psLogReceiver;
	mmFactories::mmUtilsFactoryI* m_psUtilsFactory;
};

#endif
