#ifndef calc2dcmd_mmCommandName_H_
#define calc2dcmd_mmCommandName_H_

#include <algorithm>
#include <string>

namespace mmConsole {
	class mmCommandName {
	public:
		mmCommandName(void) {}
		mmCommandName(std::wstring const & p_sNamespace, std::wstring const & p_sName) : sNamespace(p_sNamespace), sName(p_sName) {}
		mmCommandName(std::wstring const & p_sFullName) { Parse(p_sFullName); }
	public:
		bool operator < (mmCommandName const & p_sCN) const { return (sNamespace != p_sCN.sNamespace) ? sNamespace < p_sCN.sNamespace : sName < p_sCN.sName; }
		bool operator == (mmCommandName const & p_sCN) const { return sNamespace == p_sCN.sNamespace && sName == p_sCN.sName; }
	public:
		void Parse(std::wstring const & p_sFullName) {
			sName = p_sFullName;
			std::wstring::size_type v_sPos;
			if((v_sPos = p_sFullName.find(cSeparator)) != std::wstring::npos) {
				sNamespace = sName.substr(0, v_sPos);
				sName = sName.substr(v_sPos + 1);
			}
		}
	public:
		static wchar_t const cSeparator;
	public:
		std::wstring sNamespace;
		std::wstring sName;
	};

	struct RemoveIfNotPartialMatch {
		RemoveIfNotPartialMatch(std::wstring const & p_sPartialName) : m_sPartialName(p_sPartialName) {}
		bool operator () (std::wstring const & p_sFullName) {
			return p_sFullName.length() < m_sPartialName.length() || p_sFullName.substr(0, m_sPartialName.length()) != m_sPartialName;
		}
	private:
		std::wstring const m_sPartialName;
	};

	struct RemoveIfNotPartialMatchNS {
		RemoveIfNotPartialMatchNS(mmCommandName const & p_sCommandName) : m_sCommandName(p_sCommandName) {}
		bool operator () (mmCommandName const & p_sName) {
			return (m_sCommandName.sNamespace != L"" && 
						(m_sCommandName.sNamespace != p_sName.sNamespace || 
						 p_sName.sName.length() < m_sCommandName.sName.length() || 
						 p_sName.sName.substr(0, m_sCommandName.sName.length()) != m_sCommandName.sName)) ||
				   (m_sCommandName.sNamespace == L"" && 
						(p_sName.sNamespace.length() < m_sCommandName.sName.length() || 
						p_sName.sNamespace.substr(0, m_sCommandName.sName.length()) != m_sCommandName.sName) &&
						(p_sName.sName.length() < m_sCommandName.sName.length() || 
						p_sName.sName.substr(0, m_sCommandName.sName.length()) != m_sCommandName.sName));
		}
	private:
		mmCommandName const m_sCommandName;
	};

	inline bool SortCaseInsensitive(mmCommandName p_sStr1, mmCommandName p_sStr2) {
		std::transform(p_sStr1.sName.begin(), p_sStr1.sName.end(), p_sStr1.sName.begin(), towlower);
		std::transform(p_sStr1.sNamespace.begin(), p_sStr1.sNamespace.end(), p_sStr1.sNamespace.begin(), towlower);
		std::transform(p_sStr2.sName.begin(), p_sStr2.sName.end(), p_sStr2.sName.begin(), towlower);
		std::transform(p_sStr2.sNamespace.begin(), p_sStr2.sNamespace.end(), p_sStr2.sNamespace.begin(), towlower);

		if(p_sStr1.sNamespace != p_sStr2.sNamespace)
			return p_sStr1.sNamespace < p_sStr2.sNamespace;
		else
			return p_sStr1.sName < p_sStr2.sName;
	}

	template<class command_t>
	inline mmCommandName CommandToName(std::pair<mmCommandName, command_t> const & p_sCommand) {
		return p_sCommand.first;
	}

	inline std::wstring CommandToString(mmCommandName const & p_sCommand) {
		return p_sCommand.sNamespace.empty() ? p_sCommand.sName : (p_sCommand.sNamespace + mmCommandName::cSeparator + p_sCommand.sName);
	}
};

#endif