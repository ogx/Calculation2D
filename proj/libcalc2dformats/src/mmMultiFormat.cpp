#include <formats/mmMultiFormat.h>
#include <mmBitmap.h>
#include <mmPNG.h>
#include <mmImageXML.h>
#include <mmStringUtilities.h>

mmFormats::mmMultiFormat::mmMultiFormat(void) {
	m_sFormats.push_back(new mmBitmap());
	m_sFormats.push_back(new mmPNG());
	m_sFormats.push_back(new mmImageXML());

	for(std::size_t v_iI = 0; v_iI < m_sFormats.size(); ++v_iI) {
		std::list<mmString> v_sExtensions = m_sFormats[v_iI]->GetSupportedExensions();
		for(std::list<mmString>::iterator v_sE = v_sExtensions.begin(); v_sE != v_sExtensions.end(); ++v_sE) {
			m_sExtensions[*v_sE] = m_sFormats[v_iI];
			m_sSupportedExtensions.push_back(*v_sE);
		}
	}
}

bool mmFormats::mmMultiFormat::Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName) {
	
	mmString const v_sExtension = mmStringUtilities::MMStringToLower(p_sFileName.substr(p_sFileName.find_last_of(L'.') + 1));

	std::map<mmString, mmFormats::mmFormatI*>::iterator v_sFormat;
	if((v_sFormat = m_sExtensions.find(v_sExtension)) == m_sExtensions.end()) {
		return false;
	}

	if(! v_sFormat->second->Read(p_sFileName, p_psImageStructure, p_sName)) {
		return false;
	}

	return true;
}

bool mmFormats::mmMultiFormat::Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage) {
	
	mmString const v_sExtension = mmStringUtilities::MMStringToLower(p_sFileName.substr(p_sFileName.find_last_of(L'.') + 1));

	std::map<mmString, mmFormats::mmFormatI*>::iterator v_sFormat;
	if((v_sFormat = m_sExtensions.find(v_sExtension)) == m_sExtensions.end()) {
		return false;
	}

	if(! v_sFormat->second->Write(p_sFileName, p_psImage)) {
		return false;
	}

	return true;
}
