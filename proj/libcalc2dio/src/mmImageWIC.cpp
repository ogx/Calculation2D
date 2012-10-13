#include <interfaces/mmIFormat.h>
#include <comdef.h> // _com_ptr_t
#include <wincodec.h> // WIC
#include <vector> // image buffer

#pragma comment(lib, "Windowscodecs.lib")

namespace mmFormats 
{

#define HR_RET(X) {if (FAILED(X)) return false;}

#define EVEN_SMARTER(X) _COM_SMARTPTR_TYPEDEF(X, __uuidof(X))
	EVEN_SMARTER(IWICImagingFactory);
	EVEN_SMARTER(IWICBitmapDecoder);
	EVEN_SMARTER(IWICBitmapFrameDecode);
	EVEN_SMARTER(IWICFormatConverter);

	EVEN_SMARTER(IWICStream);
	EVEN_SMARTER(IWICBitmapEncoder);
	EVEN_SMARTER(IWICBitmapFrameEncode);
	
	class ogxHelperWIC
	{
	public:

		struct Image
		{
			std::vector<unsigned char> vBuffer;
			unsigned int uiWidth, uiHeight;
			// bytes per pixel
			unsigned char ucBPP;
			// num of channels in pixel
			unsigned char ucChannels;
		};

		ogxHelperWIC()
		{
			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			m_psFactory.CreateInstance(CLSID_WICImagingFactory);
		}

		~ogxHelperWIC() {
			m_psFactory = 0;
			CoUninitialize();
		}

		GUID FindEncoderFromFileExt(const mmString& p_sFilePath)
		{
			mmString v_sExt = p_sFilePath.substr(p_sFilePath.rfind(L'.'));

			if (L".bmp" == v_sExt) return GUID_ContainerFormatBmp;
			if (L".png" == v_sExt) return GUID_ContainerFormatPng;
			if (L".jpg" == v_sExt || 
				L".jpeg" == v_sExt) return GUID_ContainerFormatJpeg;
			if (L".tif" == v_sExt || 
				L".tiff" == v_sExt) return GUID_ContainerFormatTiff;
			if (L".gif" == v_sExt) return GUID_ContainerFormatGif;
			if (L".ico" == v_sExt) return GUID_ContainerFormatIco;
			if (L".wmp" == v_sExt) return GUID_ContainerFormatWmp;
			else return GUID();
		}

		bool Load(const mmString& p_sFilePath, Image& p_sImage) 
		{
			UINT v_uiStride(0);
			UINT v_uiWidth(0), v_uiHeight(0);

			WICPixelFormatGUID v_sTargetFormat(GUID_WICPixelFormat32bppRGBA), v_sPixelFormat(GUID_WICPixelFormatUndefined);

			IWICBitmapDecoderPtr v_psDecoder;
			IWICBitmapFrameDecodePtr v_psFrame;
			IWICFormatConverterPtr v_psConverter;

			HR_RET(m_psFactory->CreateDecoderFromFilename(p_sFilePath.c_str(), NULL, 
				GENERIC_READ, WICDecodeMetadataCacheOnDemand, &v_psDecoder));

			HR_RET(v_psDecoder->GetFrame(0, &v_psFrame));
			HR_RET(v_psFrame->GetSize(&v_uiWidth, &v_uiHeight));
			HR_RET(v_psFrame->GetPixelFormat(&v_sPixelFormat));

			HR_RET(m_psFactory->CreateFormatConverter(&v_psConverter));

			BOOL v_bCanConvert(FALSE);
			HR_RET(v_psConverter->CanConvert(v_sPixelFormat, v_sTargetFormat, &v_bCanConvert));
			if (FALSE == v_bCanConvert) return false;

			v_uiStride = v_uiWidth*4;

			HR_RET(v_psConverter->Initialize(
				v_psFrame, v_sTargetFormat, 
				WICBitmapDitherTypeNone, NULL, 0.0, 
				WICBitmapPaletteTypeCustom));
				
			p_sImage.vBuffer.resize(v_uiHeight*v_uiStride);

			HR_RET(v_psConverter->CopyPixels(NULL, v_uiStride, p_sImage.vBuffer.size(), &p_sImage.vBuffer.front()));
			p_sImage.uiWidth = v_uiWidth;
			p_sImage.uiHeight = v_uiHeight;
			p_sImage.ucBPP = p_sImage.ucChannels = 4;

			return true;
		}

