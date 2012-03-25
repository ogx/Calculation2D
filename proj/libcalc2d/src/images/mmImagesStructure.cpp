#include <images\mmImagesStructure.h>

#include <windows.h>

#include <math.h>
#include <limits.h>
#include <float.h>

#include <mmStringUtilities.h>
#include <mmOperatingSystemCalls.h>
#include <interfaces\mmInterfaceInitializers.h>

#undef min
#undef max
#include <algorithm>
#include <iterator>

struct FindByName {
	FindByName(mmString const & p_sName) : m_sName(p_sName) {}
	bool operator () (mmImages::mmLayer * const p_psLayer) {
		return p_psLayer->GetName() == m_sName;
	}
	bool operator () (mmImages::mmImage * const p_psImage) {
		return p_psImage->GetName() == m_sName;
	}
private:
	mmString const m_sName;
};

template <class _Id> // templated finder-by-id class, used only within this file
struct _FinderByID {
	_FinderByID(_Id const & p_sID) : m_sID(p_sID) {}

	template <class _Identifiable>
	bool operator () (_Identifiable * const p_psIdentifiable) {
		return p_psIdentifiable->GetID() == m_sID;
	}
private:
	_Id const m_sID;
};
template <class _Id> // helper function enabling implicit template argument
inline _FinderByID<_Id> FindByID(_Id const & p_sID)
{
	return _FinderByID<_Id>(p_sID);
}

mmImages::mmLayerI::sID const mmImages::mmLayerI::sID::invalid = { mmID::invalid, mmID::invalid };

mmImages::mmLayer::mmLayer(sID const & p_sID, mmString const & p_sName, mmUInt const p_iWidth, mmUInt const p_iHeight, mmReal const p_rDefaultValue, mmCallbackI * const p_psCallback) :
	m_sID(p_sID),
	m_sName(p_sName),
	m_rDefaultValue(p_rDefaultValue),
	m_prValues((p_iWidth * p_iHeight) ? new mmReal[p_iWidth * p_iHeight] : NULL),
	m_iWidth(p_iWidth), m_iHeight(p_iHeight), 
	m_psCallback(p_psCallback), 
	m_psLock(mmInterfaceInitializers::CreateReadWriteLock())
{
	std::fill(m_prValues, m_prValues + m_iWidth * m_iHeight, m_rDefaultValue);

	if(m_psCallback != NULL)
		m_psCallback->OnLayerCreate(this);
}

mmImages::mmLayer::~mmLayer(void) {
	delete m_psLock;
	delete [] m_prValues;

	if(m_psCallback != NULL)
		m_psCallback->OnLayerDestroy(this);
}

mmImages::mmLayerI::sID mmImages::mmLayer::GetID(void) const {
	return m_sID;
}

mmUInt mmImages::mmLayer::GetWidth(void) const {
	return m_iWidth;
}

mmUInt mmImages::mmLayer::GetHeight(void) const {
	return m_iHeight;
}

mmReal mmImages::mmLayer::GetDefaultValue(void) const {
	return m_rDefaultValue;
}

void mmImages::mmLayer::Resize(mmUInt const p_iWidth, mmUInt const p_iHeight) {
	m_psLock->LockForWrite();

	if(p_iHeight * p_iWidth == 0) {
		delete [] m_prValues;
		m_prValues = NULL;

		m_iWidth = m_iHeight = 0;
	} else {
		mmReal * const v_prValues = new mmReal[p_iWidth * p_iHeight];
		
		if(p_iWidth > m_iHeight || p_iWidth > m_iWidth)
			std::fill(v_prValues, v_prValues + p_iWidth * p_iHeight, m_rDefaultValue);

		mmUInt const v_iCopyWidth = std::min(p_iWidth, m_iWidth), v_iCopyHeight = std::min(p_iHeight, m_iHeight);
		for(mmUInt v_iY = 0; v_iY < v_iCopyHeight; ++v_iY)
			::memcpy(v_prValues + v_iY * p_iWidth, m_prValues + v_iY * m_iWidth, v_iCopyWidth * sizeof(mmReal));

		delete [] m_prValues;

		m_prValues = v_prValues;
		m_iWidth = p_iWidth;
		m_iHeight = p_iHeight;

	}

	m_sStats.Clear();

	m_psLock->UnlockFromWrite();
		
	if(m_psCallback != NULL)
		m_psCallback->OnLayerPropertiesChange(this);
}

