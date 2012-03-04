#include <plugin/C2DPluginCommon.cpp>

/*
 * This is the only file of the C2D common plug-in system you need to edit:
 * - do not alter wizard auto generated stubs
 * - follow the TODO list below
*/

// TODO: 1. include headers of your calculation methods
#include "cmFlipImage.h"
#include "cmGeneric.h"

// TODO: 2. add your calculation method here by adding another entry, 
// otherwise it will not be detected by the main application
mmImages::mmImagesCalculationMethodI* GetCalculationMethod( mmInt const p_iCalculationMethodIndex, 
                                                            mmLog::mmLogReceiverI* p_psLogReceiver) {
	switch(p_iCalculationMethodIndex) {
	case __COUNTER__:
		return new mmImages::cmFlipImage(p_psLogReceiver );
	case __COUNTER__:
		return new mmImages::cmGeneric(p_psLogReceiver );
	default:
		return NULL;
	};
}