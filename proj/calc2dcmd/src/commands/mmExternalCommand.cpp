#include <commands/mmExternalCommand.h>

#include <algorithm>

#undef min
#undef max

mmCommands::mmExternalCommand::mmExternalCommand(std::wstring const & p_sID, std::wstring const & p_sName, std::wstring const & p_sInputParams, mmImages::mmImagesCalculationMethodContainerI * const p_psMethodsContainer, mmImages::mmImagesCalculationManagement * const p_psCalculationManager, mmImages::mmImageStructureI * const p_psImageStructure) 
	: m_sID(p_sID), 
	  m_psMethodsContainer(p_psMethodsContainer), 
	  m_psImageStructure(p_psImageStructure), 
	  m_psCalculationManager(p_psCalculationManager) 
{
	m_sNames.push_back(p_sName);

	// create XML for parsing the parameters
	std::auto_ptr<mmXML::mmXMLDocI> v_psXML(mmInterfaceInitializers::CreateXMLDocument(NULL));
	v_psXML->ParseXMLBuffer(p_sInputParams);
	// get child nodes with parameters
	std::vector<mmXML::mmXMLNodeI*> v_sChildNodes = v_psXML->GetXMLRootNode()->GetChildren();
	// translate parameters
	for(std::size_t v_iI = 0; v_iI < v_sChildNodes.size(); ++v_iI ) {
		m_sParams.push_back(mmParam(
			v_sChildNodes[v_iI]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node)->GetText(), 
			TranslateType(mmXML::GetTypeTransition(v_sChildNodes[v_iI]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node)->GetText())), 
			false)
		);
		m_sParams.back().sValue = v_sChildNodes[v_iI]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->GetText();
	}
}

bool mmCommands::mmExternalCommand::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	if(p_sParams.size() != m_sParams.size()) {
		m_sError = L"supplied parameter list does not match signature";
		return false;
	}

	std::auto_ptr<mmImages::mmImagesCalculationMethodI> v_psMethod(m_psMethodsContainer->InitializeImagesCalculationMethod(m_sID));
	if(v_psMethod.get() == NULL) {
		m_sError = L"method initialization failed";
		return false;
	}

	mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams v_sParamsXML = v_psMethod->GetCalculationMethodInfo().sAutoParams;
	// create XML for parsing the parameters
	std::auto_ptr<mmXML::mmXMLDocI> v_psXML(mmInterfaceInitializers::CreateXMLDocument(NULL));
	v_psXML->ParseXMLBuffer(v_sParamsXML.sInParams);
	// get child nodes with parameters
	std::vector<mmXML::mmXMLNodeI*> v_sChildNodes = v_psXML->GetXMLRootNode()->GetChildren();

	if(p_sParams.size() != v_sChildNodes.size()) {
		m_sError = L"supplied parameter list does not match signature";
		return false;
	}

	// translate parameters
	for(std::size_t v_iI = 0; v_iI < v_sChildNodes.size(); ++v_iI )
		v_sChildNodes[v_iI]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->SetText(p_sParams[v_iI].sValue);

	::wcscpy_s(v_sParamsXML.sInParams, v_psXML->SaveToXMLBuffer().c_str());

	m_psCalculationManager->CalculateImages(v_psMethod.get(), m_psImageStructure, &v_sParamsXML);

	unsigned v_iLastStatus = 10, v_iCurrentStatus;
	::_cputws(L"0% ");
	while(m_psCalculationManager->IsCalculating()) {
		v_iCurrentStatus = static_cast<unsigned>(m_psCalculationManager->GetProgress());
		if(v_iCurrentStatus > v_iLastStatus)
			for(; v_iLastStatus < v_iCurrentStatus && v_iLastStatus < 100 && m_psCalculationManager->IsCalculating(); v_iLastStatus += 10)
				::_cwprintf(L"%u%% ", v_iLastStatus);
		::Sleep(10);
	}
	for(; v_iLastStatus <= 100; v_iLastStatus += 10)
		::_cwprintf(L"%u%% ", v_iLastStatus);
	::_cputws(L"\r\n");

	return true;
}