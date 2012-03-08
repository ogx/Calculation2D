#include <serialization/mmBasicSerialization.h>

#include <mmStringUtilities.h>
#include <interfaces/mmIImages.h>

mmString mmSerializer<mmString>::FromString(mmString const & p_sString) {
	return p_sString;
}

mmString mmSerializer<mmString>::ToString(mmString const & p_sValue) {
	return p_sValue;
}

mmInt mmSerializer<mmInt>::FromString(mmString const & p_sString) {
	return mmStringUtilities::StringToMMInt(p_sString);
}

mmString mmSerializer<mmInt>::ToString(mmInt const & p_sValue) {
	return mmStringUtilities::MMIntToString(p_sValue);
}

mmReal mmSerializer<mmReal>::FromString(mmString const & p_sString) {
	return mmStringUtilities::StringToMMReal(p_sString);
}

mmString mmSerializer<mmReal>::ToString(mmReal const & p_sValue) {
	return mmStringUtilities::MMRealToString(p_sValue);
}

bool mmSerializer<bool>::FromString(mmString const & p_sString) {
	return p_sString.compare(mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES) == 0;
}

mmString mmSerializer<bool>::ToString(bool const & p_sValue) {
	return (p_sValue ? mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES : mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_NO);
}