mmImages::mmStats mmImages::mmLayer::GetStats(void) const {
	return m_sStats;
}

mmString mmImages::mmLayer::GetName(void) const {
	return m_sName;
}

void mmImages::mmLayer::SetName(mmString const & p_sName) {
	m_psLock->LockForWrite();

	m_sName = p_sName;

	m_psLock->UnlockFromWrite();

	if(m_psCallback != NULL)
		m_psCallback->OnLayerPropertiesChange(this);
}

bool mmImages::mmLayer::GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmReal p_prValues[]) const {
	if(p_iStart > m_iHeight || (p_iStart + p_iCount) > m_iHeight)
		return false;

	m_psLock->LockForRead();
	
	::memcpy(p_prValues, m_prValues + p_iStart * m_iWidth, p_iCount * m_iWidth * sizeof(mmReal));
	
	m_psLock->UnlockFromRead();

	return true;
}

bool mmImages::mmLayer::SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmReal const p_prValues[]) {
	if(p_iStart > m_iHeight || (p_iStart + p_iCount) > m_iHeight)
		return false;

	m_psLock->LockForWrite();

	::memcpy(m_prValues + p_iStart * m_iWidth, p_prValues, p_iCount * m_iWidth * sizeof(mmReal));

	m_sStats.Update(m_prValues, m_iWidth * m_iHeight);

	m_psLock->UnlockFromWrite();

	if(m_psCallback != NULL)
		m_psCallback->OnLayerValuesChange(this);
	
	return true;
}

bool mmImages::mmLayer::GetPixels(mmRect const & p_sRect, mmReal p_prValues[]) const
{
	if(p_sRect.iLeft > m_iWidth || p_sRect.iTop > m_iHeight || (p_sRect.iLeft + p_sRect.iWidth) > m_iWidth || (p_sRect.iTop + p_sRect.iHeight) > m_iHeight || p_sRect.iWidth * p_sRect.iHeight == 0)
		return false;

	m_psLock->LockForRead();
	
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		::memcpy(p_prValues + v_iRow * p_sRect.iWidth, m_prValues + (p_sRect.iTop + v_iRow) * m_iWidth + p_sRect.iLeft, p_sRect.iWidth * sizeof(mmReal));
	
	m_psLock->UnlockFromRead();

	return true;
}

bool mmImages::mmLayer::SetPixels(mmRect const & p_sRect, mmReal const p_prValues[]) {
	if(p_sRect.iLeft > m_iWidth || p_sRect.iTop > m_iHeight || (p_sRect.iLeft + p_sRect.iWidth) > m_iWidth || (p_sRect.iTop + p_sRect.iHeight) > m_iHeight || p_sRect.iWidth * p_sRect.iHeight == 0)
		return false;

	m_psLock->LockForWrite();

	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		::memcpy(m_prValues + (p_sRect.iTop + v_iRow) * m_iWidth + p_sRect.iLeft, p_prValues + v_iRow * p_sRect.iWidth, p_sRect.iWidth * sizeof(mmReal));

	m_sStats.Update(m_prValues, m_iWidth * m_iHeight);

	m_psLock->UnlockFromWrite();

	if(m_psCallback != NULL)
		m_psCallback->OnLayerValuesChange(this);
	
	return true;
}

