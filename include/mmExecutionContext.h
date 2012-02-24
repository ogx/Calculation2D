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

#include <log\mmLogSender.h>
#include <interfaces\mmIUtilsFactory.h>

////////////////////////////////////////////////////////////////////////////////
/// Namespace for interface initializers.
////////////////////////////////////////////////////////////////////////////////
class mmExecutionContext {
public:
	////////////////////////////////////////////////////////////////////////////////
	/// This constructor initializes all private members.
	///
	/// @param[in] p_psUtilsFactory pointer to utils factory.
	/// @param[in] p_psLogReceiver pointer to log object.
	////////////////////////////////////////////////////////////////////////////////
	mmExecutionContext(mmFactories::mmUtilsFactoryI * const p_psUtilsFactory, mmLog::mmLogReceiverI * const p_psLogReceiver);
	////////////////////////////////////////////////////////////////////////////////
	/// Method returns pointer to utils factory.
	///
	/// @return pointer to utils factory
	////////////////////////////////////////////////////////////////////////////////
	mmFactories::mmUtilsFactoryI* GetUtilsFactory(void) const;
	////////////////////////////////////////////////////////////////////////////////
	/// Method returns pointer to log object.
	///
	/// @return pointer to log object
	////////////////////////////////////////////////////////////////////////////////
	mmLog::mmLogReceiverI* GetLogReceiver(void) const;
private:
	mmLog::mmLogReceiverI* const m_psLogReceiver;
	mmFactories::mmUtilsFactoryI* const m_psUtilsFactory;
};

#endif
