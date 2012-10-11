#include <mmImageXML.h>
#include <mmBitmap.h>
#include <mmPNG.h>

#include <algorithm>
#include <vector>

#include <windows.h>

#include <interfaces/mmInterfaceInitializers.h>
#include <mmStringUtilities.h>
#include <images/mmImagesStructure.h>

mmFormats::mmImageXML::mmImageXML(void) {
	m_sSupportedExtensions.push_back(L"ixml");
}

namespace mmFormats {
	inline mmString GetNodeText(mmXML::mmXMLNodeI * const p_psParent, mmString const & p_sNodeName) {
		return (p_psParent->FindChild(p_sNodeName) == NULL ? mmString(L"") : p_psParent->FindChild(p_sNodeName)->GetText());
	}

	wchar_t const * const g_ppcTypeToString[] = {
		L"1",
		L"3",
		L"4",
		L"mmP8", 
		L"mmP24",
		L"mmP32",
		L"8bit Intensity(8 bits)", 
		L"24bit Red(8 bits)Green(8 bits)Blue(8 bits)", 
		L"32bit Red(8 bits)Green(8 bits)Blue(8 bits)Alpha(8 bits)", 
	};
	int const g_ppcChannels[] = {1, 3, 4, 1, 3, 4, 1, 3, 4};
	wchar_t const * const * const g_ppcTypeToStringEnd = g_ppcTypeToString + sizeof(g_ppcTypeToString) / sizeof(*g_ppcTypeToString);
	struct EqualStrings {
		EqualStrings(wchar_t const p_pcS[]) : m_pcS(p_pcS) {}
		bool operator ()(wchar_t const p_pcS[]) { return ::wcscmp(p_pcS, m_pcS) == 0; }
	private:
		wchar_t const * const m_pcS;
	};

	mmInt ChannelsFromString(mmString const & p_sString) {
		wchar_t const * const * const v_ppcTypeToString = std::find_if(g_ppcTypeToString, g_ppcTypeToStringEnd, EqualStrings(p_sString.c_str()));
		return (v_ppcTypeToString != g_ppcTypeToStringEnd ? g_ppcChannels[v_ppcTypeToString - g_ppcTypeToString] : -1);
	}
};

