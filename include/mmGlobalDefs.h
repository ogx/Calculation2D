//******************************************************************************
//******************************************************************************
//
//
//
//******************************************************************************
//******************************************************************************
#ifndef mmGlobalDefsH
#define mmGlobalDefsH

#include <string>

////////////////////////////////////////////////////////////////////////////////
/// Definition of type representing real value
////////////////////////////////////////////////////////////////////////////////
typedef 	double						mmReal;

////////////////////////////////////////////////////////////////////////////////
/// Definition of type representing real(float) value
////////////////////////////////////////////////////////////////////////////////
typedef 	float							mmFloat;

////////////////////////////////////////////////////////////////////////////////
/// Definition of type representing integer value
////////////////////////////////////////////////////////////////////////////////
typedef		int								mmInt;

typedef		unsigned						mmUInt;

class mmID { 
public:
	mmID(void) : iID(0) {}
	explicit mmID(mmUInt const p_iID) : iID(p_iID) {} 
	mmID operator ++ (void) {++iID; return *this; }
	bool operator == (mmID const & p_sID) const { return iID == p_sID.iID; }
	bool operator != (mmID const & p_sID) const { return iID != p_sID.iID; }
private:
	mmUInt iID; 
};

struct mmRect { 
	mmRect(void) : iLeft(0), iTop(0), iWidth(0), iHeight(0) {}
	mmRect(mmUInt const p_iLeft, mmUInt const p_iTop, mmUInt const p_iWidth, mmUInt const p_iHeight) : iLeft(p_iLeft), iTop(p_iTop), iWidth(p_iWidth), iHeight(p_iHeight) {}
	mmUInt iLeft;
	mmUInt iTop;
	mmUInt iWidth;
	mmUInt iHeight; 
};

////////////////////////////////////////////////////////////////////////////////
/// Definition of type representing std::string value
////////////////////////////////////////////////////////////////////////////////
typedef		std::wstring			mmString;

#endif
