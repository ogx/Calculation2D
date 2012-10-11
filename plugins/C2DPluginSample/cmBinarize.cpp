#include "cmBinarize.h"
#include <algorithm>

static mmImages::mmImagesCalculationMethodI::sCalculationMethodParams cmBinarizeParams =
{
	L"Binarize (example)",
	L"{6FDB57AD-D60F-417E-859F-4AB10327AF35}",
	L"Binarizes the image using a provided threshold value",
	false,
	{0, 0},
	{0, L"John", L"Doe", L"j.doe@example.com"},
	L"cmBinarize"
};

// param names
static const wchar_t* g_UIParam_ImageName = L"Image";
static const wchar_t* g_UIParam_Threshold = L"Threshold";
static const wchar_t* g_UIParam_LayerName = L"Binarized";

mmImages::cmBinarize::cmBinarize(mmLog::mmLogReceiverI* p_psLogReceiver):
mmCalcMethod(p_psLogReceiver, L"cmBinarize")
{
	// overwrite default parameters
	m_sCMParams = cmBinarizeParams;

	// members initialization
	m_rThreshold = 0.5;

	// input parameters
	BindInputParam(g_UIParam_ImageName, mmGenericParamI::mmImageNameType, m_sImageName); 
	BindInputParam(g_UIParam_Threshold, mmGenericParamI::mmRealType, m_rThreshold);
	// output parameters
	BindOutputParam(g_UIParam_LayerName, mmGenericParamI::mmLayerNameType, m_sLayerName);
}

bool mmImages::cmBinarize::Calculate()
{
	// try to get a pointer to the image with given name
	mmImageI * v_psImage = m_psImageStructure->FindImage(NULL, m_sImageName);
	// if image does not exist, say there's an error
	if (!v_psImage) return false;

	// get image parameters -- region of interest (ROI), pixel count inside ROI and number of channels
	const mmRect v_sROI = v_psImage->GetRegionOfInterest();
	const mmInt v_iPixelCount = v_sROI.GetSize();
	const mmUInt v_iChannels = v_psImage->GetPixelType();

	// create pixel array for reading
	mmReal *v_prPixels = new mmReal[v_iPixelCount];
	// create array for new layer
	mmReal *v_prBinarized = new mmReal[v_iPixelCount];
	// initialize array with 0.0
	std::fill(v_prBinarized, v_prBinarized + v_iPixelCount, 0.0);

	// prepare counters for progress
	const mmReal v_rAllRows = 1.0 * v_sROI.iHeight * v_iChannels;
	mmUInt v_iCalculatedRows = 0;

	// loop over all channels
	for (mmUInt v_iChannel = 0; v_iChannel < v_iChannels; ++v_iChannel) {
		mmLayerI *v_psChannel = v_psImage->GetChannel(v_iChannel);
		// read channel
		v_psChannel->GetPixels(v_sROI, v_prPixels);
		// loop over all pixels
		for (mmUInt j = 0; j < v_sROI.iHeight; j++) {
			for (mmUInt i = 0; i < v_sROI.iWidth; i++) {
				// if any of the channels is over provided threshold
				if(v_prPixels[j * v_sROI.iWidth + i] > m_rThreshold) {
					v_prBinarized[j * v_sROI.iWidth + i] = 1.0;
				}
			}
			// update progress
			m_rProgress =  100.0 * ++v_iCalculatedRows / v_rAllRows;
		}
	}

	// set layer name to `binarized'
	m_sLayerName = L"binarized";
	// create new layer
	mmLayerI* v_psNewLayer = v_psImage->CreateLayer(m_sLayerName, -1.0);
	// if creating the layer failed, clear output parameter and say there's an error
	if (!v_psNewLayer) {
		m_sLayerName = L"";
		return false;
	}
	// put calculated values
	v_psNewLayer->SetPixels(v_sROI, v_prBinarized);

	// remove arrays!
	delete [] v_prPixels;
	delete [] v_prBinarized;

	return true;
}