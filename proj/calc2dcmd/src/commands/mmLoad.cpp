#include <commands/mmLoad.h>

#include <formats/mmBitmap.h>
#include <formats/mmPNG.h>
#include <formats/mmImageXML.h>

#include <mmStringUtilities.h>

mmCommands::mmLoad::mmLoad(mmImages::mmImageStructureI * const p_psImageStructure) : m_psImageStructure(p_psImageStructure) {
	m_sNames.push_back(L"load");

	m_sParams.push_back(mmParam(L"filename", mmParam::mmTypeFile, false));
	m_sParams.push_back(mmParam(L"name", mmParam::mmTypeString, true));

	m_sFormats.push_back(new mmFormats::mmBitmap);
	m_sFormats.push_back(new mmFormats::mmPNG);
	m_sFormats.push_back(new mmFormats::mmImageXML);

	for(std::size_t v_iI = 0; v_iI < m_sFormats.size(); ++v_iI) {
		std::list<mmString> v_sExtensions = m_sFormats[v_iI]->GetSupportedExensions();
		for(std::list<mmString>::iterator v_sE = v_sExtensions.begin(); v_sE != v_sExtensions.end(); ++v_sE)
			m_sExtensions[*v_sE] = m_sFormats[v_iI];
	}
}

bool mmCommands::mmLoad::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	mmString const v_sExtension = mmStringUtilities::MMStringToLower(p_sParams[0].sValue.substr(p_sParams[0].sValue.find_last_of(L'.') + 1));
	mmString const v_sName = (p_sParams.size() > 1 ? p_sParams[1].sValue : p_sParams[0].sValue.substr(p_sParams[0].sValue.find_last_of(L'\\') + 1));

	std::map<mmString, mmFormats::mmFormatI*>::iterator v_sFormat;
	if((v_sFormat = m_sExtensions.find(v_sExtension)) == m_sExtensions.end()) {
		m_sError = L"file extension not supported";
		return false;
	}

	if(! v_sFormat->second->Read(p_sParams[0].sValue, m_psImageStructure, v_sName)) {
		m_sError = L"reading file failed";
		return false;
	}

	return true;
}

mmCommands::mmSave::mmSave(mmImages::mmImageStructureI * const p_psImageStructure) : m_psImageStructure(p_psImageStructure) {
	m_sNames.push_back(L"save");

	m_sParams.push_back(mmParam(L"image", mmParam::mmTypeImage, false));
	m_sParams.push_back(mmParam(L"filename", mmParam::mmTypeFile, false));

	m_sFormats.push_back(new mmFormats::mmBitmap);
	m_sFormats.push_back(new mmFormats::mmPNG);
	m_sFormats.push_back(new mmFormats::mmImageXML);

	for(std::size_t v_iI = 0; v_iI < m_sFormats.size(); ++v_iI) {
		std::list<mmString> v_sExtensions = m_sFormats[v_iI]->GetSupportedExensions();
		for(std::list<mmString>::iterator v_sE = v_sExtensions.begin(); v_sE != v_sExtensions.end(); ++v_sE)
			m_sExtensions[*v_sE] = m_sFormats[v_iI];
	}
}

bool mmCommands::mmSave::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	mmImages::mmImageI const * const v_psImage = m_psImageStructure->FindImage(NULL, p_sParams[0].sValue);
	if(v_psImage == NULL) {
		m_sError = L"image not found";
		return false;
	}
	mmString const v_sExtension = mmStringUtilities::MMStringToLower(p_sParams[1].sValue.substr(p_sParams[1].sValue.find_last_of(L'.') + 1));

	std::map<mmString, mmFormats::mmFormatI*>::iterator v_sFormat;
	if((v_sFormat = m_sExtensions.find(v_sExtension)) == m_sExtensions.end()) {
		m_sError = L"file extension not supported";
		return false;
	}

	if(! v_sFormat->second->Write(p_sParams[1].sValue, v_psImage)) {
		m_sError = L"writing file failed";
		return false;
	}

	return true;
}
