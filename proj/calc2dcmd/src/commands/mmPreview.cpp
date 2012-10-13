#include <commands/mmPreview.h>

mmCommands::mmPreview::mmPreview(mmImages::mmImageStructureI * const p_psImageStructure) : m_psImageStructure(p_psImageStructure), m_psImagePreview(new mmImages::mmImagePreviewOGL(L"Preview")) {
	m_sNames.push_back(L"show");
	m_sNames.push_back(L"hide");

	m_sParams.push_back(mmParam(L"image", mmParam::mmTypeImage, true));
	m_sParams.push_back(mmParam(L"layer", mmParam::mmTypeLayer, true));
	m_sParams.push_back(mmParam(L"colormap", mmParam::mmTypeString, true));
}

bool mmCommands::mmPreview::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	if(p_sName == L"show") {
		if(m_psImageStructure->GetImageCount() != 0) {
			if(! p_sParams.empty()) {
				mmImages::mmImageI * v_psImage = m_psImageStructure->FindImage(NULL, p_sParams[0].sValue);
				if(v_psImage != NULL) {
					if(p_sParams.size() == 1) {
						m_psImagePreview->DisplayImage(v_psImage);
						m_psImagePreview->ShowPreviewWindow();
					} else {
						mmImages::mmLayerI * v_psLayer = v_psImage->FindLayer(NULL, p_sParams[1].sValue);
						if(v_psLayer != NULL) {
							m_psImagePreview->DisplayDataLayer(v_psLayer);
							m_psImagePreview->ShowPreviewWindow();
						} else {
							m_sError = L"layer does not exist";
							return false;
						}
					}
				} else {
					m_sError = L"image does not exist";
					return false;
				}
			} else {
				m_psImagePreview->ShowPreviewWindow();
			}
		} else {
			m_sError = L"no images";
			return false;
		}
	} else {
		m_psImagePreview->HidePreviewWindow();
	}

	return true;
}
