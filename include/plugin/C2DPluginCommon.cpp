////////////////////////////////////////////
//     maintenance code, do not edit      //
////////////////////////////////////////////
	
#include <windows.h>
#include <mmExecutionContext.h>
#include <interfaces/mmIImages.h>

// forward declaration of C2DPlugin defined function
mmImages::mmImagesCalculationMethodI* GetCalculationMethod( mmInt const p_iCalculationMethodIndex, mmLog::mmLogReceiverI* p_psLogReceiver);

//---------------------------------------------------------------------------

mmInt GetCalculationMethodCount(void) {
	int i = 0;
	while(mmImages::mmImagesCalculationMethodI* v_psTempMethod = GetCalculationMethod(i, NULL)) {
		delete v_psTempMethod;
		i++;
	}
	return i;
}
//---------------------------------------------------------------------------

__declspec(dllexport) mmInt __stdcall mmDLLGetSupportedImagesCalculationMethodsCount( void )
{
	return ::GetCalculationMethodCount();
}
//---------------------------------------------------------------------------

__declspec(dllexport) mmImages::mmImagesCalculationMethodI::sCalculationMethodParams __stdcall mmDLLGetSupportedImagesCalculationMethodDef( mmInt p_iImpIndex )
{
	mmImages::mmImagesCalculationMethodI::sCalculationMethodParams v_sCMP = {L"", L"", L"", false, L"", L""};
	mmImages::mmImagesCalculationMethodI* _v_sImagesCalculationMethod = ::GetCalculationMethod(p_iImpIndex, NULL);

	if(_v_sImagesCalculationMethod != NULL) {
		v_sCMP = _v_sImagesCalculationMethod->GetCalculationMethodInfo();
		delete _v_sImagesCalculationMethod;
	}

	return v_sCMP;
}
//---------------------------------------------------------------------------

__declspec(dllexport) mmImages::mmImagesCalculationMethodI* __stdcall mmDLLCreateImagesCalculationMethod(mmExecutionContext* m_psExecutionContext, const wchar_t* p_pcImagesCalculationMethodName )
{
	mmString v_sImagesCalculationMethodName = p_pcImagesCalculationMethodName;
	mmInt v_iMethodIndex = -1;

	mmInt v_iCoPCMCount = mmDLLGetSupportedImagesCalculationMethodsCount();

	for( mmInt v_iCoPCM = 0; v_iCoPCM < v_iCoPCMCount; v_iCoPCM++ ) {

		mmImages::mmImagesCalculationMethodI::sCalculationMethodParams v_sCMP = mmDLLGetSupportedImagesCalculationMethodDef( v_iCoPCM );

		if( v_sImagesCalculationMethodName.compare( v_sCMP.sIDName ) == 0 ) {
			v_iMethodIndex = v_iCoPCM;
			break;
		}
	}

	return ::GetCalculationMethod(v_iMethodIndex, m_psExecutionContext->m_psLogReceiver);
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall mmDLLDestroyImagesCalculationMethod( mmImages::mmImagesCalculationMethodI* p_psImagesCalculationMethod )
{
	delete p_psImagesCalculationMethod;
}