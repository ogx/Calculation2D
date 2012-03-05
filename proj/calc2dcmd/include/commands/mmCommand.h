#ifndef calc2dcmd_mmCommand_H_
#define calc2dcmd_mmCommand_H_

#include <commands/mmParam.h>

namespace mmCommands {
	class mmCommand {
	public:
		std::list<std::wstring> GetNames(void) const;
		std::vector<mmParam> GetInputParams(void) const;
		std::wstring GetErrorMessage(void) const;
		std::wstring GetInfo(void) const;
		virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) = 0;
		virtual ~mmCommand(void) {}
	protected:
		std::list<std::wstring> m_sNames;
		std::vector<mmParam> m_sParams;
		std::wstring m_sError;
	};

};

#endif