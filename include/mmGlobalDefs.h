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

struct mmRect { 
	mmRect(void) : iLeft(0), iTop(0), iWidth(0), iHeight(0) {}
	mmRect(mmUInt const p_iLeft, mmUInt const p_iTop, mmUInt const p_iWidth, mmUInt const p_iHeight) : iLeft(p_iLeft), iTop(p_iTop), iWidth(p_iWidth), iHeight(p_iHeight) {}
	mmUInt GetSize() const {return iWidth * iHeight;}
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
