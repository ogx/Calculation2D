#include "[!output CALCMETHOD_SAFE_NAME].h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmGenericParams =
{
	L"[!output STUDENT_SAFE_SIGN]: [!output CALCMETHOD_SHORT_NAME]",
	L"[!output CALCMETHOD_GUID]",
	L"[!output CALCMETHOD_DESCRIPTION]",
	false,
	{0, 0}
};

mmImages::[!output CALCMETHOD_SAFE_NAME]::[!output CALCMETHOD_SAFE_NAME](mmLog::mmLogReceiverI* p_psLogReceiver):
                                   mmCalcMethod(p_psLogReceiver, 
                                                 L"[!output CALCMETHOD_SAFE_NAME]")

{
	m_sCMParams = cmGenericParams;

	// TODO: initialize members

	// TODO: define UI parameters

	UpdateParameters();
}

void mmImages::[!output CALCMETHOD_SAFE_NAME]::RetrieveParameters()
{
	// TODO: update UI parameters
}

bool mmImages::[!output CALCMETHOD_SAFE_NAME]::Calculate()
{
	// TODO: add calculations

	return true;
}