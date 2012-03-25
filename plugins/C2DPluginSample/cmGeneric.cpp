#include "cmGeneric.h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmGenericParams =
{
	L"TODO: method name",
	L"{TODO: GUID}",
	L"TODO: method description",
	false, // multi-threaded?
	{}, 
	{
		-1, 
		L"TODO: author's first name", 
		L"TODO: author's last name",
		L"TODO: author's e-mail address"
	}
};

mmImages::cmGeneric::cmGeneric(mmLog::mmLogReceiverI* p_psLogReceiver) :
	mmCalcMethod(p_psLogReceiver, L"cmGeneric")
{
	m_sCMParams = cmGenericParams;

	// TODO: initialize members

	// TODO: define UI parameters

}

bool mmImages::cmGeneric::Calculate()
{
	// TODO: perform processing on m_psImageStructure

	return true;
}