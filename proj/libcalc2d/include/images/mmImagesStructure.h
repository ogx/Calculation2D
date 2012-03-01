//******************************************************************************
//******************************************************************************
//
//  Images implementation
//
//  Description: This header defines implementation of
//	mmImages::mmImagesStructure
//
//******************************************************************************
//******************************************************************************
#ifndef mmImagesStructureH
#define mmImagesStructureH

#include <interfaces\mmIImages.h>
#include <interfaces\mmISynchronize.h>
#include <log\mmLogSender.h>

#include <list>

namespace mmImages
{
	class mmLayer : public mmLayerI {
	public:
		mmLayer(mmID const & p_sID, mmString const & p_sName, mmUInt const p_iWidth, mmUInt const p_iHeight, mmReal const p_rDefaultValue, mmCallbackI * const p_psCallback);
		~mmLayer(void);
	public:
		void Resize(mmUInt const p_iWidth, mmUInt const p_iHeight);
	public: // mmLayerI implementation
		virtual mmID GetID(void) const;
		virtual mmUInt GetWidth(void) const;
		virtual mmUInt GetHeight(void) const;
		virtual mmReal GetDefaultValue(void) const;

		virtual mmStats GetStats(void) const;

		virtual mmString GetName(void) const;
		virtual void SetName(mmString const & p_sName);
		
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmReal p_prValues[]) const;
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmReal const p_prValues[]);

		virtual bool GetPixels(mmRect const & p_sRect, mmReal p_prValues[]) const;
		virtual bool SetPixels(mmRect const & p_sRect, mmReal const p_prValues[]);
	private:
		mmID const m_sID;
		mmUInt m_iWidth;
		mmUInt m_iHeight;
		mmString m_sName;
		mmReal m_rDefaultValue;
		mmReal * m_prValues;
		mmStats m_sStats;
		mmCallbackI * const m_psCallback;
		mmSynchronize::mmReadWriteLockI * const m_psLock;
	};

	class mmImage : public mmImageI, private mmLayerI::mmCallbackI {
	public:
		mmImage(mmID const & p_sID, mmString const & p_sName, mmUInt const p_iWidth, mmUInt const p_iHeight, mmPixelType const p_ePixelType, mmImageI::mmCallbackI * const p_psCallback);
		~mmImage(void);
	public: // mmImageI implementation
		virtual mmID GetID(void) const;
		virtual mmUInt GetWidth(void) const;
		virtual mmUInt GetHeight(void) const;
		virtual void Resize(mmUInt const p_iWidth, mmUInt const p_iHeight);

		virtual mmPixelType GetPixelType(void) const;

		virtual mmString GetName(void) const;
		virtual void SetName(mmString const & p_sName);
		
		virtual mmLayerI * GetChannel(mmUInt const p_iIndex) const;
		
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel8 p_psValues[]) const;
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel24 p_psValues[]) const;
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel32 p_psValues[]) const;
		
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel8 const p_psValues[]);
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel24 const p_psValues[]);
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel32 const p_psValues[]);
		
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel8 p_psValues[]) const;
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel24 p_psValues[]) const;
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel32 p_psValues[]) const;
		
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel8 const p_psValues[]);
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel24 const p_psValues[]);
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel32 const p_psValues[]);
		
		virtual void SetRegionOfInterest(mmRect const & p_sRegion);
		virtual mmRect GetRegionOfInterest(void) const;
		
		virtual mmLayerI * CreateLayer(mmString const & p_sName, mmReal const p_rDefaultValue);
		virtual mmUInt GetLayerCount(void) const;
		virtual mmLayerI * GetLayer(mmID const & p_sID) const;
		virtual mmLayerI * FindLayer(mmLayerI const * const p_psPreviousLayer=NULL, mmString const & p_sName=L"") const;
		virtual bool DeleteLayer(mmUInt const p_iIndex);
		virtual bool DeleteLayer(mmString const & p_sName);
		
		virtual void SetMetadata(mmString const & p_sMetadata);
		virtual mmString GetMetadata(void);
	private: // mmLayerI::mmCallbackI implementation
		virtual void OnLayerCreate(mmLayerI * p_psLayer);
		virtual void OnLayerDestroy(mmLayerI * p_psLayer);
		virtual void OnLayerPropertiesChange(mmLayerI * p_psLayer);
		virtual void OnLayerValuesChange(mmLayerI * p_psLayer);
	private:
		mmID m_sLastLayerID;
		mmID const m_sID;
		mmUInt m_iWidth;
		mmUInt m_iHeight;
		mmString m_sName;
		mmPixelType m_ePixelType;
		std::list<mmLayer*> m_sChannels;
		std::list<mmLayer*> m_sLayers;
		mmRect m_sRegionOfInterest;
		mmString m_sMetadata;
		mmImageI::mmCallbackI * const m_psCallback;
	};

	class mmImageStructure : public mmImageStructureI {
	public:
		mmImageStructure(mmImageI::mmCallbackI * const p_psCallback);
		~mmImageStructure(void);
	public: // mmImageStructureI implementation
		virtual mmImageI * CreateImage(mmString const & p_sName, mmUInt const p_iWidth, mmUInt const p_iHeight, mmImageI::mmPixelType const p_ePixelType);
		virtual mmUInt GetImageCount(void) const;
		virtual mmImageI * GetImage(mmID const & p_sID) const;
		virtual mmImageI * FindImage(mmImageI const * const p_psPreviousImage=NULL, mmString const & p_sName=L"") const;
		virtual bool DeleteImage(mmUInt const p_iIndex);
		virtual bool DeleteImage(mmString const & p_sName);		
	private: 
		mmID m_sLastImageID;
		std::list<mmImage*> m_sImages;
		mmImageI::mmCallbackI * m_psCallback;
	};
};

#endif

