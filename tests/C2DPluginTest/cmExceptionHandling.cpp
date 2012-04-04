#include "cmExceptionHandling.h"
#include "mmError.h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmExceptionHandlingParams =
{
	L"Error handling test",
	L"{6C712063-2888-41AC-8184-CC4FEA2B1498}",
	L"Raises an exception when launched. Lets one test the ability to handle client exceptions.",
	false,
	{0, 0},
	{0, L"OGX", L"", L""},
	L"cmExceptionHandling"
};

mmImages::cmExceptionHandling::cmExceptionHandling(mmLog::mmLogReceiverI* p_psLogReceiver):
	mmCalcMethod(p_psLogReceiver, L"cmExceptionHandling test")
{
	// overwrite default parameters
	m_sCMParams = cmExceptionHandlingParams;

	// members initialization

	// input parameters

	// output parameters
}

bool mmImages::cmExceptionHandling::Calculate()
{
	throw mmError(L"Intentional test exception.");

	return true;
}