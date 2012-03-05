#include <commands/mmLoad.h>

#include <external/bmpfile.h>

mmCommands::mmLoad::mmLoad(mmImages::mmImageStructureI * const p_psImageStructure) : m_psImageStructure(p_psImageStructure) {
	m_sNames.push_back(L"load");

	m_sParams.push_back(mmParam(L"filename", mmParam::mmTypeFile, false));
}

bool mmCommands::mmLoad::Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams) {
	mmInt v_iWidth, v_iHeight, v_iDepth;
	BYTE * bitmap = Bitmaps::LoadBMP(p_sParams[0].sValue, &v_iWidth, &v_iHeight, &v_iDepth);
	if(bitmap == NULL) {
		m_sError = L"loading file failed";
		return false;
	}

	mmInt v_iChannels = v_iDepth / 8;
	mmReal * pixels =  new mmReal[v_iWidth * v_iHeight];

	std::wstring v_sName = p_sParams[0].sValue;
	v_sName = v_sName.substr(v_sName.find_last_of('\\') + 1);
	v_sName = v_sName.substr(0, v_sName.find_last_of('.'));
	mmImages::mmImageI * const v_psImage = m_psImageStructure->CreateImage(v_sName, v_iWidth, v_iHeight, static_cast<mmImages::mmImageI::mmPixelType>(v_iDepth/8));
	for(int c = 0; c < v_iChannels; ++c) {
		for(int i = 0; i < v_iHeight; ++i)
			for(int j = 0; j < v_iWidth; ++j)
				pixels[i * v_iWidth + j] = static_cast<mmReal>(bitmap[(i * v_iWidth + j) * v_iChannels + c]);
		v_psImage->GetChannel(c)->SetRows(0, v_iHeight, pixels);
	}

	delete [] bitmap;
	delete [] pixels;

	return true;
}