mmImages::mmImage::mmImage(sID const & p_sID, mmString const & p_sName, mmUInt const p_iWidth, mmUInt const p_iHeight, mmPixelType const p_ePixelType, mmImageI::mmCallbackI * const p_psCallback) :
	m_sID(p_sID),
	m_sName(p_sName),
	m_iWidth(p_iWidth), m_iHeight(p_iHeight), 
	m_ePixelType(p_ePixelType), 
	m_psCallback(p_psCallback),
	m_sRegionOfInterest(0, 0, p_iWidth, p_iHeight)
{
	switch(m_ePixelType) {
		case mmP8:
			m_sChannels.push_back(new mmLayer(mmLayerI::MakeID(p_sID, ++m_sLastLayerID), L"I", m_iWidth, m_iHeight, 0.0, NULL));
			break;
		case mmP24:
			m_sChannels.push_back(new mmLayer(mmLayerI::MakeID(p_sID, ++m_sLastLayerID), L"R", m_iWidth, m_iHeight, 0.0, NULL));
			m_sChannels.push_back(new mmLayer(mmLayerI::MakeID(p_sID, ++m_sLastLayerID), L"G", m_iWidth, m_iHeight, 0.0, NULL));
			m_sChannels.push_back(new mmLayer(mmLayerI::MakeID(p_sID, ++m_sLastLayerID), L"B", m_iWidth, m_iHeight, 0.0, NULL));
			break;
		case mmP32:
			m_sChannels.push_back(new mmLayer(mmLayerI::MakeID(p_sID, ++m_sLastLayerID), L"R", m_iWidth, m_iHeight, 0.0, NULL));
			m_sChannels.push_back(new mmLayer(mmLayerI::MakeID(p_sID, ++m_sLastLayerID), L"G", m_iWidth, m_iHeight, 0.0, NULL));
			m_sChannels.push_back(new mmLayer(mmLayerI::MakeID(p_sID, ++m_sLastLayerID), L"B", m_iWidth, m_iHeight, 0.0, NULL));
			m_sChannels.push_back(new mmLayer(mmLayerI::MakeID(p_sID, ++m_sLastLayerID), L"A", m_iWidth, m_iHeight, 0.0, NULL));
			break;
		default:
			break;
	}

	if(m_psCallback != NULL)
		m_psCallback->OnImageCreate(this);
}

mmImages::mmImage::~mmImage(void) {
	for(std::list<mmLayer*>::iterator v_iI = m_sChannels.begin(); v_iI != m_sChannels.end(); ++v_iI)
		delete *v_iI;

	for(std::list<mmLayer*>::iterator v_iI = m_sLayers.begin(); v_iI != m_sLayers.end(); ++v_iI)
		delete *v_iI;

	if(m_psCallback != NULL)
		m_psCallback->OnImageDestroy(this);
}

mmID mmImages::mmImage::GetID(void) const {
	return m_sID;
}

mmUInt mmImages::mmImage::GetWidth(void) const {
	return m_iWidth;
}

mmUInt mmImages::mmImage::GetHeight(void) const {
	return m_iHeight;
}

void mmImages::mmImage::Resize(mmUInt const p_iWidth, mmUInt const p_iHeight) {
	for(std::list<mmLayer*>::iterator v_iI = m_sChannels.begin(); v_iI != m_sChannels.end(); ++v_iI)
		(*v_iI)->Resize(p_iWidth, p_iHeight);

	m_iWidth = p_iWidth;
	m_iHeight = p_iHeight;

	if(m_psCallback != NULL)
		m_psCallback->OnImagePropertiesChange(this);

	for(std::list<mmLayer*>::iterator v_iI = m_sLayers.begin(); v_iI != m_sLayers.end(); ++v_iI)
		(*v_iI)->Resize(p_iWidth, p_iHeight);
}

mmImages::mmImage::mmPixelType mmImages::mmImage::GetPixelType(void) const {
	return m_ePixelType;
}

mmString mmImages::mmImage::GetName(void) const {
	return m_sName;
}

void mmImages::mmImage::SetName(mmString const & p_sName) {
	m_sName = p_sName;

	if(m_psCallback != NULL)
		m_psCallback->OnImagePropertiesChange(this);
}

mmImages::mmLayerI * mmImages::mmImage::GetChannel(mmUInt const p_iIndex) const {
	if(p_iIndex < m_sChannels.size()) {
		std::list<mmLayer*>::const_iterator v_sLayer = m_sChannels.begin();	
		std::advance(v_sLayer, p_iIndex);
		return *v_sLayer;
	} else
		return NULL;
}

