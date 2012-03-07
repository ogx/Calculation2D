#include <console/mmClient.h>

#include <conio.h>

#include <iterator>

#include <commands/mmExternalCommand.h>
#include <commands/mmImageUtils.h>
#include <commands/mmLoad.h>
#include <commands/mmPreview.h>
#include <commands/mmQuit.h>
#include <commands/mmStructureInfo.h>

#include <mmStringUtilities.h>

wchar_t const mmConsole::mmClient::cEndOfWord = L' ';
wchar_t const mmConsole::mmClient::cEndOfLine = L'\r';
wchar_t const mmConsole::mmClient::cAutoComplete = L'\t';
wchar_t const mmConsole::mmClient::cBackSpace = 8;
wchar_t const mmConsole::mmClient::cNavTrigger1 = 0;
wchar_t const mmConsole::mmClient::cNavTrigger2 = 0xE0;
wchar_t const mmConsole::mmClient::cNavUp = L'H';
wchar_t const mmConsole::mmClient::cNavLeft = L'K';
wchar_t const mmConsole::mmClient::cNavRight = L'M';
wchar_t const mmConsole::mmClient::cNavDown = L'P';
wchar_t const mmConsole::mmClient::cEscape = L'\\';

mmConsole::mmClient::mmClient(std::wstring const & p_sTitle) 
	: m_bEscape(false),
		m_bNavigate(false),
		m_sPositionInHistory(m_sHistory.begin()),
		m_psCommand(NULL),
		m_sPrompt(L">> "), 
		m_psImageStructure(new mmImages::mmImageStructure(NULL)), 
		m_psMethodsContainer(mmInterfaceInitializers::CreateDefaultImagesCalculationMethodContainer(NULL)),
		m_psCalculationManager(new mmImages::mmImagesCalculationManagement(3, NULL))
{
	::SetConsoleTitleW(p_sTitle.c_str());

	AddCommand(new mmCommands::mmQuit());
	AddCommand(new mmCommands::mmLoad(m_psImageStructure.get()));
	AddCommand(new mmCommands::mmPreview(m_psImageStructure.get()));
	AddCommand(new mmCommands::mmStructureInfo(m_psImageStructure.get()));
	AddCommand(new mmCommands::mmImageUtils::mmRename(m_psImageStructure.get()));
	AddCommand(new mmCommands::mmImageUtils::mmResize(m_psImageStructure.get()));
	AddCommand(new mmCommands::mmImageUtils::mmClean(m_psImageStructure.get()));
		
	std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> v_sMethods = m_psMethodsContainer->GetAvailableImagesCalculationMethods();
	std::map<std::wstring, std::wstring> v_sMethodFileMapping = m_psMethodsContainer->GetMethodFileMapping();
	for(std::size_t v_iI = 0; v_iI < v_sMethods.size(); ++v_iI) {
		std::wstring v_sNamespace = v_sMethodFileMapping[v_sMethods[v_iI].sIDName];
		v_sNamespace = v_sNamespace.substr(v_sNamespace.find_last_of('\\') + 1);
		v_sNamespace = v_sNamespace.substr(0, v_sNamespace.find_last_of('.'));
		AddCommand(new mmCommands::mmExternalCommand(v_sMethods[v_iI].sIDName, v_sMethods[v_iI].sCommand, v_sMethods[v_iI].sAutoParams.sInParams, m_psMethodsContainer.get(), m_psCalculationManager.get(), m_psImageStructure.get()), v_sNamespace);
	}

	std::transform(m_sCommands.begin(), m_sCommands.end(), std::back_inserter(m_sCommandNames), CommandToName<mmCommands::mmCommand*>);

	std::sort(m_sCommandNames.begin(), m_sCommandNames.end(), SortCaseInsensitive);
}

mmConsole::mmClient::~mmClient(void) {
	for(std::map<mmCommandName, mmCommands::mmCommand*>::iterator v_sCommand = m_sCommands.begin(); v_sCommand != m_sCommands.end(); ++v_sCommand)
		delete v_sCommand->second;
}

