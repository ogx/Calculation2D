#include <formats/mmBitmap.h>

#include <vector>

#include <windows.h>

mmFormats::mmBitmap::mmBitmap(void) {
	m_sSupportedExtensions.push_back(L"bmp");
}

bool mmFormats::mmBitmap::Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName) {
	BITMAPFILEHEADER v_sFileHeader = {};
	BITMAPINFOHEADER v_sInfoHeader = {};
	DWORD v_iReadBytes = 0;

	HANDLE v_sFile = ::CreateFile(p_sFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(v_sFile == INVALID_HANDLE_VALUE)
		return false;

	if(::ReadFile(v_sFile, &v_sFileHeader, sizeof(v_sFileHeader), &v_iReadBytes, NULL) == FALSE || v_iReadBytes != sizeof(v_sFileHeader) || v_sFileHeader.bfType != 0x4D42)
		return false;

	if(::ReadFile(v_sFile, &v_sInfoHeader, sizeof(v_sInfoHeader), &v_iReadBytes, NULL) == FALSE || v_iReadBytes != sizeof(v_sInfoHeader))
		return false;

	mmUInt const v_iWidth = v_sInfoHeader.biWidth;
	mmUInt const v_iHeight = v_sInfoHeader.biHeight;
	mmUInt const v_iChannels = v_sInfoHeader.biBitCount / 8;

	mmUInt const v_iWidthInBytes = ((v_iChannels * v_iWidth * sizeof(BYTE) - 1) / 4 + 1) * 4;

	if(v_iChannels != 1 && v_iChannels != 3 && v_iChannels != 4)
		return false;

	std::vector<BYTE> v_sBitmapData(v_iWidthInBytes * v_iHeight, 0);
	if(::ReadFile(v_sFile, &v_sBitmapData[0], static_cast<DWORD>(v_sBitmapData.size() * sizeof(BYTE)), &v_iReadBytes, NULL) == FALSE || v_iReadBytes != static_cast<DWORD>(v_sBitmapData.size() * sizeof(BYTE)))
		return false;

	::CloseHandle(v_sFile);

	mmImages::mmImageI * const v_psImage = p_psImageStructure->CreateImage(p_sName, v_iWidth, v_iHeight, static_cast<mmImages::mmImageI::mmPixelType>(v_iChannels));
	if(v_psImage == NULL)
		return false;

	std::vector<mmReal> v_sChannelData(v_iWidth * v_iHeight, 0.0);
	for(mmUInt v_iC = 0; v_iC < v_iChannels; ++v_iC) {
		for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY) {
			for(mmUInt v_iX = 0; v_iX < v_iWidth; ++v_iX) {
				v_sChannelData[v_iY * v_iWidth + v_iX] = static_cast<mmReal>(v_sBitmapData[(v_iHeight - 1 - v_iY) * v_iWidthInBytes + v_iX * v_iChannels + v_iC]) / 255.0;
			}
		}
		mmImages::mmLayerI * const v_psLayer(v_iChannels > 1 ? v_psImage->GetChannel(v_iC % 2 ? v_iC : 2 - v_iC) : v_psImage->GetChannel(v_iC));
		v_psLayer->SetRows(0, v_iHeight, &v_sChannelData[0]);
	}

	return true;
}

bool mmFormats::mmBitmap::Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage) {
	DWORD v_iWrittenBytes = 0;
	if(p_psImage == NULL)
		return false;

	mmUInt const v_iWidth = p_psImage->GetWidth();
	mmUInt const v_iHeight = p_psImage->GetHeight();
	mmUInt const v_iChannels = p_psImage->GetPixelType();

	mmUInt const v_iWidthInBytes = ((v_iChannels * v_iWidth * sizeof(BYTE) - 1) / 4 + 1) * 4;

	BITMAPFILEHEADER v_sFileHeader = {0x4d42, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + v_iHeight * v_iWidthInBytes, 0, 0, 0x36};
	BITMAPINFOHEADER v_sInfoHeader = {sizeof(BITMAPINFOHEADER), v_iWidth, v_iHeight, 1, v_iChannels * 8, BI_RGB, 0, 0x0ec4, 0x0ec4, 0, 0};

	HANDLE v_sFile = ::CreateFile(p_sFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(v_sFile == INVALID_HANDLE_VALUE)
		return false;

	if(::WriteFile(v_sFile, &v_sFileHeader, sizeof(v_sFileHeader), &v_iWrittenBytes, NULL) == FALSE || v_iWrittenBytes != sizeof(v_sFileHeader))
		return false;

	if(::WriteFile(v_sFile, &v_sInfoHeader, sizeof(v_sInfoHeader), &v_iWrittenBytes, NULL) == FALSE || v_iWrittenBytes != sizeof(v_sInfoHeader))
		return false;

	std::vector<mmReal> v_sChannelData(v_iWidth * v_iHeight, 0.0);
	std::vector<BYTE> v_sBitmapData(v_iWidthInBytes * v_iHeight, 0);
	for(mmUInt v_iC = 0; v_iC < v_iChannels; ++v_iC) {
		mmImages::mmLayerI const * const v_psLayer(v_iChannels > 1 ? p_psImage->GetChannel(v_iC % 2 ? v_iC : 2 - v_iC) : p_psImage->GetChannel(v_iC));
		v_psLayer->GetRows(0, v_iHeight, &v_sChannelData[0]);
		for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY) {
			for(mmUInt v_iX = 0; v_iX < v_iWidth; ++v_iX) {
				v_sBitmapData[(v_iHeight - 1 - v_iY) * v_iWidthInBytes + v_iX * v_iChannels + v_iC] = static_cast<BYTE>(v_sChannelData[v_iY * v_iWidth + v_iX] * 255.0);
			}
		}
	}
	if(::WriteFile(v_sFile, &v_sBitmapData[0], v_iHeight * v_iWidthInBytes, &v_iWrittenBytes, NULL) == FALSE || v_iWrittenBytes != (v_iHeight * v_iWidthInBytes))
		return false;

	::CloseHandle(v_sFile);

	return true;
}
