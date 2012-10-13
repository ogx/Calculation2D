#ifndef calc2dcmd_mmClient_H_
#define calc2dcmd_mmClient_H_

#include <map>
#include <string>
#include <vector>

#include <commands/mmCommand.h>

#include <console/mmCommandName.h>
#include <console/mmCommandLine.h>

#include <interfaces/mmInterfaceInitializers.h>
#include <images/mmImagesCalculationManagement.h>
#include <images/mmImagesStructure.h>

namespace mmConsole {
	class mmClient {
	public:
		mmClient(std::wstring const & p_sTitle);
		~mmClient(void);
		mmInt Run(void);
	private:
		bool ValidateParams(std::vector<std::wstring> const & p_sReceivedParams, std::vector<mmCommands::mmParam> & p_sExpectedParams);
		void AddCommand(mmCommands::mmCommand * const p_psCommand, std::wstring const & p_sNamespace=L"");
		mmCommands::mmCommand * FindCommand(std::wstring const & p_sFullName);
		bool Aggregate(wchar_t const p_cCurrent);
		bool AutoComplete(void);
		void DisplayPrompt(void);
		void EraseToPrompt(void);
		static bool IsAllowedCharacter(wchar_t const p_cWhat);
		static void Write(std::wstring const & p_sWhat);
		static void WriteLn(std::wstring const & p_sWhat);
		static void Write(wchar_t const p_cWhat);
		static void NewLine(void);
	private:
		static wchar_t const cEscape;
		static wchar_t const cEndOfWord;
		static wchar_t const cEndOfLine;
		static wchar_t const cAutoComplete;
		static wchar_t const cBackSpace;
		static wchar_t const cNavTrigger1;
		static wchar_t const cNavTrigger2;
		static wchar_t const cNavUp;
		static wchar_t const cNavDown;
		static wchar_t const cNavLeft;
		static wchar_t const cNavRight;
	private:
		std::wstring m_sPrompt;
		bool m_bEscape;
		std::map<mmCommandName, mmCommands::mmCommand*> m_sCommands;
		std::vector<mmCommandName> m_sCommandNames;
		std::vector<mmCommandLine> m_sHistory;
		std::vector<mmCommandLine>::iterator m_sPositionInHistory;
		bool m_bNavigate;
		mmCommandLine m_sCommandLine;
		mmCommands::mmCommand * m_psCommand;
	private:
		std::auto_ptr<mmImages::mmImageStructure> m_psImageStructure;
		std::auto_ptr<mmImages::mmImagesCalculationMethodContainerI> m_psMethodsContainer;
		std::auto_ptr<mmImages::mmImagesCalculationManagement> m_psCalculationManager;
	};
};

#endif