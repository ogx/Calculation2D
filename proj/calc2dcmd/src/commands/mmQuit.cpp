#include <commands/mmQuit.h>

#include <Windows.h>

mmCommands::mmQuit::mmQuit(void) { 
	m_sNames.push_back(L"quit");
	m_sNames.push_back(L"exit");
}

bool mmCommands::mmQuit::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	::exit(EXIT_SUCCESS);

	return true;
}