bool mmImages::mmImage::GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel8 p_psValues[]) const {
	if(m_ePixelType != mmP8 || p_iStart > (m_iWidth * m_iHeight) || (p_iStart + p_iCount) > (m_iWidth * m_iHeight) || p_iCount == 0)
		return false;

	m_sChannels.front()->GetRows(p_iStart, p_iCount, &p_psValues->rI);

	return true;
}

bool mmImages::mmImage::GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel24 p_psValues[]) const {
	if((m_ePixelType != mmP24 && m_ePixelType != mmP32) || p_iStart > (m_iWidth * m_iHeight) || (p_iStart + p_iCount) > (m_iWidth * m_iHeight) || p_iCount == 0)
		return false;

	mmReal * v_prValues = new mmReal[p_iCount * m_iWidth];
	std::list<mmLayer*>::const_iterator v_sChannel = m_sChannels.begin();

	(*v_sChannel)->GetRows(p_iStart, p_iCount, v_prValues);
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		p_psValues[v_iI].rR = v_prValues[v_iI];

	++v_sChannel;
	(*v_sChannel)->GetRows(p_iStart, p_iCount, v_prValues);
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		p_psValues[v_iI].rG = v_prValues[v_iI];

	++v_sChannel;
	(*v_sChannel)->GetRows(p_iStart, p_iCount, v_prValues);
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		p_psValues[v_iI].rB = v_prValues[v_iI];

	delete [] v_prValues;

	return true;
}

bool mmImages::mmImage::GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel32 p_psValues[]) const {
	if(m_ePixelType != mmP32 || p_iStart > (m_iWidth * m_iHeight) || (p_iStart + p_iCount) > (m_iWidth * m_iHeight) || p_iCount == 0)
		return false;

	mmReal * v_prValues = new mmReal[p_iCount * m_iWidth];
	std::list<mmLayer*>::const_iterator v_sChannel = m_sChannels.begin();

	(*v_sChannel)->GetRows(p_iStart, p_iCount, v_prValues);
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		p_psValues[v_iI].rR = v_prValues[v_iI];

	++v_sChannel;
	(*v_sChannel)->GetRows(p_iStart, p_iCount, v_prValues);
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		p_psValues[v_iI].rG = v_prValues[v_iI];

	++v_sChannel;
	(*v_sChannel)->GetRows(p_iStart, p_iCount, v_prValues);
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		p_psValues[v_iI].rB = v_prValues[v_iI];

	++v_sChannel;
	(*v_sChannel)->GetRows(p_iStart, p_iCount, v_prValues);
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		p_psValues[v_iI].rA = v_prValues[v_iI];

	delete [] v_prValues;

	return true;
}

bool mmImages::mmImage::SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel8 const p_psValues[]) {
	if(m_ePixelType != mmP8 || p_iStart > (m_iWidth * m_iHeight) || (p_iStart + p_iCount) > (m_iWidth * m_iHeight) || p_iCount == 0)
		return false;

	m_sChannels.front()->SetRows(p_iStart, p_iCount, &p_psValues->rI);

	if(m_psCallback != NULL)
		m_psCallback->OnImagePixelsChange(this);

	return true;
}

bool mmImages::mmImage::SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel24 const p_psValues[]) {
	if((m_ePixelType != mmP24 && m_ePixelType != mmP32) || p_iStart > (m_iWidth * m_iHeight) || (p_iStart + p_iCount) > (m_iWidth * m_iHeight) || p_iCount == 0)
		return false;

	mmReal * v_prValues = new mmReal[p_iCount * m_iWidth];
	std::list<mmLayer*>::iterator v_sChannel = m_sChannels.begin();

	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		v_prValues[v_iI] = p_psValues[v_iI].rR;
	(*v_sChannel)->SetRows(p_iStart, p_iCount, v_prValues);

	++v_sChannel;
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		v_prValues[v_iI] = p_psValues[v_iI].rG;
	(*v_sChannel)->SetRows(p_iStart, p_iCount, v_prValues);

	++v_sChannel;
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		v_prValues[v_iI] = p_psValues[v_iI].rB;
	(*v_sChannel)->SetRows(p_iStart, p_iCount, v_prValues);

	delete [] v_prValues;

	if(m_psCallback != NULL)
		m_psCallback->OnImagePixelsChange(this);

	return true;
}

