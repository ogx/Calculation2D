#include <commands/mmImageUtils.h>


mmCommands::mmImageUtils::mmRename::mmRename(mmImages::mmImageStructureI * const p_psImageStructure) : m_psImageStructure(p_psImageStructure) { 
	m_sNames.push_back(L"rename");

	m_sParams.push_back(mmParam(L"image", mmParam::mmTypeImage, false));
	m_sParams.push_back(mmParam(L"new_name", mmParam::mmTypeString, false));
}

bool mmCommands::mmImageUtils::mmRename::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	mmImages::mmImageI * const v_psImage = m_psImageStructure->FindImage(NULL, p_sParams[0].sValue);

	if(v_psImage == NULL) {
		m_sError = L"image not found";
		return false;
	}

	v_psImage->SetName(p_sParams[1].sValue);

	return true;
}

mmCommands::mmImageUtils::mmResize::mmResize(mmImages::mmImageStructureI * const p_psImageStructure) : m_psImageStructure(p_psImageStructure) { 
	m_sNames.push_back(L"resize");

	m_sParams.push_back(mmParam(L"image", mmParam::mmTypeImage, false));
	m_sParams.push_back(mmParam(L"width", mmParam::mmTypeInt, false));
	m_sParams.push_back(mmParam(L"height", mmParam::mmTypeInt, false));
}

bool mmCommands::mmImageUtils::mmResize::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	mmImages::mmImageI * const v_psImage = m_psImageStructure->FindImage(NULL, p_sParams[0].sValue);

	if(v_psImage == NULL) {
		m_sError = L"image not found";
		return false;
	}

	v_psImage->Resize(mmStringUtilities::StringToMMInt(p_sParams[1].sValue), mmStringUtilities::StringToMMInt(p_sParams[2].sValue));

	return true;
}

mmCommands::mmImageUtils::mmClean::mmClean(mmImages::mmImageStructureI * const p_psImageStructure) : m_psImageStructure(p_psImageStructure) { 
	m_sNames.push_back(L"delete");
	m_sNames.push_back(L"clear");

	m_sParams.push_back(mmParam(L"image", mmParam::mmTypeImage, true));
	m_sParams.push_back(mmParam(L"layer", mmParam::mmTypeLayer, true));
}

bool mmCommands::mmImageUtils::mmClean::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	if(p_sName == L"delete") {
		if(! m_sParams.empty()) {
			mmImages::mmImageI * const v_psImage = m_psImageStructure->FindImage(NULL, p_sParams[0].sValue);

			if(v_psImage == NULL) {
				m_sError = L"image not found";
				return false;
			}

			if(p_sParams.size() == 1) {
				m_psImageStructure->DeleteImage(v_psImage->GetID());
			} else {
				mmImages::mmLayerI * const v_psLayer = v_psImage->FindLayer(NULL, p_sParams[1].sValue);

				if(v_psLayer == NULL) {
					m_sError = L"layer not found";
					return false;
				}

				v_psImage->DeleteLayer(v_psLayer->GetID());
			}
		} else {
			m_sError = L"image not found";
			return false;
		}
	} else if(p_sName == L"clear") {
		std::vector<mmID> v_sImageIDs;
		for(mmImages::mmImageI * v_psImage = m_psImageStructure->FindImage(NULL); NULL != v_psImage; v_psImage = m_psImageStructure->FindImage(v_psImage))
			v_sImageIDs.push_back(v_psImage->GetID());
		for(std::size_t v_iI = 0; v_iI < v_sImageIDs.size(); ++v_iI)
			m_psImageStructure->DeleteImage(v_sImageIDs[v_iI]);
	}

	return true;
}
