#include "cmFlipImage.h"

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmFlipImageParams =
{
	L"Flip Image (example)",
	L"{BA728F54-6D6B-4478-AFAC-C4367BDB494F}",
	L"Flips selected image horizontally and/or vertically to a new image based on ROI.",
	false,
	{0, 0},
	{0, L"John", L"Doe", L"j.doe@example.com"},
	L"cmFlipImage"
};

static const wchar_t* g_UIParam_ImageName = L"Image";
static const wchar_t* g_UIParam_Horizontal = L"Horizontally?";
static const wchar_t* g_UIParam_Vertical = L"Vertically?";
static const wchar_t* g_UIParam_NewImage = L"Create new image?";

static const wchar_t* g_UIParam_NewImageName = L"Flipped image";

mmImages::cmFlipImage::cmFlipImage(mmLog::mmLogReceiverI* p_psLogReceiver):
mmCalcMethod(p_psLogReceiver, L"cmFlipImage")
{
	m_sCMParams = cmFlipImageParams;

	// members initialization
	m_bHorizontal = true;
	m_bVertical = true;
	m_bNewImage = false;

	// input parameters
	BindInputParam(g_UIParam_ImageName, mmGenericParamI::mmImageNameType, m_sImageName); 
	BindInputParam(g_UIParam_Horizontal, mmGenericParamI::mmBoolType, m_bHorizontal); 
	BindInputParam(g_UIParam_Vertical, mmGenericParamI::mmBoolType, m_bVertical); 
	BindInputParam(g_UIParam_NewImage, mmGenericParamI::mmBoolType, m_bNewImage);
	// output parameters
	BindOutputParam(g_UIParam_NewImageName, mmGenericParamI::mmImageNameType, m_sNewImageName);
}

bool mmImages::cmFlipImage::Calculate()
{

	mmImageI *v_psImage, *v_psNewImage;

	v_psImage = m_psImageStructure->FindImage(NULL, m_sImageName);

	if (!v_psImage) return false;

	const mmUInt v_iWidth = v_psImage->GetWidth();
	const mmUInt v_iHeight = v_psImage->GetHeight();

	const mmImageI::mmPixelType v_iPixelType = v_psImage->GetPixelType();

	if (m_bNewImage) {
		// append suffixes to image name
		m_sNewImageName = m_sImageName + L"_flipped";

		if (m_bHorizontal) m_sNewImageName += L"_H";
		if (m_bVertical) m_sNewImageName += L"_V";

	
		v_psNewImage = m_psImageStructure->CreateImage(m_sNewImageName,
																														v_iWidth,
																														v_iHeight,
																														v_iPixelType);
		if (!v_psNewImage) return false;
	}
	

	const mmRect v_sROI = v_psImage->GetRegionOfInterest();
	const mmInt v_iPixelCount = v_sROI.GetSize();

	// create pixel array for reading
	mmReal *v_prPixels = new mmReal[v_iPixelCount];

	// create pixel array for writing
	mmReal *v_prNewPixels = new mmReal[v_iPixelCount];

	const mmUInt v_iChannels = v_iPixelType;
	const mmReal v_rAllRows = 1.0 * v_iHeight * (v_iChannels + v_psImage->GetLayerCount());
	mmUInt v_iCalculatedRows = 0;

	// loop over all channels
	for (mmUInt v_iChannel = 0; v_iChannel < v_iChannels; ++v_iChannel) {

		mmLayerI *v_psChannel = v_psImage->GetChannel(v_iChannel);

		// read channel
		v_psChannel->GetPixels(v_sROI, v_prPixels);

		// loop over all pixels
		for (mmUInt j = 0; j < v_sROI.iHeight; j++) {
			for (mmUInt i = 0; i < v_sROI.iWidth; i++) {

				mmUInt v_iPixelID = j*v_sROI.iWidth + i;
				mmUInt v_iNewPixelID = (m_bHorizontal ? v_sROI.iWidth - 1 - i : i) + v_sROI.iWidth*(m_bVertical ? v_sROI.iHeight - 1 - j : j);

				v_prNewPixels[v_iNewPixelID] = v_prPixels[v_iPixelID];
			}
			m_rProgress =  100.0 * ++v_iCalculatedRows / v_rAllRows;
		}

		mmLayerI *v_psNewChannel;
		if (m_bNewImage) {
			v_psNewChannel = v_psNewImage->GetChannel(v_iChannel);
		}
		else {
			v_psNewChannel = v_psImage->GetChannel(v_iChannel);
		}

		// write channel
		v_psNewChannel->SetPixels(v_sROI, v_prNewPixels);
	}

	// loop over all additional data layers
	//for (mmLayerI* v_psLayer = v_psImage->FindLayer(); NULL != v_psLayer; v_psLayer = v_psImage->FindLayer(v_psLayer)) {

	//	v_psLayer->GetPixels(v_sROI, v_prPixels);

	//	// loop over all pixels
	//	for (mmUInt j = 0; j < v_sROI.iHeight; j++) {
	//		for (mmUInt i = 0; i < v_sROI.iWidth; i++) {

	//			mmUInt v_iPixelID = j*v_sROI.iWidth + i;
	//			mmUInt v_iNewPixelID = (m_bHorizontal ? v_sROI.iWidth - 1 - i : i) + v_sROI.iWidth*(m_bVertical ? v_sROI.iHeight - 1 - j : j);

	//			v_prNewPixels[v_iNewPixelID] = v_prPixels[v_iPixelID];

	//		}
	//		m_rProgress = 100.0 * ++v_iCalculatedRows / v_rAllRows;
	//	}

	//	mmLayerI *v_psNewLayer= v_psNewImage->CreateLayer(v_psLayer->GetName(), v_psLayer->GetDefaultValue());

	//	if(!v_psNewLayer) return false;
	//	// write data layer
	//	v_psNewLayer->SetPixels(v_sROI, v_prNewPixels);

	//}

	delete [] v_prPixels;
	delete [] v_prNewPixels;

	return true;
}