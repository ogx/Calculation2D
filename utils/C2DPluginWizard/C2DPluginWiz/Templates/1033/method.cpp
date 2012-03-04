#include "[!output CALCMETHOD_SAFE_NAME].h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams [!output CALCMETHOD_SAFE_NAME]Params =
{
	L"[!output CALCMETHOD_SHORT_NAME]",
	L"[!output CALCMETHOD_GUID]",
	L"[!output CALCMETHOD_DESCRIPTION]",
	false,
	{0, 0},
	{[!output STUDENT_ID], L"[!output STUDENT_FIRSTNAME]", L"[!output STUDENT_LASTNAME]", L"[!output STUDENT_EMAIL]"},
	L"[!output CALCMETHOD_SAFE_NAME]"
};

mmImages::[!output CALCMETHOD_SAFE_NAME]::[!output CALCMETHOD_SAFE_NAME](mmLog::mmLogReceiverI* p_psLogReceiver):
mmCalcMethod(p_psLogReceiver, L"[!output CALCMETHOD_SAFE_NAME]")

{
	m_sCMParams = [!output CALCMETHOD_SAFE_NAME]Params;

	// TODO: initialize members

	// TODO: define UI parameters
}

bool mmImages::[!output CALCMETHOD_SAFE_NAME]::Calculate()
{
	// TODO: add calculations
	[!if CALCMETHOD_EXTEND]
	ForEachImage(this);
	[!endif]

	return true;
}

[!if CALCMETHOD_EXTEND]
void mmImages::[!output CALCMETHOD_SAFE_NAME]::OnBeforeEachImage( mmImageI* p_psCurrentImage )
{

}

void mmImages::[!output CALCMETHOD_SAFE_NAME]::OnAfterEachImage( mmImageI* p_psCurrentImage )
{

}

void mmImages::[!output CALCMETHOD_SAFE_NAME]::operator()( mmImageI* p_psCurrentImage, mmInt p_iFirstRow, mmInt p_iRowsCount )
{

}
[!endif]
