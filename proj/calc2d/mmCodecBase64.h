//******************************************************************************
//******************************************************************************
//
//  Base64 conversion utility functions
//
//
//  Description: Complementary functions for coding / decoding Base64 strings
//  using optimized procedures
//
//******************************************************************************
//******************************************************************************
#pragma once

#include <mmGlobalDefs.h>
#include <vector>

namespace base64 {

	////////////////////////////////////////////////////////////////////////////////
	/// Definition of the supported vector type
	////////////////////////////////////////////////////////////////////////////////
	typedef std::vector<unsigned char> byte_vect;

	////////////////////////////////////////////////////////////////////////////////
	/// Function for decoding byte vectors from Base64 char strings.
	/// Because of speed optimization it doesn't perform any string validation
	/// but should NULL terminate the output string.
	/// Throws if string is not padded correctly with equal signs.
	///
	/// @param[in] input - Base64 filled string - padding is mandatory.
	/// @param[out] output - vector containing the decoded binary stream
	////////////////////////////////////////////////////////////////////////////////
	byte_vect decode(const std::string& input); //throws

	////////////////////////////////////////////////////////////////////////////////
	/// Function for converting byte vectors to Base64 char strings.
	/// Warning! Function may alter the input vector if its size is not a 
	/// multiple of three by appending up to two bytes.
	///
	/// @param[in,out] input - a Base64 filled string. Padding is mandatory.
	/// @param[out] output - a vector containing the decoded binary stream
	////////////////////////////////////////////////////////////////////////////////
	std::string encode(byte_vect& input);
}