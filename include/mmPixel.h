#ifndef mmPixelH
#define mmPixelH

#include <mmGlobalDefs.h>

struct mmPixel8;
struct mmPixel24;
struct mmPixel32;

struct mmPixel8 {
	mmPixel8(void);
	mmPixel8(mmReal const p_rI);

	static mmUInt const iComponents;

	mmReal rI; 
};

struct mmPixel24 {
	mmPixel24(void);
	mmPixel24(mmReal const p_rR, mmReal const p_rG, mmReal const p_rB);
	mmPixel24(mmPixel8 const & p_smmPixel8);

	static mmUInt const iComponents;

	mmReal rR, rG, rB;
};

struct mmPixel32 {
	mmPixel32(void);
	mmPixel32(mmReal const p_rR, mmReal const p_rG, mmReal const p_rB, mmReal const p_rA);
	mmPixel32(mmPixel8 const & p_smmPixel8);
	mmPixel32(mmPixel24 const & p_smmPixel24);

	static mmUInt const iComponents;

	mmReal rR, rG, rB, rA;
};

extern mmPixel8 operator + (mmPixel8 p_sP1, mmPixel8 const & p_sP2);
extern mmPixel8 operator - (mmPixel8 p_sP1, mmPixel8 const & p_sP2);
extern mmPixel8 operator += (mmPixel8 & p_sP1, mmPixel8 const & p_sP2);
extern mmPixel8 operator -= (mmPixel8 & p_sP1, mmPixel8 const & p_sP2);

extern mmPixel8 operator * (mmPixel8 p_sP, mmReal const p_sS);
extern mmPixel8 operator * (mmReal const p_sS, mmPixel8 p_sP);
extern mmPixel8 operator / (mmPixel8 p_sP, mmReal const p_sS);
extern mmPixel8 operator *= (mmPixel8 & p_sP, mmReal const p_sS);
extern mmPixel8 operator *= (mmReal const p_sS, mmPixel8 & p_sP);
extern mmPixel8 operator /= (mmPixel8 & p_sP, mmReal const p_sS);

extern mmPixel24 operator + (mmPixel24 p_sP1, mmPixel24 const & p_sP2);
extern mmPixel24 operator - (mmPixel24 p_sP1, mmPixel24 const & p_sP2);
extern mmPixel24 operator += (mmPixel24 & p_sP1, mmPixel24 const & p_sP2);
extern mmPixel24 operator -= (mmPixel24 & p_sP1, mmPixel24 const & p_sP2);

extern mmPixel24 operator * (mmPixel24 p_sP, mmReal const p_sS);
extern mmPixel24 operator * (mmReal const p_sS, mmPixel24 p_sP);
extern mmPixel24 operator / (mmPixel24 p_sP, mmReal const p_sS);
extern mmPixel24 operator *= (mmPixel24 & p_sP, mmReal const p_sS);
extern mmPixel24 operator *= (mmReal const p_sS, mmPixel24 & p_sP);
extern mmPixel24 operator /= (mmPixel24 & p_sP, mmReal const p_sS);

extern mmPixel32 operator + (mmPixel32 p_sP1, mmPixel32 const & p_sP2);
extern mmPixel32 operator - (mmPixel32 p_sP1, mmPixel32 const & p_sP2);
extern mmPixel32 operator += (mmPixel32 & p_sP1, mmPixel32 const & p_sP2);
extern mmPixel32 operator -= (mmPixel32 & p_sP1, mmPixel32 const & p_sP2);

extern mmPixel32 operator * (mmPixel32 p_sP, mmReal const p_sS);
extern mmPixel32 operator * (mmReal const p_sS, mmPixel32 p_sP);
extern mmPixel32 operator / (mmPixel32 p_sP, mmReal const p_sS);
extern mmPixel32 operator *= (mmPixel32 & p_sP, mmReal const p_sS);
extern mmPixel32 operator *= (mmReal const p_sS, mmPixel32 & p_sP);
extern mmPixel32 operator /= (mmPixel32 & p_sP, mmReal const p_sS);

#endif