			bool Save(const mmString& p_sFilePath, const Image& p_sImage) {
				
				IWICStreamPtr v_psStream;
				IWICBitmapEncoderPtr v_psEncoder;
				IWICBitmapFrameEncodePtr v_psFrame;

				// open output file
				HR_RET(m_psFactory->CreateStream(&v_psStream));
				HR_RET(v_psStream->InitializeFromFilename(p_sFilePath.c_str(), GENERIC_WRITE));

				// init encoder
				HR_RET(m_psFactory->CreateEncoder( FindEncoderFromFileExt(p_sFilePath), 0, &v_psEncoder));
				HR_RET(v_psEncoder->Initialize(v_psStream, WICBitmapEncoderNoCache));

				// init frame
				HR_RET(v_psEncoder->CreateNewFrame(&v_psFrame, 0));
				HR_RET(v_psFrame->Initialize(0));
				// http://msdn.microsoft.com/en-us/library/windows/desktop/ee719871(v=vs.85).aspx#encoder_options_examples
				// use IPropertyBag2

				HR_RET(v_psFrame->SetSize(p_sImage.uiWidth, p_sImage.uiHeight));

				WICPixelFormatGUID v_sPixelFormat = GUID_WICPixelFormatUndefined;

				switch (p_sImage.ucChannels)
				{
				case 1:
					v_sPixelFormat = GUID_WICPixelFormat8bppGray;
				case 3:
					v_sPixelFormat = GUID_WICPixelFormat24bppRGB;
				case 4:
					v_sPixelFormat = GUID_WICPixelFormat32bppRGBA;
				}
				
				WICPixelFormatGUID v_sPixelFormatChosen = v_sPixelFormat;
				HR_RET(v_psFrame->SetPixelFormat(&v_sPixelFormatChosen));
				// TODO: convert if (v_sPixelFormatChosen != v_sPixelFormat)

				UINT v_uiStride = p_sImage.uiWidth*p_sImage.ucBPP;
				BYTE* v_pcBuffer = (BYTE*) &p_sImage.vBuffer.front(); 

				HR_RET(v_psFrame->WritePixels(
						p_sImage.uiHeight, v_uiStride, 
						p_sImage.vBuffer.size(), v_pcBuffer));

				// frame and encoder commits done auto-magically
				HR_RET(v_psFrame->Commit());
				HR_RET(v_psEncoder->Commit());

				return true;
			}

	private:
		IWICImagingFactoryPtr m_psFactory;
	};

	class mmImageWIC: public mmFormatI 
	{
		// implementing mmFormatI

		virtual Extensions mmFormatI::GetSupportedExensions( void ) 
		{
			Extensions v_sList;
			v_sList.push_back(L".bmp");
			v_sList.push_back(L".png");
			v_sList.push_back(L".jpg");
			v_sList.push_back(L".jpeg");
			v_sList.push_back(L".tif");
			v_sList.push_back(L".tiff");
			v_sList.push_back(L".gif");
			v_sList.push_back(L".ico");
			v_sList.push_back(L".wmp");
			return v_sList;
		}

		virtual bool mmFormatI::Read( mmString const & p_sFilePath, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName)
		{

			if (p_sFilePath.empty() || !p_psImageStructure) return false;

			mmString v_sName(p_sName);

			if (p_sName.empty())
			{
				const mmInt v_iSlashPos = (mmInt) p_sFilePath.find_last_of(L'\\');
				v_sName = p_sFilePath.substr(v_iSlashPos+1);

				const mmInt v_iDotPos = (mmInt) v_sName.find_last_of(L'.');
				v_sName = v_sName.substr(0, v_iDotPos);
			}

			ogxHelperWIC::Image v_sImage = {};
			if (!m_sWIC.Load(p_sFilePath, v_sImage)) return false;
			mmImages::mmImageI* v_psImage = p_psImageStructure->CreateImage(p_sName, v_sImage.uiWidth, v_sImage.uiHeight, (mmImages::mmImageI::mmPixelType)v_sImage.ucChannels);

			if (!v_psImage) return false;

			const unsigned char* v_pcBuffer = &v_sImage.vBuffer.front();
			const unsigned char v_ucChannels = v_sImage.ucChannels;
			const mmUInt v_uiPixelCount = v_sImage.vBuffer.size()/v_ucChannels;

			std::vector<mmReal> v_vPixels(v_uiPixelCount);
			mmReal* v_prBuf = &v_vPixels.front();

			for (mmUInt c = 0; c < v_ucChannels; ++c)
			{
				for (mmUInt i = 0; i < v_uiPixelCount; ++i) 
					v_prBuf[i] = 1.0*v_pcBuffer[c + i*v_ucChannels]/255.0;

				v_psImage->GetChannel(c)->SetRows(0, v_sImage.uiHeight, v_prBuf);
			}

			return true;
		}

		virtual bool mmFormatI::Write( mmString const & p_sFilePath, mmImages::mmImageI const * const p_psImage ) 
		{
			if (p_sFilePath.empty() || !p_psImage) return false;

			ogxHelperWIC::Image v_sImage = {};
			v_sImage.uiWidth = p_psImage->GetWidth();
			v_sImage.uiHeight = p_psImage->GetHeight();
			v_sImage.ucChannels = v_sImage.ucBPP = p_psImage->GetPixelType();

			const mmUInt v_uiPixelCount = v_sImage.uiWidth*v_sImage.uiHeight;
			const unsigned char v_ucChannels = v_sImage.ucChannels;

			std::vector<mmReal> v_vPixels(v_uiPixelCount);
			mmReal* v_prBuf = &v_vPixels.front();

			v_sImage.vBuffer.resize(v_uiPixelCount*v_sImage.ucBPP);
			unsigned char* v_pcBuffer = &v_sImage.vBuffer.front();

			for (mmUInt c = 0; c < v_ucChannels; ++c)
			{
				p_psImage->GetChannel(c)->GetRows(0, v_sImage.uiHeight, v_prBuf);

				// RGB to BGR conversion
				unsigned char v_ucSwappedChannel = c;
				if (2 < v_ucChannels)
				{
					if (0 == c) c = 2;
					else if (2 == c) c = 0;
				}

				for (mmUInt i = 0; i < v_uiPixelCount; ++i) {
					mmInt val = (mmInt) (v_prBuf[i]*255.0 + 0.5);
					if (255 < val) val = 255;
					else if (0 > val) val = 0;

					v_pcBuffer[v_ucSwappedChannel + i*v_ucChannels] = val;
				}
			}

			return m_sWIC.Save(p_sFilePath, v_sImage);
		}
		// members
		ogxHelperWIC m_sWIC;
	};

	mmFormatI* CreateImageWIC()
	{
		return new mmImageWIC();
	}


}