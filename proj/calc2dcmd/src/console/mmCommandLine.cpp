#include <console/mmCommandLine.h>

mmConsole::mmCommandLine::mmCommandLine(void) : m_sElements(1, L"") {
	m_sCursor.sElement = m_sElements.begin();
	m_sCursor.sCharacter = m_sCursor.sElement->begin();
}

mmConsole::mmCommandLine::mmCommandLine(mmCommandLine const & p_sCommandLine) : m_sElements(p_sCommandLine.m_sElements) {
	m_sCursor.sElement = m_sElements.begin() + (p_sCommandLine.m_sCursor.sElement - p_sCommandLine.m_sElements.begin());
	m_sCursor.sCharacter = m_sCursor.sElement->begin() + (p_sCommandLine.m_sCursor.sCharacter - p_sCommandLine.m_sCursor.sElement->begin());
}

mmConsole::mmCommandLine mmConsole::mmCommandLine::operator = (mmCommandLine const & p_sCommandLine) {
	m_sElements = p_sCommandLine.m_sElements;
	m_sCursor.sElement = m_sElements.begin() + (p_sCommandLine.m_sCursor.sElement - p_sCommandLine.m_sElements.begin());
	m_sCursor.sCharacter = m_sCursor.sElement->begin() + (p_sCommandLine.m_sCursor.sCharacter - p_sCommandLine.m_sCursor.sElement->begin());
	return *this;
}

//void mmConsole::mmCommandLine::Clear(void) {
//	m_sElements.clear();
//	m_sCursor.sElement = m_sElements.begin();
//	m_sCursor.sCharacter = m_sCursor.sElement->begin();
//}

void mmConsole::mmCommandLine::AddParam(void) {
	if(m_sCursor.sElement == m_sElements.end())
		m_sElements.push_back(L"");
	else
		m_sCursor.sElement = m_sElements.insert(m_sCursor.sElement + 1, L"");
	m_sCursor.sCharacter = m_sCursor.sElement->begin();
}

void mmConsole::mmCommandLine::AddToParam(wchar_t const p_cCharacter) {
	if(m_sCursor.sCharacter == m_sCursor.sElement->end())
		m_sCursor.sElement->push_back(p_cCharacter);
	else
		m_sCursor.sCharacter = m_sCursor.sElement->insert(m_sCursor.sCharacter + 1, p_cCharacter);
}

void mmConsole::mmCommandLine::SetParam(std::wstring const p_sValue) {
	*m_sCursor.sElement = p_sValue;
	m_sCursor.sCharacter = (p_sValue.empty() ? m_sCursor.sElement->begin() : m_sCursor.sElement->end() - 1);
}

bool mmConsole::mmCommandLine::EraseParam(void) {
	if(m_sElements.size() > 1) {
		m_sCursor.sElement = m_sElements.erase(m_sCursor.sElement);
		if(m_sCursor.sElement != m_sElements.begin())
			--m_sCursor.sElement;
		m_sCursor.sCharacter = (m_sCursor.sElement->empty() ? m_sCursor.sElement->begin() : --m_sCursor.sElement->end());

		return true;
	} else
		return false;
}

bool mmConsole::mmCommandLine::EraseFromParam(void) {
	if(! m_sCursor.sElement->empty()) {
		m_sCursor.sElement->erase(m_sCursor.sCharacter);
		if(m_sCursor.sCharacter != m_sCursor.sElement->begin())
			--m_sCursor.sCharacter;

		return true;
	} else
		return false;
}

bool mmConsole::mmCommandLine::IsParamEmpty(void) const {
	return m_sCursor.sElement->empty();
}

bool mmConsole::mmCommandLine::IsEmpty(void) const {
	return m_sElements.empty();
}

std::pair<std::wstring, std::size_t> mmConsole::mmCommandLine::GetParam(void) const {
	return std::make_pair(*m_sCursor.sElement, m_sCursor.sElement - m_sElements.begin());
}

std::wstring mmConsole::mmCommandLine::GetParam(std::size_t const v_iIndex) const {
	return v_iIndex < m_sElements.size() ? m_sElements[v_iIndex] : std::wstring(L"");
}

std::vector<std::wstring>mmConsole::mmCommandLine:: GetParams(void) const {
	return m_sElements;
}

void mmConsole::mmCommandLine::DecreaseCursorPos(void) {
	if(m_sCursor.sCharacter != m_sCursor.sElement->begin()) {
		--m_sCursor.sCharacter;
	} else if(m_sCursor.sElement != m_sElements.begin()) {
		--m_sCursor.sElement;
		m_sCursor.sCharacter = m_sCursor.sElement->begin();
	}
}

void mmConsole::mmCommandLine::IncreaseCursorPos(void) {
	if((m_sCursor.sCharacter + 1) != m_sCursor.sElement->end()) {
		++m_sCursor.sCharacter;
	} else if((m_sCursor.sElement + 1) != m_sElements.end()) {
		++m_sCursor.sElement;
		m_sCursor.sCharacter = m_sCursor.sElement->begin();
	}
}

std::wstring mmConsole::mmCommandLine::Print(wchar_t const p_cSeparator, wchar_t const p_cEscape) const {
	std::wstring v_sLine;
	for(std::size_t v_iI = 0; v_iI < m_sElements.size(); ++v_iI) {
		v_sLine += m_sElements[v_iI];
		if(v_iI != m_sElements.size() - 1)
			v_sLine += p_cSeparator;
	}
	return v_sLine;
}