bool mmFormats::mmImageXML::Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName) {
	std::auto_ptr<mmXML::mmXMLDocI> v_psXMLDoc(mmInterfaceInitializers::CreateXMLDocument());
	v_psXMLDoc->ParseXMLFile(p_sFileName);
	mmString const v_sDirectory = p_sFileName.substr(0, p_sFileName.find_last_of(L'\\'));

	mmXML::mmXMLNodeI * const v_psRoot = v_psXMLDoc->GetXMLRootNode();
	if(v_psRoot == NULL || (v_psRoot->GetName() != L"ImagesStructure" && v_psRoot->GetName() != L"ImageStructure") || v_psRoot->GetChildren().empty())
		return false;

	mmXML::mmXMLNodeI * const v_psImageNode = v_psRoot->GetChild(0);
	mmString const v_sImageName = (p_sName.empty() ? GetNodeText(v_psImageNode, L"Name") : p_sName);
	mmUInt const v_iWidth = FromString<mmUInt>(GetNodeText(v_psImageNode, L"Width"));
	mmUInt const v_iHeight = FromString<mmUInt>(GetNodeText(v_psImageNode, L"Height"));
	mmInt const v_iChannels = ChannelsFromString(GetNodeText(v_psImageNode, L"PixelFormat"));
	mmImages::mmImageI::mmPixelType v_sPixelType = static_cast<mmImages::mmImageI::mmPixelType>(v_iChannels);
	mmString const v_sPreviewPath = GetNodeText(v_psImageNode, L"PixelsPreviewPath");
	mmString const v_sPreviewExtension = mmStringUtilities::MMStringToLower(v_sPreviewPath.substr(v_sPreviewPath.find_last_of(L'.') + 1));
	mmString const v_sPixelsPath = GetNodeText(v_psImageNode, L"PixelsPath");

	if(v_iChannels != 1 && v_iChannels != 3 && v_iChannels != 4)
		return false;

	mmImages::mmImageI * v_psImage = NULL;
	if(v_sPixelsPath.empty()) {
		std::auto_ptr<mmFormatI> v_psReader;
		if(v_sPreviewExtension == L"bmp")
			v_psReader.reset(new mmBitmap);
		else if(v_sPreviewExtension == L"png")
			v_psReader.reset(new mmPNG);

		if(! v_psReader->Read(v_sDirectory + L"\\" + v_sPreviewPath, p_psImageStructure, v_sImageName))
			return false;

		if((v_psImage = p_psImageStructure->FindImage(NULL, v_sImageName)) == NULL)
			return false;
	} else {
		HANDLE v_sFile = ::CreateFile((v_sDirectory + L"\\" + v_sPixelsPath).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(v_sFile == INVALID_HANDLE_VALUE)
			return false;

		DWORD v_iReadBytes = 0;
		std::vector<mmReal> v_sImageData(v_iWidth * v_iHeight * v_iChannels, 0.0);
		if(::ReadFile(v_sFile, &v_sImageData[0], static_cast<DWORD>(v_sImageData.size() * sizeof(mmReal)), &v_iReadBytes, NULL) == FALSE || v_iReadBytes != static_cast<DWORD>(v_sImageData.size() * sizeof(mmReal)))
			return false;

		::CloseHandle(v_sFile);

		if((v_psImage = p_psImageStructure->CreateImage(v_sImageName, v_iWidth, v_iHeight, v_sPixelType)) == NULL)
			return false;

		std::vector<mmReal> v_sChannelData(v_iWidth * v_iHeight, 0.0);
		for(mmInt v_iC = 0; v_iC < v_iChannels; ++v_iC) {
			for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY) {
				for(mmUInt v_iX = 0; v_iX < v_iWidth; ++v_iX) {
					v_sChannelData[v_iY * v_iWidth + v_iX] = v_sImageData[(v_iY * v_iWidth + v_iX) * v_iChannels + v_iC] / 255.0;
				}
			}
			v_psImage->GetChannel(v_iC)->SetRows(0, v_iHeight, &v_sChannelData[0]);
		}
	}

	if(v_psImageNode->FindChild(L"XMLMetadata") != NULL)
		v_psImage->SetMetadata(GetNodeText(v_psImageNode, L"XMLMetadata"));
	else if(v_psImageNode->FindChild(L"Metadata") != NULL)
		v_psImage->SetMetadata(GetNodeText(v_psImageNode, L"Metadata"));

	mmString const v_sROI = GetNodeText(v_psImageNode, L"RegionOfInterest");
	if(! v_sROI.empty())
		v_psImage->SetRegionOfInterest(FromString<mmRect>(v_sROI));

	std::vector<mmXML::mmXMLNodeI*> const v_sLayers = v_psImageNode->GetChildren();
	for(mmUInt v_iI = 0;  v_iI < v_sLayers.size(); ++v_iI) {
		if(v_sLayers[v_iI]->GetName() != L"DataLayer" && v_sLayers[v_iI]->GetName() != L"Layer")
			continue;

		HANDLE v_sFile = ::CreateFile((v_sDirectory + L"\\" + GetNodeText(v_sLayers[v_iI], L"Path")).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(v_sFile == INVALID_HANDLE_VALUE)
			continue;

		DWORD v_iReadBytes = 0;
		std::vector<mmReal> v_sLayerData(v_iWidth * v_iHeight, 0.0);
		if(::ReadFile(v_sFile, &v_sLayerData[0], static_cast<DWORD>(v_sLayerData.size() * sizeof(mmReal)), &v_iReadBytes, NULL) == FALSE || v_iReadBytes != static_cast<DWORD>(v_sLayerData.size() * sizeof(mmReal)))
			continue;

		::CloseHandle(v_sFile);

		mmImages::mmLayerI* const v_psLayer = v_psImage->CreateLayer(GetNodeText(v_sLayers[v_iI], L"Name"), FromString<mmReal>(GetNodeText(v_sLayers[v_iI], L"DefaultValue")));
		if(v_psLayer == NULL)
			continue;

		v_psLayer->SetRows(0, v_iHeight, &v_sLayerData[0]);
	}

	return true;
}

bool mmFormats::mmImageXML::Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage) {
	static mmUInt const v_iMaxPreviewDimension = 800;

	if(p_psImage == NULL)
		return false;

	mmString const v_sDataPath = p_sFileName.substr(0, p_sFileName.find_last_of(L'.'));
	mmString const v_sDataDir = v_sDataPath.substr(v_sDataPath.find_last_of('\\') + 1);
	mmUInt const v_iWidth = p_psImage->GetWidth();
	mmUInt const v_iHeight = p_psImage->GetHeight();
	mmUInt const v_iChannels = p_psImage->GetPixelType();
	/* calculate preview dimensions */
	mmUInt v_iPreviewWidth = 0, v_iPreviewHeight = 0;
	if(v_iWidth < v_iMaxPreviewDimension && v_iHeight < v_iMaxPreviewDimension) {
		v_iPreviewWidth = v_iWidth;
		v_iPreviewHeight = v_iHeight;
	} else {
		if(v_iWidth > v_iHeight) {
			v_iPreviewWidth = v_iMaxPreviewDimension;
			v_iPreviewHeight = v_iHeight * v_iPreviewWidth / v_iWidth;
		} else {
			v_iPreviewWidth = v_iWidth * v_iPreviewHeight / v_iHeight;
			v_iPreviewHeight = v_iMaxPreviewDimension;
		}
	}
	/* create data directory */
	std::auto_ptr<mmFileIO::mmFileUtilsI>(mmInterfaceInitializers::CreateFileUtils())->CreateDir(v_sDataPath);
	/* create XML */
	std::auto_ptr<mmXML::mmXMLDocI> v_psXMLDoc(mmInterfaceInitializers::CreateXMLDocument());
	v_psXMLDoc->CreateXMLRootNode(L"ImageStructure");
	mmXML::mmXMLNodeI * const v_psRoot = v_psXMLDoc->GetXMLRootNode();
	mmXML::mmXMLNodeI * const v_psImageNode = v_psRoot->AddChild(L"Image");

	v_psImageNode->AddChild(L"Name")->SetText(p_psImage->GetName());
	v_psImageNode->AddChild(L"Width")->SetText(ToString(v_iWidth));
	v_psImageNode->AddChild(L"Height")->SetText(ToString(v_iHeight));
	v_psImageNode->AddChild(L"PixelFormat")->SetText(ToString(v_iChannels));
	v_psImageNode->AddChild(L"RegionOfInterest")->SetText(ToString(p_psImage->GetRegionOfInterest()));
	v_psImageNode->AddChild(L"Metadata")->SetText(p_psImage->GetMetadata());
	v_psImageNode->AddChild(L"PixelsPreviewPath")->SetText(v_sDataDir + L"\\0_preview.png");
	v_psImageNode->AddChild(L"PixelsPath")->SetText(v_sDataDir + L"\\1_pixels.bin");

	/* save image and preview */ {
		DWORD v_iWrittenBytes = 0;
		HANDLE v_sFile = ::CreateFile((v_sDataPath + L"\\1_pixels.bin").c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(v_sFile == INVALID_HANDLE_VALUE)
			return false;
		std::vector<mmReal> v_sPreviewData(v_iPreviewWidth * v_iPreviewHeight, 0.0);
		std::auto_ptr<mmImages::mmImageStructure> v_psTempImageStructure(new mmImages::mmImageStructure(NULL));
		mmImages::mmImageI* const v_psPreviewImage = v_psTempImageStructure->CreateImage(L"preview", v_iPreviewWidth, v_iPreviewHeight, p_psImage->GetPixelType());

		std::vector<mmReal> v_sChannelData(v_iWidth * v_iHeight, 0.0);
		std::vector<mmReal> v_sImageData(v_iWidth * v_iHeight * v_iChannels, 0.0);
		for(mmUInt v_iC = 0; v_iC < v_iChannels; ++v_iC) {
			p_psImage->GetChannel(v_iC)->GetRows(0, v_iHeight, &v_sChannelData[0]);
			for(mmUInt v_iY = 0; v_iY < v_iHeight; ++v_iY) {
				for(mmUInt v_iX = 0; v_iX < v_iWidth; ++v_iX) {
					v_sImageData[(v_iY * v_iWidth + v_iX) * v_iChannels + v_iC]  = v_sChannelData[v_iY * v_iWidth + v_iX] * 255.0;
				}
			}
			for(mmUInt v_iY = 0; v_iY < v_iPreviewHeight; ++v_iY) {
				for(mmUInt v_iX = 0; v_iX < v_iPreviewWidth; ++v_iX) {
					v_sPreviewData[v_iY * v_iPreviewWidth + v_iX]  = v_sChannelData[(v_iY * v_iHeight / v_iPreviewHeight) * v_iWidth + (v_iX * v_iWidth / v_iPreviewWidth)];
				}
			}
			v_psPreviewImage->GetChannel(v_iC)->SetRows(0, v_iPreviewHeight, &v_sPreviewData[0]);
		}

		mmPNG().Write(v_sDataPath + L"\\0_preview.png", v_psPreviewImage);
		
		if(::WriteFile(v_sFile, &v_sImageData[0], static_cast<DWORD>(v_sImageData.size() * sizeof(mmReal)), &v_iWrittenBytes, NULL) == FALSE || v_iWrittenBytes != static_cast<DWORD>(v_sImageData.size() * sizeof(mmReal)))
			return false;

		::CloseHandle(v_sFile);
	}

	/* save layers */ {
		mmUInt v_iLayerIndex = 2;
		std::vector<mmReal> v_sLayerData(v_iWidth * v_iHeight, 0.0);
		DWORD v_iWrittenBytes = 0;
		for(mmImages::mmLayerI const * v_psLayer = p_psImage->FindLayer(); v_psLayer != NULL; v_psLayer = p_psImage->FindLayer(v_psLayer), ++v_iLayerIndex) {
			mmXML::mmXMLNodeI * const v_psLayerNode = v_psImageNode->AddChild(L"Layer");
			v_psLayerNode->AddChild(L"Name")->SetText(v_psLayer->GetName());
			v_psLayerNode->AddChild(L"DefaultValue")->SetText(ToString(v_psLayer->GetDefaultValue()));
			v_psLayerNode->AddChild(L"Path")->SetText(v_sDataDir + L"\\" + ToString(v_iLayerIndex) + L"_layer.bin");

			HANDLE v_sFile = ::CreateFile((v_sDataPath + L"\\" + ToString(v_iLayerIndex) + L"_layer.bin").c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(v_sFile == INVALID_HANDLE_VALUE)
				continue;

			v_psLayer->GetRows(0, v_iHeight, &v_sLayerData[0]);
			if(::WriteFile(v_sFile, &v_sLayerData[0], static_cast<DWORD>(v_sLayerData.size() * sizeof(mmReal)), &v_iWrittenBytes, NULL) == FALSE || v_iWrittenBytes != static_cast<DWORD>(v_sLayerData.size() * sizeof(mmReal)))
				continue;

			::CloseHandle(v_sFile);
		}
	}

	/* write XML */
	v_psXMLDoc->SaveToXMLFile(p_sFileName);

	return true;
}
