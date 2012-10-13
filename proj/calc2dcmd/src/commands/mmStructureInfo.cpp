#include <commands/mmStructureInfo.h>

#include <serialization\mmSerializeID.h>

mmCommands::mmStructureInfo::mmStructureInfo(mmImages::mmImageStructureI * const p_psImageStructure) : m_psImageStructure(p_psImageStructure) {
	m_sNames.push_back(L"images");

	m_sParams.push_back(mmParam(L"image", mmParam::mmTypeImage, true));
}

bool mmCommands::mmStructureInfo::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	std::wstring const v_sName = p_sParams.empty() ? mmString(L"") : p_sParams[0].sValue;

	if(m_psImageStructure->GetImageCount() == 0)
		::_cputws(L"No images available\r\n");
	else
		for(mmImages::mmImageI* v_psImage = m_psImageStructure->FindImage(NULL, v_sName); NULL != v_psImage; v_psImage = m_psImageStructure->FindImage(v_psImage, v_sName)) {
			DisplayImageInfo(v_psImage);			
			::_cputws(L"\r\n");
		}

	return true;
}

void mmCommands::mmStructureInfo::DisplayImageInfo(mmImages::mmImageI * const p_psImage) {
	::_cwprintf(L"ID: %d\r\n", ToString(p_psImage->GetID()).c_str());
	::_cwprintf(L"Name: %s\r\n", p_psImage->GetName().c_str());
	::_cwprintf(L"Size: %dx%d\r\n", p_psImage->GetWidth(), p_psImage->GetHeight());
	::_cwprintf(L"Channels (%d):\r\n", static_cast<mmInt>(p_psImage->GetPixelType()));
	for(mmUInt v_iI = 0; v_iI < static_cast<mmUInt>(p_psImage->GetPixelType()); ++v_iI) {
		mmImages::mmLayerI * v_psLayer = p_psImage->GetChannel(v_iI);
		mmImages::mmStats v_sStats = v_psLayer->GetStats();
		::_cwprintf(L" - %s, min=%.1lf, max=%.1lf, avg=%.1lf, stddev=%.1lf\r\n", v_psLayer->GetName().c_str(), v_sStats.rMin, v_sStats.rMax, v_sStats.rAverage, v_sStats.rStdDev);
	}
		
	if(p_psImage->GetLayerCount() != 0) {
		::_cwprintf(L"Layers (%d):\r\n", p_psImage->GetLayerCount());
		for(mmImages::mmLayerI* v_psLayer = p_psImage->FindLayer(); NULL != v_psLayer; v_psLayer = p_psImage->FindLayer(v_psLayer)) {
			mmImages::mmStats v_sStats = v_psLayer->GetStats();
			::_cwprintf(L" - %s, min=%.1lf, max=%.1lf, avg=%.1lf, stddev=%.1lf\r\n", v_psLayer->GetName().c_str(), v_sStats.rMin, v_sStats.rMax, v_sStats.rAverage, v_sStats.rStdDev);
		}
	}
}