bool mmImages::mmImage::SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel32 const p_psValues[]) {
	if(m_ePixelType != mmP32 || p_iStart > (m_iWidth * m_iHeight) || (p_iStart + p_iCount) > (m_iWidth * m_iHeight) || p_iCount == 0)
		return false;

	mmReal * v_prValues = new mmReal[p_iCount * m_iWidth];
	std::list<mmLayer*>::iterator v_sChannel = m_sChannels.begin();

	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		v_prValues[v_iI] = p_psValues[v_iI].rR;
	(*v_sChannel)->SetRows(p_iStart, p_iCount, v_prValues);

	++v_sChannel;
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		v_prValues[v_iI] = p_psValues[v_iI].rG;
	(*v_sChannel)->SetRows(p_iStart, p_iCount, v_prValues);

	++v_sChannel;
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		v_prValues[v_iI] = p_psValues[v_iI].rB;
	(*v_sChannel)->SetRows(p_iStart, p_iCount, v_prValues);

	++v_sChannel;
	for(mmUInt v_iI = 0; v_iI < p_iCount * m_iWidth; ++v_iI)
		v_prValues[v_iI] = p_psValues[v_iI].rA;
	(*v_sChannel)->SetRows(p_iStart, p_iCount, v_prValues);

	delete [] v_prValues;

	if(m_psCallback != NULL)
		m_psCallback->OnImagePixelsChange(this);

	return true;
}

bool mmImages::mmImage::GetPixels(mmRect const & p_sRect, mmPixel8 p_psValues[]) const {
	if(m_ePixelType != mmP8 || p_sRect.iLeft > m_iWidth || p_sRect.iTop > m_iHeight || (p_sRect.iLeft + p_sRect.iWidth) > m_iWidth || (p_sRect.iTop + p_sRect.iHeight) > m_iHeight || p_sRect.iWidth * p_sRect.iHeight == 0)
		return false;

	m_sChannels.front()->GetPixels(p_sRect, &p_psValues->rI);

	return true;
}

bool mmImages::mmImage::GetPixels(mmRect const & p_sRect, mmPixel24 p_psValues[]) const {
	if((m_ePixelType != mmP24 && m_ePixelType != mmP32) || p_sRect.iLeft > m_iWidth || p_sRect.iTop > m_iHeight || (p_sRect.iLeft + p_sRect.iWidth) > m_iWidth || (p_sRect.iTop + p_sRect.iHeight) > m_iHeight || p_sRect.iWidth * p_sRect.iHeight == 0)
		return false;

	mmReal * v_prValues = new mmReal[p_sRect.iWidth * p_sRect.iHeight];
	std::list<mmLayer*>::const_iterator v_sChannel = m_sChannels.begin();

	(*v_sChannel)->GetPixels(p_sRect, v_prValues);
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			p_psValues[v_iRow * p_sRect.iWidth + v_iPixel].rR = v_prValues[v_iRow * p_sRect.iWidth + v_iPixel];

	++v_sChannel;
	(*v_sChannel)->GetPixels(p_sRect, v_prValues);
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			p_psValues[v_iRow * p_sRect.iWidth + v_iPixel].rG = v_prValues[v_iRow * p_sRect.iWidth + v_iPixel];

	++v_sChannel;
	(*v_sChannel)->GetPixels(p_sRect, v_prValues);
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			p_psValues[v_iRow * p_sRect.iWidth + v_iPixel].rB = v_prValues[v_iRow * p_sRect.iWidth + v_iPixel];

	delete [] v_prValues;

	return true;
}

