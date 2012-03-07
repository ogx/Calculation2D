#include <plugin/mmGenericParam.h>

#include <interfaces/mmInterfaceInitializers.h>
#include <interfaces/mmIImages.h>

#include <cstdlib>

inline mmImages::mmGenericParamI::mmType mmImages::GetTypeTransition(mmXML::mmXMLDataType const p_eType) {
	switch(p_eType) {
		case mmXML::g_eXMLInt: return mmGenericParamI::mmIntType;
		case mmXML::g_eXMLReal: return mmGenericParamI::mmRealType;
		case mmXML::g_eXMLString: return mmGenericParamI::mmStringType;
		case mmXML::g_eXMLBool: return mmGenericParamI::mmBoolType;
		case mmXML::g_eXMLImageName: return mmGenericParamI::mmImageNameType;
		case mmXML::g_eXMLDataLayerName: return mmGenericParamI::mmLayerNameType;
		default: return mmGenericParamI::mmUnknownType;
	}
}

inline mmXML::mmXMLDataType mmImages::GetTypeTransition(mmGenericParamI::mmType const p_eType) {
	switch(p_eType) {
	case mmGenericParamI::mmIntType: return mmXML::g_eXMLInt;
	case mmGenericParamI::mmRealType: return mmXML::g_eXMLReal;
	case mmGenericParamI::mmStringType: return mmXML::g_eXMLString;
	case mmGenericParamI::mmBoolType: return mmXML::g_eXMLBool;
	case mmGenericParamI::mmImageNameType: return mmXML::g_eXMLImageName;
	case mmGenericParamI::mmLayerNameType: return mmXML::g_eXMLDataLayerName;
	default: return mmXML::g_eXMLUnknownDataType;
	}
}

template<>
inline mmString mmImages::FromString<mmString>(mmString const & p_sString) {
	return p_sString;
}

template<>
inline mmInt mmImages::FromString<mmInt>(mmString const & p_sString) {
	return mmStringUtilities::StringToMMInt(p_sString);
}

template<>
inline mmReal mmImages::FromString<mmReal>(mmString const & p_sString) {
	return mmStringUtilities::StringToMMReal(p_sString);
}

template<>
inline bool mmImages::FromString<bool>(mmString const & p_sString) {
	return p_sString.compare(g_pAutoCalcXML_Params_ParamType_BoolValue_YES) == 0;
}

template<>
inline mmRect mmImages::FromString<mmRect>(mmString const & p_sString) {
	mmRect v_sRect;
	::swscanf_s(p_sString.c_str(), L"%d|%d|%d|%d", &v_sRect.iLeft, &v_sRect.iTop, &v_sRect.iWidth, &v_sRect.iHeight);
	return v_sRect;
}

template<>
inline mmMath::sPoint2D mmImages::FromString<mmMath::sPoint2D>(mmString const & p_sString) {
	mmMath::sPoint2D v_sPoint = {0.0, 0.0};
	::swscanf_s(p_sString.c_str(), L"%lf|%lf", &v_sPoint.rX, &v_sPoint.rY);
	return v_sPoint;
}

namespace mmImages {
	//wchar_t const * const g_ppcTypeToString[] = {L"int", L"real", L"bool", L"string", L"image", L"image_name", L"layer", L"layer_name", L"rect", L"point"};
	// to be removed with mmXMLIOUtilities
	wchar_t const * const g_ppcTypeToString[] = {
		g_pAutoCalcXML_Params_ParamType_IntegerValue, 
		g_pAutoCalcXML_Params_ParamType_RealValue, 
		g_pAutoCalcXML_Params_ParamType_BoolValue, 
		g_pAutoCalcXML_Params_ParamType_String, 
		L"image", 
		g_pAutoCalcXML_Params_ParamType_ImageName, 
		L"layer", 
		g_pAutoCalcXML_Params_ParamType_DataLayerName,
		L"rect",
		L"point"
	};
	wchar_t const * const * const g_ppcTypeToStringEnd = g_ppcTypeToString + sizeof(g_ppcTypeToString) / sizeof(*g_ppcTypeToString);
	struct EqualStrings {
		EqualStrings(wchar_t const p_pcS[]) : m_pcS(p_pcS) {}
		bool operator ()(wchar_t const p_pcS[]) { return ::wcscmp(p_pcS, m_pcS) == 0; }
	private:
		wchar_t const * const m_pcS;
	};
};

template<>
extern mmImages::mmGenericParamI::mmType mmImages::FromString<mmImages::mmGenericParamI::mmType>(mmString const & p_sString) {
	wchar_t const * const * const v_ppcTypeToString = std::find_if(g_ppcTypeToString, g_ppcTypeToStringEnd, EqualStrings(p_sString.c_str()));
	return (v_ppcTypeToString != g_ppcTypeToStringEnd ? static_cast<mmGenericParamI::mmType>(v_ppcTypeToString - g_ppcTypeToString) : mmGenericParamI::mmUnknownType);
}

template<>
inline mmString mmImages::ToString<mmString>(mmString const & p_sValue) {
	return p_sValue;
}

