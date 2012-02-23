//******************************************************************************
//******************************************************************************
//
//  DLL support for Windows OS class
//
//
//  Description: This header defines useful functions for Windows OS using dlls.
//
//******************************************************************************
//******************************************************************************
#ifndef mmDLLSupportForWindowsOSH
#define mmDLLSupportForWindowsOSH

#include <mmGlobalDefs.h>

namespace mmDLLSupport
{
	////////////////////////////////////////////////////////////////////////////////
	/// This function searches in DLL export table for specified symbol and returns
	/// full exported symbol.
	///
	/// @param[in] p_sDLLName DLL name with full path
	/// @param[in] p_sName name of symbol
	/// @return full name of symbol in DLL export table
	////////////////////////////////////////////////////////////////////////////////
	mmString FindSymbolInDLLExportTable(mmString p_sDLLName,
																			mmString p_sName);
};

#endif