bool mmImages::mmImage::GetPixels(mmRect const & p_sRect, mmPixel32 p_psValues[]) const {
	if(m_ePixelType != mmP32 || p_sRect.iLeft > m_iWidth || p_sRect.iTop > m_iHeight || (p_sRect.iLeft + p_sRect.iWidth) > m_iWidth || (p_sRect.iTop + p_sRect.iHeight) > m_iHeight || p_sRect.iWidth * p_sRect.iHeight == 0)
		return false;

	mmReal * v_prValues = new mmReal[p_sRect.iWidth * p_sRect.iHeight];
	std::list<mmLayer*>::const_iterator v_sChannel = m_sChannels.begin();

	(*v_sChannel)->GetPixels(p_sRect, v_prValues);
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			p_psValues[v_iRow * p_sRect.iWidth + v_iPixel].rR = v_prValues[v_iRow * p_sRect.iWidth + v_iPixel];

	++v_sChannel;
	(*v_sChannel)->GetPixels(p_sRect, v_prValues);
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			p_psValues[v_iRow * p_sRect.iWidth + v_iPixel].rG = v_prValues[v_iRow * p_sRect.iWidth + v_iPixel];

	++v_sChannel;
	(*v_sChannel)->GetPixels(p_sRect, v_prValues);
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			p_psValues[v_iRow * p_sRect.iWidth + v_iPixel].rB = v_prValues[v_iRow * p_sRect.iWidth + v_iPixel];

	++v_sChannel;
	(*v_sChannel)->GetPixels(p_sRect, v_prValues);
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			p_psValues[v_iRow * p_sRect.iWidth + v_iPixel].rA = v_prValues[v_iRow * p_sRect.iWidth + v_iPixel];

	delete [] v_prValues;

	return true;
}

bool mmImages::mmImage::SetPixels(mmRect const & p_sRect, mmPixel8 const p_psValues[]) {
	if(m_ePixelType != mmP8 || p_sRect.iLeft > m_iWidth || p_sRect.iTop > m_iHeight || (p_sRect.iLeft + p_sRect.iWidth) > m_iWidth || (p_sRect.iTop + p_sRect.iHeight) > m_iHeight || p_sRect.iWidth * p_sRect.iHeight == 0)
		return false;

	m_sChannels.front()->SetPixels(p_sRect, &p_psValues->rI);

	if(m_psCallback != NULL)
		m_psCallback->OnImagePixelsChange(this);

	return true;
}

bool mmImages::mmImage::SetPixels(mmRect const & p_sRect, mmPixel24 const p_psValues[]) {
	if((m_ePixelType != mmP24 && m_ePixelType != mmP32) || p_sRect.iLeft > m_iWidth || p_sRect.iTop > m_iHeight || (p_sRect.iLeft + p_sRect.iWidth) > m_iWidth || (p_sRect.iTop + p_sRect.iHeight) > m_iHeight || p_sRect.iWidth * p_sRect.iHeight == 0)
		return false;

	mmReal * v_prValues = new mmReal[p_sRect.iWidth * p_sRect.iHeight];
	std::list<mmLayer*>::iterator v_sChannel = m_sChannels.begin();

	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			v_prValues[v_iRow * m_iWidth + v_iPixel] = p_psValues[v_iRow * m_iWidth + v_iPixel].rR;
	(*v_sChannel)->SetPixels(p_sRect, v_prValues);

	++v_sChannel;
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			v_prValues[v_iRow * m_iWidth + v_iPixel] = p_psValues[v_iRow * m_iWidth + v_iPixel].rG;
	(*v_sChannel)->SetPixels(p_sRect, v_prValues);

	++v_sChannel;
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			v_prValues[v_iRow * m_iWidth + v_iPixel] = p_psValues[v_iRow * m_iWidth + v_iPixel].rB;
	(*v_sChannel)->SetPixels(p_sRect, v_prValues);

	delete [] v_prValues;

	if(m_psCallback != NULL)
		m_psCallback->OnImagePixelsChange(this);

	return true;
}

