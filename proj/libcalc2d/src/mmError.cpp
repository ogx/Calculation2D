#include <mmError.h>

////////////////////////////////////////////////////////////////////////////////
/// Table with error numbers. Table ends with -1 value.
////////////////////////////////////////////////////////////////////////////////
//const mmInt g_tErrorCodesTable[] = {20000,
//                                  	20100,
//																		-1};

mmError::mmError(mmInt p_iErrID) :
	m_iErrorID(p_iErrID),
	m_sErrorText(L"No error description.") // TODO: provide error text lookup
{
}

mmError::mmError(mmString const & p_sErrorText) :
	m_iErrorID(mmeClientCustomError),
	m_sErrorText(p_sErrorText)
{
}

mmInt mmError::GetErrorCode(void) const
{
	return m_iErrorID;
}

mmString mmError::GetErrorString(void) const
{
	return m_sErrorText;
}