mmInt mmConsole::mmClient::Run(void) {
	::_cwprintf_s(	L"\r\n"
					L"                 Welcome to Calculation2D command line!                \r\n\r\n"
					L"    %d calculation methods are available, for a list of commands press TAB\r\n\r\n", 
					m_psMethodsContainer->GetAvailableImagesCalculationMethods().size());

	DisplayPrompt();

	for(wchar_t v_cCurrent = ::_getwch(); Aggregate(v_cCurrent); v_cCurrent = ::_getwch());

	return EXIT_SUCCESS;
}

bool mmConsole::mmClient::ValidateParams(std::vector<std::wstring> const & p_sReceivedParams, std::vector<mmCommands::mmParam> & p_sExpectedParams) {
	if(p_sExpectedParams.size() < p_sReceivedParams.size())
		return false;
		
	std::vector<mmCommands::mmParam> v_sRealParams;
	for(std::size_t v_iI = 0; v_iI < p_sReceivedParams.size(); ++v_iI) {
		mmCommands::mmParam v_sParam(p_sExpectedParams[v_iI]);
		if(mmCommands::TryConvert(v_sParam, p_sReceivedParams[v_iI], m_psImageStructure.get())) {
			v_sParam.sValue = p_sReceivedParams[v_iI];
			v_sRealParams.push_back(v_sParam);
		} else
			return false;
	}

	if(p_sExpectedParams.size() > v_sRealParams.size() && ! p_sExpectedParams[v_sRealParams.size()].bOptional)
		return false;

	p_sExpectedParams = v_sRealParams;

	return true;
}

void mmConsole::mmClient::AddCommand(mmCommands::mmCommand * const p_psCommand, std::wstring const & p_sNamespace) {
	std::list<std::wstring> v_sNames = p_psCommand->GetNames();
	for(std::list<std::wstring>::const_iterator v_sName = v_sNames.begin(); v_sName != v_sNames.end(); ++v_sName)
		m_sCommands[mmCommandName(p_sNamespace, *v_sName)] = p_psCommand;
}

mmCommands::mmCommand * mmConsole::mmClient::FindCommand(std::wstring const & p_sFullName) {
	std::map<mmCommandName, mmCommands::mmCommand*>::iterator v_sMethod = m_sCommands.find(mmCommandName(p_sFullName));
	if(v_sMethod != m_sCommands.end())
		return v_sMethod->second;
	else 
		return NULL;
}

bool mmConsole::mmClient::Aggregate(wchar_t const p_cCurrent) {
	if(p_cCurrent == cNavTrigger1 || p_cCurrent == cNavTrigger2) {
		wchar_t v_cDirection = ::_getwch();
		if(v_cDirection == cNavUp && ! m_sHistory.empty()) {

			EraseToPrompt();
			m_sCommandLine = m_sHistory.back();
			Write(m_sCommandLine.Print(cEndOfWord, cEscape));
		}
	} else if(p_cCurrent == cEndOfWord) {
		m_sCommandLine.AddParam();
		Write(p_cCurrent);
	} else if(p_cCurrent == cEndOfLine) {
		// optionally find method if not found
		std::vector<std::wstring> v_sCommandLineElements = m_sCommandLine.GetParams();
		if(m_psCommand == NULL)
			m_psCommand = FindCommand(v_sCommandLineElements.front());
		// print new line
		NewLine();
		// interpret command
		if(m_psCommand != NULL) {
			std::vector<mmCommands::mmParam> v_sExpectedParams = m_psCommand->GetInputParams();
			std::vector<std::wstring> v_sReceivedParams(v_sCommandLineElements.empty() ? v_sCommandLineElements.begin() : (v_sCommandLineElements.begin() + 1), v_sCommandLineElements.end());
			if(! ValidateParams(v_sReceivedParams, v_sExpectedParams)) {
				WriteLn(m_psCommand->GetInfo());
			} else if(! m_psCommand->Run(v_sCommandLineElements.front(), v_sExpectedParams)) {
				WriteLn(L"error: " + m_psCommand->GetErrorMessage());
			}
		} else {
			WriteLn(v_sCommandLineElements.front() + L": command not found");
		}
		// clear command
		m_psCommand = NULL;
		m_sHistory.push_back(m_sCommandLine);
		m_sCommandLine = mmCommandLine();
		// display prompt
		DisplayPrompt();
	} else if(p_cCurrent == cAutoComplete) {
		// run autocomplete for current context
		if(! AutoComplete()) {
			// display prompt
			DisplayPrompt();
		} 
		// echo all
		Write(m_sCommandLine.Print(cEndOfWord, cEscape));
	} else if(p_cCurrent == cBackSpace) {
		bool v_bSuccess = false;
		if(! m_sCommandLine.IsParamEmpty())
			v_bSuccess = m_sCommandLine.EraseFromParam();
		else
			v_bSuccess = m_sCommandLine.EraseParam();
		if(v_bSuccess) {
			Write(cBackSpace);
			Write(L' ');
			Write(cBackSpace);
		}
	} else if(IsAllowedCharacter(p_cCurrent)) {
		m_sCommandLine.AddToParam(p_cCurrent);
		Write(p_cCurrent);
	}

	return true;
}

