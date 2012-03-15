#include <commands/mmParam.h>

mmCommands::mmParam::mmType mmCommands::TranslateType(mmImages::mmGenericParamI::mmType const p_eXMLType) {
	switch(p_eXMLType) {
	case mmImages::mmGenericParamI::mmIntType: return mmParam::mmTypeInt;
	case mmImages::mmGenericParamI::mmRealType: return mmParam::mmTypeReal;
	case mmImages::mmGenericParamI::mmStringType: return mmParam::mmTypeString;
	case mmImages::mmGenericParamI::mmBoolType: return mmParam::mmTypeBool;
	case mmImages::mmGenericParamI::mmImageNameType: return mmParam::mmTypeImage;
	case mmImages::mmGenericParamI::mmLayerNameType: return mmParam::mmTypeLayer;
	default: return mmParam::mmTypeUnknown;
	}
}

bool mmCommands::TryConvert(mmParam const & p_sParam, std::wstring const & p_sPossibleValue, mmImages::mmImageStructureI const * const p_psImageStructure = NULL) {
	switch(p_sParam.eType) {
		case mmParam::mmTypeInt: {
			wchar_t * v_pcEnd(NULL);
			mmInt const v_iInt = ::wcstol(p_sPossibleValue.c_str(), &v_pcEnd, 10);
			return p_sPossibleValue != L"" /*&& *v_pcEnd != L'\0'*/ && v_iInt != LONG_MIN && v_iInt != LONG_MAX;
		}
		case mmParam::mmTypeReal: {
			wchar_t * v_pcEnd(NULL);
			mmReal const v_rReal = ::wcstod(p_sPossibleValue.c_str(), &v_pcEnd);
			return  p_sPossibleValue != L"" /*&& *v_pcEnd != L'\0'*/ && v_rReal != HUGE_VAL && v_rReal != -HUGE_VAL;
		}
		case mmParam::mmTypeBool: {
			static std::wstring const v_psPossibleValues[] = { 
				L"yes", L"YES", 
				L"aye", L"AYE",
				L"True", L"TRUE", L"true",
				L"1", 
				L"no", L"NO", 
				L"nay", L"NAY", 
				L"False", L"FALSE", L"false",
				L"0" };
			static std::wstring const * const v_psPossibleValuesEnd = v_psPossibleValues + 16;
			return std::find(v_psPossibleValues, v_psPossibleValuesEnd, p_sPossibleValue) != v_psPossibleValuesEnd;
		}
		case mmParam::mmTypeString:
			return true;
		case mmParam::mmTypeImage:
			return p_psImageStructure != NULL && p_psImageStructure->FindImage(NULL, p_sPossibleValue) != NULL;
		case mmParam::mmTypeLayer:
			return true;
		case mmParam::mmTypeFile:
			return true; //::_waccess(p_sPossibleValue.c_str(), 0) == 0;
		case mmParam::mmTypeUnknown: 
		default: 
			return false;
	}

	return false;
}