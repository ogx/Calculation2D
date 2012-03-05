#include <plugin/mmGenericParam.h>

#include <interfaces/mmInterfaceInitializers.h>
#include <interfaces/mmIImages.h>

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
