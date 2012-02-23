#include <mmError.h>

////////////////////////////////////////////////////////////////////////////////
/// Table with error numbers. Table ends with -1 value.
////////////////////////////////////////////////////////////////////////////////
//const mmInt g_tErrorCodesTable[] = {20000,
//                                  	20100,
//																		-1};

mmError::mmError(mmInt p_iErrID)
{
	m_iErrorID = p_iErrID;
}

mmInt mmError::GetErrorCode(void)
{
	return m_iErrorID;
}

mmString mmError::GetErrorString(void)
{
	// TODO

	return mmString(L"no error string");
}
