#include <serialization/mmBasicSerialization.h>

#include <algorithm>

#include <mmStringUtilities.h>

mmString mmSerializer<mmString>::FromString(mmString const & p_sString) {
	return p_sString;
}

mmString mmSerializer<mmString>::ToString(mmString const & p_sValue) {
	return p_sValue;
}

mmInt mmSerializer<mmInt>::FromString(mmString const & p_sString) {
	return mmStringUtilities::StringToMMInt(p_sString);
}

mmString mmSerializer<mmInt>::ToString(mmInt const & p_sValue) {
	return mmStringUtilities::MMIntToString(p_sValue);
}

mmUInt mmSerializer<mmUInt>::FromString(mmString const & p_sString) {
	return mmStringUtilities::StringToMMInt(p_sString);
}

mmString mmSerializer<mmUInt>::ToString(mmUInt const & p_sValue) {
	return mmStringUtilities::MMIntToString(p_sValue);
}

mmReal mmSerializer<mmReal>::FromString(mmString const & p_sString) {
	return mmStringUtilities::StringToMMReal(p_sString);
}

mmString mmSerializer<mmReal>::ToString(mmReal const & p_sValue) {
	return mmStringUtilities::MMRealToString(p_sValue);
}

namespace mmImages {
	wchar_t const * const g_ppcBoolTRUE[] = {L"yes", L"true", L"aye", L"1" };
	wchar_t const * const * const g_ppcBoolTRUEEnd = g_ppcBoolTRUE + sizeof(g_ppcBoolTRUE) / sizeof(*g_ppcBoolTRUE);
	struct EqualStringsCaseInsensitive {
		EqualStringsCaseInsensitive(wchar_t const p_pcS[]) : m_sS(mmStringUtilities::MMStringToLower(p_pcS)) {}
		bool operator ()(wchar_t const p_pcS[]) { return mmStringUtilities::MMStringToLower(p_pcS) == m_sS; }
	private:
		mmString const m_sS;
	};
};

bool mmSerializer<bool>::FromString(mmString const & p_sString) {
	wchar_t const * const * const v_ppcBoolTRUE = std::find_if(mmImages::g_ppcBoolTRUE, mmImages::g_ppcBoolTRUEEnd, mmImages::EqualStringsCaseInsensitive(p_sString.c_str()));
	return (v_ppcBoolTRUE != mmImages::g_ppcBoolTRUEEnd);
}

mmString mmSerializer<bool>::ToString(bool const & p_sValue) {
	return (p_sValue ? L"yes" : L"no");
}

