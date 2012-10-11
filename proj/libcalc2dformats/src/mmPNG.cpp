#include <mmPNG.h>

#include <vector>

#include <png.h>
#include <Windows.h>

mmFormats::mmPNG::mmPNG(void) {
	m_sSupportedExtensions.push_back(L"png");
}

bool mmFormats::mmPNG::Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName) {
	// open file
	FILE * v_psFile = NULL;
	if(::_wfopen_s(&v_psFile, p_sFileName.c_str(), L"rb") != 0 || v_psFile == NULL)
		return false;

	// test for a png
	png_byte v_pcFileHeader[8] = {};    // 8 is the maximum size that can be checked
	if(::fread(v_pcFileHeader, sizeof(png_byte), sizeof(v_pcFileHeader) / sizeof(png_byte), v_psFile) == 0 || ::png_sig_cmp(v_pcFileHeader, 0, 8) != 0)
		return false;

	// initialize stuff
	png_structp v_psPNGFile = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(v_psPNGFile == NULL)
		return false;

	png_infop v_psPNGInfo = ::png_create_info_struct(v_psPNGFile);
	if(v_psPNGFile == NULL)
		return false;

	::png_init_io(v_psPNGFile, v_psFile);
	::png_set_sig_bytes(v_psPNGFile, 8);

	::png_read_info(v_psPNGFile, v_psPNGInfo);

	if(::png_get_bit_depth(v_psPNGFile, v_psPNGInfo) != 8)
		return false;

	mmUInt const v_iWidth = ::png_get_image_width(v_psPNGFile, v_psPNGInfo);
	mmUInt const v_iHeight = ::png_get_image_height(v_psPNGFile, v_psPNGInfo);
	mmUInt const v_iChannels = ::png_get_channels(v_psPNGFile, v_psPNGInfo);

	if(v_iChannels != 1 && v_iChannels != 3 && v_iChannels != 4)
		return false;

	int const v_iNumberOfPasses = ::png_set_interlace_handling(v_psPNGFile);
	::png_read_update_info(v_psPNGFile, v_psPNGInfo);

	// read file
	std::vector<png_bytep> v_sRowPointers(v_iHeight, NULL);
	for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY)
		v_sRowPointers[v_iY] = new png_byte[::png_get_rowbytes(v_psPNGFile, v_psPNGInfo)];

	::png_read_image(v_psPNGFile, &v_sRowPointers[0]);

	::fclose(v_psFile);	

	mmImages::mmImageI * const v_psImage = p_psImageStructure->CreateImage(p_sName, v_iWidth, v_iHeight, static_cast<mmImages::mmImageI::mmPixelType>(v_iChannels));
	if(v_psImage == NULL)
		return false;

	std::vector<mmReal> v_sChannelData(v_iWidth * v_iHeight, 0.0);
	for(mmUInt v_iC = 0; v_iC < v_iChannels; ++v_iC) {
		for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY) {
			for(mmUInt v_iX = 0; v_iX < v_iWidth; ++v_iX) {
				v_sChannelData[v_iY * v_iWidth + v_iX] = static_cast<mmReal>(v_sRowPointers[v_iY][v_iX * v_iChannels + v_iC]) / 255.0;
			}
		}
		v_psImage->GetChannel(v_iC)->SetRows(0, v_iHeight, &v_sChannelData[0]);
	}

	// clean up
	for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY)
		delete [] v_sRowPointers[v_iY];

	::png_destroy_read_struct(&v_psPNGFile, &v_psPNGInfo, NULL);

	return true;
}

bool mmFormats::mmPNG::Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage) {
	static int const v_piColorTypeLUT[] = {-1, PNG_COLOR_TYPE_GRAY, -1, PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_RGBA};

	if(p_psImage == NULL)
		return false;

	// create file
	FILE * v_psFile = NULL; 
	if(::_wfopen_s(&v_psFile, p_sFileName.c_str(), L"wb") != 0 || v_psFile == NULL)
		return false;

	// initialize
	png_structp v_psPNGFile = ::png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(v_psPNGFile == NULL)
		return false;

	png_infop v_psPNGInfo = ::png_create_info_struct(v_psPNGFile);
	if(! v_psPNGInfo)
		return false;

	::png_init_io(v_psPNGFile, v_psFile);

	mmUInt const v_iWidth = p_psImage->GetWidth();
	mmUInt const v_iHeight = p_psImage->GetHeight();
	mmUInt const v_iChannels = p_psImage->GetPixelType();
	int const v_iColorType = v_piColorTypeLUT[v_iChannels];
	if(v_iColorType == -1)
		return false;

	// write header
	::png_set_IHDR(v_psPNGFile, v_psPNGInfo, v_iWidth, v_iHeight, 8, v_iColorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	::png_write_info(v_psPNGFile, v_psPNGInfo);
	// write bytes
	png_bytepp v_ppsRowPointers = new png_bytep[v_iHeight];
	for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY)
		v_ppsRowPointers[v_iY] = new png_byte[::png_get_rowbytes(v_psPNGFile, v_psPNGInfo)];

	std::vector<mmReal> v_sChannelData(v_iWidth * v_iHeight, 0.0);
	for(mmUInt v_iC = 0; v_iC < v_iChannels; ++v_iC) {
		p_psImage->GetChannel(v_iC)->GetRows(0, v_iHeight, &v_sChannelData[0]);
		for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY) {
			for(mmUInt v_iX = 0; v_iX < v_iWidth; ++v_iX) {
				v_ppsRowPointers[v_iY][v_iX * v_iChannels + v_iC] = static_cast<png_byte>(v_sChannelData[v_iY * v_iWidth + v_iX] * 255.0);
			}
		}
	}

	::png_write_image(v_psPNGFile, v_ppsRowPointers);

	::fclose(v_psFile);		

	// clean up
	for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY)
		delete [] v_ppsRowPointers[v_iY];
	delete [] v_ppsRowPointers;

	::png_destroy_write_struct(&v_psPNGFile, &v_psPNGInfo);

	return true;
}
