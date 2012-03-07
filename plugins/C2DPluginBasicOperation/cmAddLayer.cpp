#include "cmAddLayer.h"

#include <algorithm>

#undef min
#undef max

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmAddLayerParams =
{
	L"Add layer",
	L"{521FEFE6-C9D3-485F-B411-83FCEB407158}",
	L"Adds a new layer when provided with image, name and default value",
	false,
	{0, 0},
	{0, L"John", L"Doe", L"j.doe@example.com"},
	L"cmAddLayer"
};

static const wchar_t* g_UIParam_ImageName = L"Image";
static const wchar_t* g_UIParam_NewLayerName = L"Name";
static const wchar_t* g_UIParam_DefaultValue = L"Value";


mmImages::cmAddLayer::cmAddLayer(mmLog::mmLogReceiverI* p_psLogReceiver):
mmCalcMethod(p_psLogReceiver, L"cmAddLayer")
{
	m_sCMParams = cmAddLayerParams;

	// members initialization
	m_rDefaultValue = -1;
	m_sNewLayerName = L"New layer";

	// input parameters
	SetParam(g_UIParam_ImageName, mmXML::g_eXMLImageName, &m_sImageName); 
	SetParam(g_UIParam_NewLayerName, mmXML::g_eXMLString, &m_sNewLayerName); 
	SetParam(g_UIParam_DefaultValue, mmXML::g_eXMLReal, &m_rDefaultValue); 
	// output parameters
	SetParam(g_UIParam_NewLayerName, mmXML::g_eXMLDataLayerName, &m_sNewLayerName, true); 
	SetParam(g_UIParam_DefaultValue, mmXML::g_eXMLReal, &m_rDefaultValue, true); 
}

bool mmImages::cmAddLayer::Calculate()
{
	mmImageI *v_psImage = m_psImageStructure->FindImage(NULL, m_sImageName);

	if (!v_psImage) return false;

	m_rProgress = 0.0;
	v_psImage->CreateLayer(m_sNewLayerName, m_rDefaultValue);
	m_rProgress = 100.0;

	return true;
}