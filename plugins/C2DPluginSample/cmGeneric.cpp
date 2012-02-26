#include "cmGeneric.h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmGenericParams =
{
	L"Generic (example)",
	L"{TODO: insert GUID here}",
	L"Sample: Generic",
	false,
	{0, 0}
};

mmImages::cmGeneric::cmGeneric(mmLog::mmLogReceiverI* p_psLogReceiver):
                                   mmCalcMethod(p_psLogReceiver, 
                                                 L"cmGeneric")

{
	m_sCMParams = cmGenericParams;

	// TODO: initialize members

	// TODO: define UI parameters

	UpdateParameters();
}

void mmImages::cmGeneric::RetrieveParameters()
{
	// TODO: update UI parameters
}

bool mmImages::cmGeneric::Calculate()
{
	// TODO: add calculations

	return true;
}