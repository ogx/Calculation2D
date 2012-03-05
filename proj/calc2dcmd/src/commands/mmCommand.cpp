#include <commands/mmCommand.h>

std::list<std::wstring> mmCommands::mmCommand::GetNames(void) const {
	return m_sNames;
}

std::vector<mmCommands::mmParam> mmCommands::mmCommand::GetInputParams(void) const {
	return m_sParams;
}

std::wstring mmCommands::mmCommand::GetErrorMessage(void) const {
	return m_sError;
}

std::wstring mmCommands::mmCommand::GetInfo(void) const {
	std::wstring v_sInfo = L"Usage: ";
	for(std::list<std::wstring>::const_iterator v_sName = m_sNames.begin(); v_sName != m_sNames.end(); ++v_sName) {
		if(v_sName != m_sNames.begin())
			v_sInfo += L"/";
		v_sInfo += *v_sName;
	}
	if(! m_sParams.empty()) {
		for(std::size_t v_iI = 0; v_iI < m_sParams.size(); ++v_iI) {
			v_sInfo += L" ";
			if(m_sParams[v_iI].bOptional)
				v_sInfo += L"[";
			switch(m_sParams[v_iI].eType) {
				case mmParam::mmTypeInt: v_sInfo += L"{int}"; break; 
				case mmParam::mmTypeReal: v_sInfo += L"{real}"; break; 
				case mmParam::mmTypeBool: v_sInfo += L"{bool}"; break;
				case mmParam::mmTypeString: v_sInfo += L"{string}"; break;
				case mmParam::mmTypeImage: v_sInfo += L"{image}"; break;
				case mmParam::mmTypeLayer: v_sInfo += L"{layer}"; break;
				case mmParam::mmTypeFile: v_sInfo += L"{filename}"; break;
				default: v_sInfo += L"{unknown}";
			}
			v_sInfo += L" " + m_sParams[v_iI].sName;
			if(m_sParams[v_iI].bOptional)
				v_sInfo += L"]";
		}
	}
	return v_sInfo;
}
