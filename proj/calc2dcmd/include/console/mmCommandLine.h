#ifndef calc2dcmd_mmCommandLine_H_
#define calc2dcmd_mmCommandLine_H_

#include <string>
#include <vector>

namespace mmConsole {
	class mmCommandLine {
	public:
		mmCommandLine(void);
		mmCommandLine(mmCommandLine const & p_sCommandLine);
		mmCommandLine operator = (mmCommandLine const & p_sCommandLine);

		void AddParam(void);
		std::pair<std::wstring, std::size_t> GetParam(void) const;
		std::wstring GetParam(std::size_t const v_iIndex) const;
		bool EraseParam(void);
		bool IsEmpty(void) const;

		void AddToParam(wchar_t const p_cCharacter);
		void SetParam(std::wstring const p_sValue);
		bool EraseFromParam(void);
		bool IsParamEmpty(void) const;

		void DecreaseCursorPos(void);
		void IncreaseCursorPos(void);

		std::wstring Print(wchar_t const p_cSeparator, wchar_t const p_cEscape) const;
		std::vector<std::wstring> GetParams(void) const;
	private:
		struct mmCursor {
			std::vector<std::wstring>::iterator sElement;
			std::wstring::iterator sCharacter;
		};
	private:
		std::vector<std::wstring> m_sElements;
		mmCursor m_sCursor;
	};
};

#endif