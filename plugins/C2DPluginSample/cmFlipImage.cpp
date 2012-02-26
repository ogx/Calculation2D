#include "cmFlipImage.h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmFlipImageParams =
{
	L"Flip Image (example)",
	L"{BA728F54-6D6B-4478-AFAC-C4367BDB494F}",
	L"flips selected image horizontally and/or vertically to a new image",
	false,
	{0, 0}
};

static const wchar_t* g_UIParam_ImageName = L"Image:";
static const wchar_t* g_UIParam_Horizontal = L"Horizontally?";
static const wchar_t* g_UIParam_Vertical = L"Vertically?";

mmImages::cmFlipImage::cmFlipImage(mmLog::mmLogReceiverI* p_psLogReceiver):
                                   mmCalcMethod(p_psLogReceiver, 
                                                L"cmFlipImage")

{
	m_sCMParams = cmFlipImageParams;

	// members initialization
	m_bHorizontal = true;
	m_bVertical = true;

	// parameters definition
	SetParam(g_UIParam_ImageName,mmXML::g_eXMLImageName, &m_sImageName); 
	SetParam(g_UIParam_Horizontal,mmXML::g_eXMLBool, &m_bHorizontal); 
	SetParam(g_UIParam_Vertical,mmXML::g_eXMLBool, &m_bVertical); 

	UpdateParameters();
}

void mmImages::cmFlipImage::RetrieveParameters()
{
	GetParam(g_UIParam_ImageName, &m_sImageName); 
	GetParam(g_UIParam_Horizontal, &m_bHorizontal); 
	GetParam(g_UIParam_Vertical, &m_bVertical); 
}

bool mmImages::cmFlipImage::Calculate()
{

	mmImageI *v_psImage, *v_psNewImage;

	v_psImage = m_psImageStructure->GetImage(m_sImageName);

	if (!v_psImage) return false;

	const mmUInt v_iWidth = v_psImage->GetWidth();
	const mmUInt v_iHeight = v_psImage->GetHeight();

	const mmImageI::mmPixelType v_iPixelType = v_psImage->GetPixelType();

	// append suffixes to image name
	mmString m_sNewImageName = m_sImageName + L"_flipped";

	if (m_bHorizontal) m_sNewImageName += L"_H";
	if (m_bVertical) m_sNewImageName += L"_V";

	mmUInt v_iNewImageID = m_psImageStructure->CreateImage(m_sNewImageName,
																												 v_iWidth,
																												 v_iHeight,
																												 v_iPixelType);

	v_psNewImage = m_psImageStructure->GetImage(v_iNewImageID);


	const mmInt v_iPixelCount = v_iWidth*v_iHeight;
	const mmRect v_sUselessRect = {0, 0, v_iWidth, v_iHeight};

	// create pixel array for reading
	mmReal *v_prPixels = new mmReal[v_iPixelCount];

	// create pixel array for writing
	mmReal *v_prNewPixels = new mmReal[v_iPixelCount];

	const mmUInt v_iChannels = v_iPixelType;

	// loop over all channels
	for (mmUInt v_iChannel = 0; v_iChannel < v_iChannels; ++v_iChannel) {

		mmLayerI *v_psChannel = v_psImage->GetChannel(v_iChannel);

		// read channel
		v_psChannel->GetPixels(v_sUselessRect, v_prPixels);

		// loop over all pixels
		for (mmUInt j = 0; j < v_iHeight; j++) {
			for (mmUInt i = 0; i < v_iWidth; i++) {

				mmUInt v_iPixelID = j*v_iWidth + i;
				mmUInt v_iNewPixelID = (m_bHorizontal ? v_iWidth - 1 - i : i) + v_iWidth*(m_bVertical ? v_iHeight - 1 - j : j);

				v_prNewPixels[v_iNewPixelID] = v_prPixels[v_iPixelID];

			}
		}

		mmLayerI *v_psNewChannel = v_psNewImage->GetChannel(v_iChannel);

		// write channel
		v_psNewChannel->SetPixels(v_sUselessRect, v_prNewPixels);
	}

	const mmUInt v_iDataLayerCount = v_psImage->GetLayerCount();

	// loop over all additional data layers
	for (mmUInt v_iDataLayer = 0; v_iDataLayer < v_iDataLayerCount; ++v_iDataLayer) {

		// read data layer
		mmLayerI *v_psLayer = v_psImage->GetChannel(v_iDataLayer);
		v_psLayer->GetPixels(v_sUselessRect, v_prPixels);

		// loop over all pixels
		for (mmUInt j = 0; j < v_iHeight; j++) {
			for (mmUInt i = 0; i < v_iWidth; i++) {

				mmUInt v_iPixelID = j*v_iWidth + i;
				mmUInt v_iNewPixelID = (m_bHorizontal ? v_iWidth - 1 - i : i) + v_iWidth*(m_bVertical ? v_iHeight - 1 - j : j);

				v_prNewPixels[v_iNewPixelID] = v_prPixels[v_iPixelID];

			}
		}

		mmUInt v_psNewLayerID = v_psNewImage->CreateLayer(v_psLayer->GetName(), v_psLayer->GetDefaultValue());
		mmLayerI *v_psNewLayer = v_psNewImage->GetLayer(v_psNewLayerID);

		// write data layer
		v_psNewLayer->SetPixels(v_sUselessRect, v_prNewPixels);

	}

	delete [] v_prPixels, v_prNewPixels;

	return true;
}