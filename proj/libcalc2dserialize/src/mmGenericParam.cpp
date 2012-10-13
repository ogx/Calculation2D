#include <serialization/mmGenericParam.h>

#include <interfaces/mmInterfaceInitializers.h>
#include <interfaces/mmIImages.h>

#include <cstdlib>

namespace mmImages {
	wchar_t const * const g_ppcTypeToString[] = {L"int", L"real", L"bool", L"string", L"image", L"image_name", L"layer", L"layer_name", L"rect", L"point", L"list"};
	wchar_t const * const * const g_ppcTypeToStringEnd = g_ppcTypeToString + sizeof(g_ppcTypeToString) / sizeof(*g_ppcTypeToString);
	struct EqualStrings {
		EqualStrings(wchar_t const p_pcS[]) : m_pcS(p_pcS) {}
		bool operator ()(wchar_t const p_pcS[]) { return ::wcscmp(p_pcS, m_pcS) == 0; }
	private:
		wchar_t const * const m_pcS;
	};
};

mmImages::mmGenericParamI::mmType mmSerializer<mmImages::mmGenericParamI::mmType>::FromString(mmString const & p_sString) {
	wchar_t const * const * const v_ppcTypeToString = std::find_if(mmImages::g_ppcTypeToString, mmImages::g_ppcTypeToStringEnd, mmImages::EqualStrings(p_sString.c_str()));
	return (v_ppcTypeToString != mmImages::g_ppcTypeToStringEnd ? static_cast<mmImages::mmGenericParamI::mmType>(v_ppcTypeToString - mmImages::g_ppcTypeToString) : mmImages::mmGenericParamI::mmUnknownType);
}

mmString mmSerializer<mmImages::mmGenericParamI::mmType>::ToString(mmImages::mmGenericParamI::mmType const & p_sValue) {
	std::size_t i = p_sValue;
	return (p_sValue == mmImages::mmGenericParamI::mmUnknownType ? L"" : mmImages::g_ppcTypeToString[static_cast<std::size_t>(p_sValue)]);
}

inline mmXML::mmXMLNodeI* FindOrCreateChild(mmXML::mmXMLNodeI * const p_psParent, mmString const & p_sName) {
	mmXML::mmXMLNodeI * v_psChild;
	if((v_psChild = p_psParent->FindChild(p_sName)) == NULL)
		v_psChild = p_psParent->AddChild(p_sName);
	return v_psChild;
}

namespace mmImages {
	inline void Serialize(mmGenericParamI const * const p_psParam, mmXML::mmXMLNodeI* const p_psOutputXML) {
		if(p_psParam == NULL || p_psOutputXML == NULL || p_psOutputXML->GetName() != mmImages::g_pAutoCalcXML_Params_Param_Node)
			return;

		FindOrCreateChild(p_psOutputXML, mmImages::g_pAutoCalcXML_Params_ParamName_Node)->SetText(p_psParam->GetNameString());
		FindOrCreateChild(p_psOutputXML, mmImages::g_pAutoCalcXML_Params_ParamType_Node)->SetText(p_psParam->GetTypeString());
		FindOrCreateChild(p_psOutputXML, mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->SetText(p_psParam->GetValueString());
	}
}

inline mmString mmImages::SerializeParameters(mmString const & p_sRootName, std::list<mmGenericParamI*> const & p_sParams) {
	// create XML document which stores automation options
	std::auto_ptr<mmXML::mmXMLDocI> v_psXML(mmInterfaceInitializers::CreateXMLDocument());
	v_psXML->CreateXMLRootNode(p_sRootName);
	for(std::list<mmGenericParamI*>::const_iterator v_sParam = p_sParams.begin(); v_sParam != p_sParams.end(); ++v_sParam)
		Serialize(*v_sParam, v_psXML->GetXMLRootNode()->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node));
	return v_psXML->SaveToXMLBuffer();
}

namespace mmImages {
	inline bool Deserialize(mmXML::mmXMLNodeI* const p_psInputXML, mmGenericParamI * const p_psParam) {
		if(p_psParam == NULL || p_psInputXML == NULL || p_psInputXML->GetName() != mmImages::g_pAutoCalcXML_Params_Param_Node)
			return false;

		mmXML::mmXMLNodeI * v_psChild = p_psInputXML->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
		if(v_psChild == NULL)
			return false;

		p_psParam->SetValueString(v_psChild->GetText());

		return true;
	}
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
