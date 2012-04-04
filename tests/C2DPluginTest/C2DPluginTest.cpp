#include <plugin/C2DPluginCommon.cpp>
#include "cmLayerTransport.h"
#include "cmExceptionHandling.h"

mmImages::mmImagesCalculationMethodI* GetCalculationMethod( mmInt const p_iCalculationMethodIndex, 
                                                            mmLog::mmLogReceiverI* p_psLogReceiver) {
	switch(p_iCalculationMethodIndex) {
	case __COUNTER__:
		return new mmImages::cmLayerTransport(p_psLogReceiver );
	case __COUNTER__:
		return new mmImages::cmExceptionHandling(p_psLogReceiver );
	default:
		return NULL;
	};
}