template<>
inline mmString mmImages::ToString<mmInt>(mmInt const & p_sValue) {
	return mmStringUtilities::MMIntToString(p_sValue);
}

template<>
inline mmString mmImages::ToString<mmReal>(mmReal const & p_sValue) {
	return mmStringUtilities::MMRealToString(p_sValue);
}

template<>
inline mmString mmImages::ToString<bool>(bool const & p_sValue) {
	return p_sValue ? g_pAutoCalcXML_Params_ParamType_BoolValue_YES : g_pAutoCalcXML_Params_ParamType_BoolValue_NO;
}

template<>
inline mmString mmImages::ToString<mmRect>(mmRect const & p_sValue) {
	return mmStringUtilities::MMIntToString(p_sValue.iLeft) + L"|" + mmStringUtilities::MMIntToString(p_sValue.iTop) + L"|" + mmStringUtilities::MMIntToString(p_sValue.iWidth) + L"|" + mmStringUtilities::MMIntToString(p_sValue.iHeight);
}

template<>
inline mmString mmImages::ToString<mmMath::sPoint2D>(mmMath::sPoint2D const & p_sValue) {
	return mmStringUtilities::MMRealToString(p_sValue.rX) + L"|" + mmStringUtilities::MMRealToString(p_sValue.rY);
}

template<>
extern mmString mmImages::ToString<mmImages::mmGenericParamI::mmType>(mmGenericParamI::mmType const & p_sValue) {
	std::size_t i = p_sValue;
	return (p_sValue == mmGenericParamI::mmUnknownType ? L"" : g_ppcTypeToString[static_cast<std::size_t>(p_sValue)]);
}

inline mmXML::mmXMLNodeI* FindOrCreateChild(mmXML::mmXMLNodeI * const p_psParent, mmString const & p_sName) {
	mmXML::mmXMLNodeI * v_psChild;
	if((v_psChild = p_psParent->FindChild(p_sName)) == NULL)
		v_psChild = p_psParent->AddChild(p_sName);
	return v_psChild;
}

inline void mmImages::Serialize(mmGenericParamI const * const p_psParam, mmXML::mmXMLNodeI* const p_psOutputXML) {
	if(p_psParam == NULL || p_psOutputXML == NULL || p_psOutputXML->GetName() != mmImages::g_pAutoCalcXML_Params_Param_Node)
		return;

	FindOrCreateChild(p_psOutputXML, mmImages::g_pAutoCalcXML_Params_ParamName_Node)->SetText(p_psParam->GetNameString());
	FindOrCreateChild(p_psOutputXML, mmImages::g_pAutoCalcXML_Params_ParamType_Node)->SetText(p_psParam->GetTypeString());
	FindOrCreateChild(p_psOutputXML, mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->SetText(p_psParam->GetValueString());
}

inline mmString mmImages::SerializeParameters(mmString const & p_sRootName, std::list<mmGenericParamI*> const & p_sParams) {
	// create XML document which stores automation options
	std::auto_ptr<mmXML::mmXMLDocI> v_psXML(mmInterfaceInitializers::CreateXMLDocument());
	v_psXML->CreateXMLRootNode(p_sRootName);
	for(std::list<mmGenericParamI*>::const_iterator v_sParam = p_sParams.begin(); v_sParam != p_sParams.end(); ++v_sParam)
		Serialize(*v_sParam, v_psXML->GetXMLRootNode()->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node));
	return v_psXML->SaveToXMLBuffer();
}

inline bool mmImages::Deserialize(mmXML::mmXMLNodeI* const p_psInputXML, mmGenericParamI * const p_psParam) {
	if(p_psParam == NULL || p_psInputXML == NULL || p_psInputXML->GetName() != mmImages::g_pAutoCalcXML_Params_Param_Node)
		return false;

	mmXML::mmXMLNodeI * v_psChild = p_psInputXML->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
	if(v_psChild == NULL)
		return false;

	p_psParam->SetValueString(v_psChild->GetText());

	return true;
}

inline void mmImages::DeserializeParameters(mmString const & p_sSerializedParams, std::list<mmGenericParamI*> & p_sParams) {
	std::auto_ptr<mmXML::mmXMLDocI> v_psXML(mmInterfaceInitializers::CreateXMLDocument());
	v_psXML->ParseXMLBuffer(p_sSerializedParams);
	std::vector<mmXML::mmXMLNodeI*> v_sChildNodes = v_psXML->GetXMLRootNode()->GetChildren();
	std::list<mmGenericParamI*>::iterator v_sParam;
	mmXML::mmXMLNodeI* v_psNameNode;
	for(std::size_t v_iI = 0; v_iI < v_sChildNodes.size(); ++v_iI) {
		if((v_psNameNode = v_sChildNodes[v_iI]->FindChild(g_pAutoCalcXML_Params_ParamName_Node)) != NULL && (v_sParam = std::find_if(p_sParams.begin(), p_sParams.end(), mmGenericParamI::FindByName(v_psNameNode->GetText()))) != p_sParams.end())
			Deserialize(v_sChildNodes[v_iI], *v_sParam);
	}
}
