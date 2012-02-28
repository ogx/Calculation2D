#include <plugin/C2DPluginCommon.cpp>

/*
 * This is the only file of the C2D common plug-in system you need to edit:
 * - do not alter wizard auto generated stubs
 * - follow the TODO list below
*/

// TODO: 1. include headers of your calculation methods
[!if CALCMETHOD_CREATE]
#include "[!output CALCMETHOD_SAFE_NAME].h"
[!endif]

// TODO: 2. add your calculation method here by adding another entry, 
// otherwise it will not be detected by the main application
mmImages::mmImagesCalculationMethodI* GetCalculationMethod( mmInt const p_iCalculationMethodIndex, 
                                                            mmLog::mmLogReceiverI* p_psLogReceiver) {
	switch(p_iCalculationMethodIndex) {
	[!if CALCMETHOD_CREATE]
	case __COUNTER__:
		return new mmImages::[!output CALCMETHOD_SAFE_NAME](p_psLogReceiver );
	[!endif]
	default:
		return NULL;
	};
}