bool mmConsole::mmClient::IsAllowedCharacter(wchar_t const p_cWhat) {
	static wchar_t const v_pcAllowedCharacters[] = L"{}-_[]\"':\\.";
	static wchar_t const * const v_pcAllowedCharactersEnd =  v_pcAllowedCharacters + sizeof(v_pcAllowedCharacters) / sizeof(v_pcAllowedCharacters[0]) - 1;
	return 
		(p_cWhat >= L'a' && p_cWhat <= L'z') || 
		(p_cWhat >= L'A' && p_cWhat <= L'Z') || 
		(p_cWhat >= L'0' && p_cWhat <= L'9') || 
		std::find(v_pcAllowedCharacters, v_pcAllowedCharactersEnd, p_cWhat) != v_pcAllowedCharactersEnd;
}

bool mmConsole::mmClient::AutoComplete(void) {
	std::pair<std::wstring, std::size_t> v_sElement = m_sCommandLine.GetParam();
	if(v_sElement.second == 0) {
		std::vector<mmCommandName> v_sGoodNames(m_sCommandNames.begin(), m_sCommandNames.end());
		std::vector<mmCommandName>::iterator v_sGoodNamesEnd = (v_sElement.first.empty() ? v_sGoodNames.end() : std::remove_if(v_sGoodNames.begin(), v_sGoodNames.end(), RemoveIfNotPartialMatchNS(v_sElement.first)));

		if(v_sGoodNamesEnd - v_sGoodNames.begin() == 1) {
			EraseToPrompt();
			m_sCommandLine.SetParam(CommandToString(v_sGoodNames.front()));

			return true;
		} else {
			NewLine();
			for(std::vector<mmCommandName>::iterator v_sName = v_sGoodNames.begin(); v_sName != v_sGoodNamesEnd; ++v_sName)
				WriteLn(CommandToString(*v_sName));

			if(! v_sElement.first.empty()) {
				std::vector<std::wstring> v_sGoodNamesFull;
				std::transform(v_sGoodNames.begin(), v_sGoodNamesEnd, std::back_inserter(v_sGoodNamesFull), CommandToString); 
				std::wstring v_sCommonString = mmStringUtilities::GetCommonSubstring(v_sGoodNamesFull.begin(), v_sGoodNamesFull.end(), v_sElement.first.length());
				if(v_sCommonString.length() > v_sElement.first.size())
					m_sCommandLine.SetParam(v_sCommonString);
			}

			return false;
		}
	} else {
		mmCommands::mmCommand * v_psCommand = FindCommand(m_sCommandLine.GetParam(0));
		if(v_psCommand != NULL) {
			std::vector<mmCommands::mmParam> v_sParams = v_psCommand->GetInputParams();
			if(v_sElement.second - 1 < v_sParams.size()) {
				switch(v_sParams[v_sElement.second - 1].eType) {
					case mmCommands::mmParam::mmTypeImage: {
						if(m_psImageStructure->GetImageCount() == 0) {
							NewLine();
							::_cwprintf_s(L"%s (image): no images available\r\n", v_sParams[v_sElement.second - 1].sName.c_str());
							return false;
						}

						std::vector<std::wstring> v_sImageNames;
						for(mmImages::mmImageI * v_psImage = m_psImageStructure->FindImage(); NULL != v_psImage; v_psImage = m_psImageStructure->FindImage(v_psImage))
							v_sImageNames.push_back(v_psImage->GetName());

						std::vector<std::wstring>::iterator v_sImageNamesEnd = std::remove_if(v_sImageNames.begin(), v_sImageNames.end(), RemoveIfNotPartialMatch(v_sElement.first));
						if(v_sImageNamesEnd - v_sImageNames.begin() == 1) {
							EraseToPrompt();
							m_sCommandLine.SetParam(v_sImageNames.front());

							return true;
						} else {
							NewLine();

							if(v_sImageNamesEnd != v_sImageNames.begin()) {
								for(std::vector<std::wstring>::iterator v_sName = v_sImageNames.begin(); v_sName != v_sImageNamesEnd; ++v_sName)
									WriteLn(*v_sName);

								if(! v_sElement.first.empty()) {
									std::wstring v_sCommonString = GetCommonSubstring(v_sImageNames.begin(), v_sImageNamesEnd, v_sElement.first.length());
									if(v_sCommonString.length() > v_sElement.first.size())
										m_sCommandLine.SetParam(v_sCommonString);
								}
							}

							return false;
						}
					} break;
					case mmCommands::mmParam::mmTypeLayer: {
						NewLine();
						::_cwprintf_s(L"%s (layer)\r\n", v_sParams[v_sElement.second - 1].sName.c_str());
					} break;
					case mmCommands::mmParam::mmTypeFile: {
						NewLine();
						::_cwprintf_s(L"%s (file)\r\n", v_sParams[v_sElement.second - 1].sName.c_str());
					} break;
					case mmCommands::mmParam::mmTypeBool: {
						NewLine();
						::_cwprintf_s(L"%s (bool)\r\n", v_sParams[v_sElement.second - 1].sName.c_str());
					} break;
					case mmCommands::mmParam::mmTypeInt: {
						NewLine();
						::_cwprintf_s(L"%s (int)\r\n", v_sParams[v_sElement.second - 1].sName.c_str());
					} break;
					case mmCommands::mmParam::mmTypeReal: {
						NewLine();
						::_cwprintf_s(L"%s (real)\r\n", v_sParams[v_sElement.second - 1].sName.c_str());
					} break;
					case mmCommands::mmParam::mmTypeString: {
						NewLine();
						::_cwprintf_s(L"%s (string)\r\n", v_sParams[v_sElement.second - 1].sName.c_str());
					} break;
					default: {
						NewLine();
						::_cwprintf_s(L"%s (unknown type)\r\n", v_sParams[v_sElement.second - 1].sName.c_str());
					} break;
				}
			} else {
				NewLine();
				WriteLn(L"too many parameters");
			}
		}
	}

	return false;
}

void mmConsole::mmClient::DisplayPrompt(void) {
	::_cwprintf_s(L"%s", m_sPrompt.c_str());
}

void mmConsole::mmClient::Write(std::wstring const & p_sWhat) {
	::_cputws(p_sWhat.c_str());
}

void mmConsole::mmClient::Write(wchar_t const p_cWhat) {
	::_putwch(p_cWhat);
}

void mmConsole::mmClient::WriteLn(std::wstring const & p_sWhat) {
	::_cwprintf_s(L"%s\r\n", p_sWhat.c_str());
}

void mmConsole::mmClient::NewLine(void) {
	::_cwprintf_s(L"\r\n");
}

void mmConsole::mmClient::EraseToPrompt(void) {
	std::size_t v_iHowMany = m_sCommandLine.Print(cEndOfWord, cEscape).length();
	for(std::size_t v_iI = 0; v_iI < v_iHowMany; ++v_iI)
		Write(cBackSpace);
	for(std::size_t v_iI = 0; v_iI < v_iHowMany; ++v_iI)
		Write(L' ');
	for(std::size_t v_iI = 0; v_iI < v_iHowMany; ++v_iI)
		Write(cBackSpace);
}