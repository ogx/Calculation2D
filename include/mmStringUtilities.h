//******************************************************************************
//******************************************************************************
//
//  String utilities
//  Description: This namespace extends functionality of std::string.
//
//******************************************************************************
//******************************************************************************
#ifndef mmStringUtilitiesH
#define mmStringUtilitiesH

#include <mmGlobalDefs.h>
#include <mmError.h>

#include <vector>
#include <sstream>

#include <interfaces/mmIHardware.h>

////////////////////////////////////////////////////////////////////////////////
/// Namespace with extension of std::string functionality.
////////////////////////////////////////////////////////////////////////////////
namespace mmStringUtilities
{
  ////////////////////////////////////////////////////////////////////////////////
	/// Converts boolean value to string.
	///
	/// @param[in] p_bInput boolean value
	/// @return string representation of p_bInput
	////////////////////////////////////////////////////////////////////////////////
	mmString BoolToString(bool p_bInput);
	////////////////////////////////////////////////////////////////////////////////
	/// Converts integer value into string.
	///
	/// @param[in] p_iInput integer value
	/// @param[in] p_iLength output string lenght (fill by 0)
	/// @return string representation of p_iInput
	////////////////////////////////////////////////////////////////////////////////
	mmString MMIntToString(mmInt p_iInput,
												 mmInt p_iLength = 0);
	////////////////////////////////////////////////////////////////////////////////
	/// Converts real value to string
	///
	/// @param[in] p_rInput real value
	/// @param[in] p_iPrecision number of digits after dot
	/// @return output string.
	////////////////////////////////////////////////////////////////////////////////
	mmString MMRealToString(mmReal p_rInput,mmInt p_iPrecision = -1);
	////////////////////////////////////////////////////////////////////////////////
	/// Converts pointer to string.
	///
	/// @param[in] p_pInput input pointer
	/// @return output string.
	////////////////////////////////////////////////////////////////////////////////
	mmString PointerToString(void* p_pInput);

	////////////////////////////////////////////////////////////////////////////////
	/// Converts image pixel format to human readable string
	////////////////////////////////////////////////////////////////////////////////
	mmString PixelFormatToString(mmImageProcessing::ePixelType p_ePixelType);

	////////////////////////////////////////////////////////////////////////////////
	/// Converts string to image pixel format
	////////////////////////////////////////////////////////////////////////////////
	mmImageProcessing::ePixelType StringToPixelFormat(mmString p_sStr);

	////////////////////////////////////////////////////////////////////////////////
	/// Converts string to integer value
	///
	/// @param[in] p_sStr input string
	/// @return output integer
	////////////////////////////////////////////////////////////////////////////////
	mmInt StringToMMInt(mmString p_sStr);
	////////////////////////////////////////////////////////////////////////////////
	/// Converts string to mmReal value
	///
	/// @param[in] p_sStr input string
	/// @return output mmReal
	////////////////////////////////////////////////////////////////////////////////
	mmReal StringToMMReal(mmString p_sStr);
	////////////////////////////////////////////////////////////////////////////////
	/// Converts string to boolean value.
	///
	/// @param[in] p_sStr input string
	/// @return output boolean
	////////////////////////////////////////////////////////////////////////////////
	bool StringToBool(mmString p_sStr);

	////////////////////////////////////////////////////////////////////////////////
	/// This function returns formatted date/time string.
	///
	/// @return formatted date/time string
	////////////////////////////////////////////////////////////////////////////////
	mmString GetCurrentDateTimeString(void);

	////////////////////////////////////////////////////////////////////////////////
	/// Converts integer value representing seconds into string HH:MM:SS.
	///
	/// @param[in] p_iInput integer value
	/// @return string representation of p_iInput
	////////////////////////////////////////////////////////////////////////////////
	mmString MMIntToHHMMSSString(mmInt p_iInput);

	////////////////////////////////////////////////////////////////////////////////
	/// Converts std::string to mmString format using STL mechanism.
	///
	/// @param[in] p_sInputString single byte string,
	/// @return converted mmString.
	////////////////////////////////////////////////////////////////////////////////
	mmString MMCharStringToMMString(const std::string p_sInputString);

	////////////////////////////////////////////////////////////////////////////////
	/// Converts mmString to std::strng format using STL mechanism.
	///
	/// @param[in] p_sInputString mmString string,
	/// @return converted std::string.
	////////////////////////////////////////////////////////////////////////////////
	std::string MMStringToCharString(const mmString p_sInputString);

	////////////////////////////////////////////////////////////////////////////////
	/// Converts mmString to vector with integer values.
	///
	/// @param[in] p_sInputString mmString string,
	/// @return vector of ints.
	////////////////////////////////////////////////////////////////////////////////
	std::vector<mmInt> MMStringToVectorOfInt(const mmString p_sInputString);

	////////////////////////////////////////////////////////////////////////////////
	/// Converts mmString to vector with sub-string values. Delimiter is space char.
	///
	/// @param[in] p_sInputString mmString string,
	/// @return vector of mmStrings.
	////////////////////////////////////////////////////////////////////////////////
	std::vector<mmString> MMStringToVectorOfMMString(const mmString p_sInputString);

	////////////////////////////////////////////////////////////////////////////////
	/// Uppers input mmString.
	///
	/// @param[in] p_sInputString mmString string,
	/// @return uppered string.
	////////////////////////////////////////////////////////////////////////////////
	mmString MMStringToUpper(const mmString p_sInputString);
	////////////////////////////////////////////////////////////////////////////////
	/// Tokenizes input string.
	///
	/// @param[in] p_sInputString mmString string,
	/// @param[out] v_vcTokens vector of tokens,
	/// @param[inopt] _p_sFromString pointer to function able to convert a string to
	/// the specified output type, not required if the output type is a basic type
	/// that can be converted using STL streams.
	////////////////////////////////////////////////////////////////////////////////
	template< class T >
	void Tokenize( mmString p_tString, wchar_t p_wcDelimiter, std::vector<T> &p_vcTokens, T (*_p_sFromString)( mmString& ) = NULL ) {
		std::wstringstream v_sStream( p_tString );
		std::wstringstream v_sTokenStream;
		wchar_t v_wcTmp;
		T v_sToken;
		std::wstring v_wsToken;
		p_vcTokens.clear();
		while( ! v_sStream.eof() ) {
			v_sStream >> v_wcTmp;
			if( v_wcTmp == p_wcDelimiter || v_sStream.eof() ) {
				if( _p_sFromString != NULL ) {
					v_sTokenStream >> v_wsToken;
					v_sToken = (*_p_sFromString)( v_wsToken );
				} else
					v_sTokenStream >> v_sToken;
				if( ! ! v_sTokenStream )
					p_vcTokens.push_back( v_sToken );
				v_sTokenStream.clear();
			} else
				v_sTokenStream << v_wcTmp;
		}
	}
};

#endif
