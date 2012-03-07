#include "cmAddLayer.h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmAddLayerParams =
{
	L"Add layer",
	L"{521FEFE6-C9D3-485F-B411-83FCEB407158}",
	L"Adds a new layer when provided with image, name and default value.",
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
	BindInputParam(g_UIParam_ImageName, mmGenericParamI::mmImageNameType, m_sImageName); 
	BindInputParam(g_UIParam_NewLayerName, mmGenericParamI::mmStringType, m_sNewLayerName); 
	BindInputParam(g_UIParam_DefaultValue, mmGenericParamI::mmRealType, m_rDefaultValue); 
	// output parameters
	BindOutputParam(g_UIParam_NewLayerName, mmGenericParamI::mmLayerNameType, m_sNewLayerName); 
	BindOutputParam(g_UIParam_DefaultValue, mmGenericParamI::mmRealType, m_rDefaultValue); 
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