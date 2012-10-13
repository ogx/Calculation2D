#ifndef types_mmRect_H_
#define types_mmRect_H_

#include <types/mmBasic.h>

struct mmRect { 
	mmRect(void) : iLeft(0), iTop(0), iWidth(0), iHeight(0) {}
	mmRect(mmUInt const p_iLeft, mmUInt const p_iTop, mmUInt const p_iWidth, mmUInt const p_iHeight) : iLeft(p_iLeft), iTop(p_iTop), iWidth(p_iWidth), iHeight(p_iHeight) {}
	mmUInt GetSize() const {return iWidth * iHeight;}
	mmUInt iLeft;
	mmUInt iTop;
	mmUInt iWidth;
	mmUInt iHeight; 
};

#endif