bool mmImages::mmImage::SetPixels(mmRect const & p_sRect, mmPixel32 const p_psValues[]) {
	if(m_ePixelType != mmP32 || p_sRect.iLeft > m_iWidth || p_sRect.iTop > m_iHeight || (p_sRect.iLeft + p_sRect.iWidth) > m_iWidth || (p_sRect.iTop + p_sRect.iHeight) > m_iHeight || p_sRect.iWidth * p_sRect.iHeight == 0)
		return false;

	mmReal * v_prValues = new mmReal[p_sRect.iWidth * p_sRect.iHeight];
	std::list<mmLayer*>::iterator v_sChannel = m_sChannels.begin();

	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			v_prValues[v_iRow * m_iWidth + v_iPixel] = p_psValues[v_iRow * m_iWidth + v_iPixel].rR;
	(*v_sChannel)->SetPixels(p_sRect, v_prValues);

	++v_sChannel;
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			v_prValues[v_iRow * m_iWidth + v_iPixel] = p_psValues[v_iRow * m_iWidth + v_iPixel].rG;
	(*v_sChannel)->SetPixels(p_sRect, v_prValues);

	++v_sChannel;
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			v_prValues[v_iRow * m_iWidth + v_iPixel] = p_psValues[v_iRow * m_iWidth + v_iPixel].rB;
	(*v_sChannel)->SetPixels(p_sRect, v_prValues);

	++v_sChannel;
	for(mmUInt v_iRow = 0; v_iRow < p_sRect.iHeight; ++v_iRow)
		for(mmUInt v_iPixel = 0; v_iPixel < p_sRect.iWidth; ++v_iPixel)
			v_prValues[v_iRow * m_iWidth + v_iPixel] = p_psValues[v_iRow * m_iWidth + v_iPixel].rA;
	(*v_sChannel)->SetPixels(p_sRect, v_prValues);

	delete [] v_prValues;

	if(m_psCallback != NULL)
		m_psCallback->OnImagePixelsChange(this);

	return true;
}

void mmImages::mmImage::SetRegionOfInterest(mmRect const & p_sRegion) {
	m_sRegionOfInterest = p_sRegion;

	m_sRegionOfInterest.iLeft = std::min(m_sRegionOfInterest.iLeft, m_iWidth);
	m_sRegionOfInterest.iTop = std::min(m_sRegionOfInterest.iTop, m_iHeight);
	m_sRegionOfInterest.iWidth = std::min(m_sRegionOfInterest.iWidth, m_iWidth - m_sRegionOfInterest.iLeft);
	m_sRegionOfInterest.iHeight = std::min(m_sRegionOfInterest.iHeight, m_iHeight - m_sRegionOfInterest.iTop);

	if(m_psCallback != NULL)
		m_psCallback->OnImagePropertiesChange(this);
}

mmRect mmImages::mmImage::GetRegionOfInterest(void) const {
	return m_sRegionOfInterest;
}

mmImages::mmLayerI* mmImages::mmImage::CreateLayer(mmString const & p_sName, mmReal const p_rDefaultValue) {
	m_sLayers.push_back(new mmLayer(mmLayerI::MakeID(m_sID, ++m_sLastLayerID), p_sName, m_iWidth, m_iHeight, p_rDefaultValue, this));
	return m_sLayers.back();
}

mmUInt mmImages::mmImage::GetLayerCount(void) const {
	return static_cast<mmUInt>(m_sLayers.size());
}

mmImages::mmLayerI * mmImages::mmImage::GetLayer(mmID const & p_sID) const {
	mmImages::mmLayerI::sID v_sLayerID = mmImages::mmLayerI::MakeID(m_sID, p_sID);
	std::list<mmLayer*>::const_iterator v_sLayer = std::find_if(m_sLayers.begin(), m_sLayers.end(), FindByID(v_sLayerID));
	if(v_sLayer != m_sLayers.end())
		return *v_sLayer;
	v_sLayer = std::find_if(m_sChannels.begin(), m_sChannels.end(), FindByID(v_sLayerID));
	if(v_sLayer != m_sChannels.end())
		return *v_sLayer;
	return NULL;
}

mmImages::mmLayerI * mmImages::mmImage::FindLayer(mmLayerI const * const p_psPreviousLayer, mmString const & p_sName) const {
	std::list<mmLayer*>::const_iterator v_sLayer;

	// set iterator to next layer or first layer if NULL is passed
	if(p_psPreviousLayer == NULL)
		v_sLayer = m_sLayers.begin();
	else if((v_sLayer = std::find(m_sLayers.begin(), m_sLayers.end(), p_psPreviousLayer)) != m_sLayers.end())
		++v_sLayer;
		
	// find first layer of given name if provided
	if(p_sName != L"")
		v_sLayer = std::find_if(v_sLayer, m_sLayers.end(), FindByName(p_sName));

	if(v_sLayer != m_sLayers.end())
		return *v_sLayer;
	else
		return NULL;
}

