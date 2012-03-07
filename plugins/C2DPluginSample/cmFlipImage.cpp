#include "cmFlipImage.h"

#include <algorithm>

#undef min
#undef max

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmFlipImageParams =
{
	L"Flip Image (example)",
	L"{BA728F54-6D6B-4478-AFAC-C4367BDB494F}",
	L"flips selected image horizontally and/or vertically to a new image",
	false,
	{0, 0},
	{0, L"John", L"Doe", L"j.doe@example.com"},
	L"cmFlipImage"
};

static const wchar_t* g_UIParam_ImageName = L"Image";
static const wchar_t* g_UIParam_Horizontal = L"Horizontally?";
static const wchar_t* g_UIParam_Vertical = L"Vertically?";

static const wchar_t* g_UIParam_NewImageName = L"Flipped image";
static const wchar_t* g_UIParam_NewLayerName = L"Pixel difference";

mmImages::cmFlipImage::cmFlipImage(mmLog::mmLogReceiverI* p_psLogReceiver):
mmCalcMethod(p_psLogReceiver, L"cmFlipImage")
{
	m_sCMParams = cmFlipImageParams;

	// members initialization
	m_bHorizontal = true;
	m_bVertical = true;

	// input parameters
	SetParam(g_UIParam_ImageName, mmXML::g_eXMLImageName, &m_sImageName); 
	SetParam(g_UIParam_Horizontal, mmXML::g_eXMLBool, &m_bHorizontal); 
	SetParam(g_UIParam_Vertical, mmXML::g_eXMLBool, &m_bVertical); 
	// output parameters
	SetParam(g_UIParam_NewImageName, mmXML::g_eXMLImageName, &m_sNewImageName, true);
	SetParam(g_UIParam_NewLayerName, mmXML::g_eXMLDataLayerName, &m_sNewLayerName, true);
}

bool mmImages::cmFlipImage::Calculate()
{

	mmImageI *v_psImage, *v_psNewImage;

	v_psImage = m_psImageStructure->FindImage(NULL, m_sImageName);

	if (!v_psImage) return false;

	const mmUInt v_iWidth = v_psImage->GetWidth();
	const mmUInt v_iHeight = v_psImage->GetHeight();

	const mmImageI::mmPixelType v_iPixelType = v_psImage->GetPixelType();

	// append suffixes to image name
	m_sNewImageName = m_sImageName + L"_flipped";
	m_sNewLayerName = L"pixel_difference";

	if (m_bHorizontal) m_sNewImageName += L"_H";
	if (m_bVertical) m_sNewImageName += L"_V";

	v_psNewImage = m_psImageStructure->CreateImage(m_sNewImageName,
																												 v_iWidth,
																												 v_iHeight,
																												 v_iPixelType);
	mmLayerI* v_psNewLayer = v_psNewImage->CreateLayer(m_sNewLayerName, 0.0);

	if (!v_psNewImage) return false;

	const mmInt v_iPixelCount = v_iWidth*v_iHeight;
	const mmRect v_sROI = v_psImage->GetRegionOfInterest();

	// create pixel array for reading
	mmReal *v_prPixels = new mmReal[v_iPixelCount];

	// create pixel array for writing
	mmReal *v_prNewPixels = new mmReal[v_iPixelCount];
	// create array for new layer
	mmReal *v_prLayerValues = new mmReal[v_iPixelCount];
	// initialize array with 0.0
	std::fill(v_prLayerValues, v_prLayerValues + v_iPixelCount, 0.0);

	const mmUInt v_iChannels = v_iPixelType;

	// loop over all channels
	for (mmUInt v_iChannel = 0; v_iChannel < v_iChannels; ++v_iChannel) {

		mmLayerI *v_psChannel = v_psImage->GetChannel(v_iChannel);

		// read channel
		v_psChannel->GetPixels(v_prPixels, v_sROI);

		// loop over all pixels
		for (mmUInt j = 0; j < v_iHeight; j++) {
			for (mmUInt i = 0; i < v_iWidth; i++) {

				mmUInt v_iPixelID = j*v_iWidth + i;
				mmUInt v_iNewPixelID = (m_bHorizontal ? v_iWidth - 1 - i : i) + v_iWidth*(m_bVertical ? v_iHeight - 1 - j : j);

				v_prNewPixels[v_iNewPixelID] = v_prPixels[v_iPixelID];

				v_prLayerValues[v_iPixelID] = std::max(v_prLayerValues[v_iPixelID], ::fabs(v_prPixels[v_iPixelID] - v_prPixels[v_iNewPixelID]));
			}
		}

		mmLayerI *v_psNewChannel = v_psNewImage->GetChannel(v_iChannel);

		// write channel
		v_psNewChannel->SetPixels(v_sROI, v_prNewPixels);
	}

	v_psNewLayer->SetPixels(v_sROI, v_prLayerValues);

	const mmUInt v_iDataLayerCount = v_psImage->GetLayerCount();

	// loop over all additional data layers
	for (mmUInt v_iDataLayer = 0; v_iDataLayer < v_iDataLayerCount; ++v_iDataLayer) {

		// read data layer
		mmLayerI *v_psLayer = v_psImage->GetChannel(v_iDataLayer);
		v_psLayer->GetPixels(v_prPixels, v_sROI);

		// loop over all pixels
		for (mmUInt j = 0; j < v_iHeight; j++) {
			for (mmUInt i = 0; i < v_iWidth; i++) {

				mmUInt v_iPixelID = j*v_iWidth + i;
				mmUInt v_iNewPixelID = (m_bHorizontal ? v_iWidth - 1 - i : i) + v_iWidth*(m_bVertical ? v_iHeight - 1 - j : j);

				v_prNewPixels[v_iNewPixelID] = v_prPixels[v_iPixelID];

			}
		}

		mmLayerI *v_psNewLayer= v_psNewImage->CreateLayer(v_psLayer->GetName(), v_psLayer->GetDefaultValue());

		if(!v_psNewLayer) return false;
		// write data layer
		v_psNewLayer->SetPixels(v_sROI, v_prNewPixels);

	}

	delete [] v_prPixels;
	delete [] v_prNewPixels;

	return true;
}