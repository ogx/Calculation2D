#include "cmLayerTransport.h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmLayerTransportParams =
{
	L"Layer transport test",
	L"{7D70DDE0-C282-414A-9BE2-28ACDA078FE2}",
	L"Adds two given layers and creates an output layer.",
	false,
	{0, 0},
	{0, L"OGX", L"", L""},
	L"cmLayerTransport"
};

mmImages::cmLayerTransport::cmLayerTransport(mmLog::mmLogReceiverI* p_psLogReceiver):
mmCalcMethod(p_psLogReceiver, L"cmLayerTransport test")
{
	// overwrite default parameters
	m_sCMParams = cmLayerTransportParams;

	// members initialization
	m_sInputLayer1 = mmImages::mmLayerI::sID::invalid;
	m_sInputLayer2 = mmImages::mmLayerI::sID::invalid;
	m_sOutputImage = mmID::invalid;
	m_sOutputLayer = mmImages::mmLayerI::sID::invalid;

	// input parameters
	BindInputParam(L"Input layer 1", mmGenericParamI::mmLayerType, m_sInputLayer1); 
	BindInputParam(L"Input layer 2", mmGenericParamI::mmLayerType, m_sInputLayer2);
	BindInputParam(L"Output image", mmGenericParamI::mmImageType, m_sOutputImage);

	// output parameters
	BindOutputParam(L"Output layer", mmGenericParamI::mmLayerType, m_sOutputLayer);
}

inline mmReal TestLayerOperation(mmReal x, mmReal y) { return (x+y)/2.0; }

bool mmImages::cmLayerTransport::Calculate()
{
	mmImageI* v_psImage1 = m_psImageStructure->GetImage(m_sInputLayer1.m_sImageID);
	mmImageI* v_psImage2 = m_psImageStructure->GetImage(m_sInputLayer2.m_sImageID);
	mmImageI* v_psImage3 = m_psImageStructure->GetImage(m_sOutputImage);
	if(!v_psImage1 || !v_psImage2 || !v_psImage3)
		return false;

	mmLayerI* v_psInputLayer1 = v_psImage1->GetLayer(m_sInputLayer1.m_sLayerID);
	mmLayerI* v_psInputLayer2 = v_psImage2->GetLayer(m_sInputLayer2.m_sLayerID);
	if(!v_psInputLayer1 || !v_psInputLayer2)
		return false;

	mmUInt w = v_psInputLayer1->GetWidth();
	mmUInt h = v_psInputLayer1->GetHeight();
	// TODO: check sizes

	std::vector<mmReal> buf1(w*h), buf2(w*h), buf3(w*h);
	mmRect rect(0, 0, w, h);
	if(!v_psInputLayer1->GetPixels(rect, &buf1[0]) ||
	   !v_psInputLayer2->GetPixels(rect, &buf2[0]))
		return false;
	std::transform(buf1.begin(), buf1.end(), buf2.begin(), buf3.begin(), &TestLayerOperation);

	mmString v_sLayerName = v_psInputLayer1->GetName() + L"+" + v_psInputLayer2->GetName();
	mmLayerI* v_psOutputLayer = v_psImage3->CreateLayer(v_sLayerName, v_psInputLayer1->GetDefaultValue());
	if(!v_psOutputLayer || !v_psOutputLayer->SetPixels(rect, &buf3[0]))
		return false;
	m_sOutputLayer = v_psOutputLayer->GetID();

	return true;
}