bool mmImages::mmImage::DeleteLayer(mmID const & p_sID) {
	mmImages::mmLayerI::sID v_sLayerID = mmImages::mmLayerI::MakeID(m_sID, p_sID);
	std::list<mmLayer*>::iterator v_sLayer = std::find_if(m_sLayers.begin(), m_sLayers.end(), FindByID(v_sLayerID));
	if(v_sLayer != m_sLayers.end()) {
		delete *v_sLayer;
		m_sLayers.erase(v_sLayer);
		return true;
	} else
		return false;
}

void mmImages::mmImage::SetMetadata(mmString const & p_sMetadata) {
	m_sMetadata = p_sMetadata;
}

mmString mmImages::mmImage::GetMetadata(void) {
	return m_sMetadata;
}

void mmImages::mmImage::OnLayerCreate(mmLayerI * p_psLayer) {
	if(m_psCallback != NULL)
		m_psCallback->OnLayerCreate(this, p_psLayer);
}

void mmImages::mmImage::OnLayerDestroy(mmLayerI * p_psLayer) {
	if(m_psCallback != NULL)
		m_psCallback->OnLayerDestroy(this, p_psLayer);
}

void mmImages::mmImage::OnLayerPropertiesChange(mmLayerI * p_psLayer) {
	if(m_psCallback != NULL)
		m_psCallback->OnLayerPropertiesChange(this, p_psLayer);
}

void mmImages::mmImage::OnLayerValuesChange(mmLayerI * p_psLayer) {
	if(m_psCallback != NULL)
		m_psCallback->OnLayerValuesChange(this, p_psLayer);
}

mmImages::mmImageStructure::mmImageStructure(mmImageI::mmCallbackI * const p_psCallback) :
	m_psCallback(p_psCallback)
{}

mmImages::mmImageStructure::~mmImageStructure(void) {
	for (auto it = m_sImages.begin(); it != m_sImages.end(); delete *it++);
}

mmImages::mmImageI* mmImages::mmImageStructure::CreateImage(mmString const & p_sName, mmUInt const p_iWidth, mmUInt const p_iHeight, mmImageI::mmPixelType const p_ePixelType) {
	m_sImages.push_back(new mmImage(++m_sLastImageID, p_sName, p_iWidth, p_iHeight, p_ePixelType, m_psCallback));
	return m_sImages.back();
}

mmUInt mmImages::mmImageStructure::GetImageCount(void) const {
	return static_cast<mmUInt>(m_sImages.size());
}

mmImages::mmImageI * mmImages::mmImageStructure::GetImage(mmID const & p_sID) const {
	std::list<mmImage*>::const_iterator v_sImage = std::find_if(m_sImages.begin(), m_sImages.end(), FindByID(p_sID));
	if(v_sImage != m_sImages.end())
		return *v_sImage;
	else
		return NULL;
}

mmImages::mmImageI * mmImages::mmImageStructure::FindImage(mmImageI const * const p_psPreviousImage, mmString const & p_sName) const {
	std::list<mmImage*>::const_iterator v_sImage;

	// set iterator to next image or first image if NULL is passed
	if(p_psPreviousImage == NULL)
		v_sImage = m_sImages.begin();
	else if((v_sImage = std::find(m_sImages.begin(), m_sImages.end(), p_psPreviousImage)) != m_sImages.end())
		++v_sImage;
		
	// find first image of given name if provided
	if(p_sName != L"")
		v_sImage = std::find_if(v_sImage, m_sImages.end(), FindByName(p_sName));

	if(v_sImage != m_sImages.end())
		return *v_sImage;
	else
		return NULL;
}

bool mmImages::mmImageStructure::DeleteImage(mmID const & p_sID) {
	std::list<mmImage*>::iterator v_sImage = std::find_if(m_sImages.begin(), m_sImages.end(), FindByID(p_sID));
	if(v_sImage != m_sImages.end()) {
		delete *v_sImage;
		m_sImages.erase(v_sImage);
		return true;
	} else
		return false;
}
