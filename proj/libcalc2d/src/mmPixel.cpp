#include <mmPixel.h>

mmUInt const mmPixel8::iComponents = 1;
mmPixel8::mmPixel8(void) : rI(0.0) {}
mmPixel8::mmPixel8(mmReal const p_rI) : rI(p_rI) {}

mmUInt const mmPixel24::iComponents = 3;
mmPixel24::mmPixel24(void) : rR(0.0), rG(0.0), rB(0.0) {}
mmPixel24::mmPixel24(mmReal const p_rR, mmReal const p_rG, mmReal const p_rB) : rR(p_rR), rG(p_rG), rB(p_rB) {}
mmPixel24::mmPixel24(mmPixel8 const & p_smmPixel8) : rR(p_smmPixel8.rI), rG(p_smmPixel8.rI), rB(p_smmPixel8.rI) {}

mmUInt const mmPixel32::iComponents = 4;
mmPixel32::mmPixel32(void) : rR(0.0), rG(0.0), rB(0.0), rA(0.0) {}
mmPixel32::mmPixel32(mmReal const p_rR, mmReal const p_rG, mmReal const p_rB, mmReal const p_rA) : rR(p_rR), rG(p_rG), rB(p_rB), rA(p_rA) {}
mmPixel32::mmPixel32(mmPixel8 const & p_smmPixel8) : rR(p_smmPixel8.rI), rG(p_smmPixel8.rI), rB(p_smmPixel8.rI), rA(0.0) {}
mmPixel32::mmPixel32(mmPixel24 const & p_smmPixel24) : rR(p_smmPixel24.rR), rG(p_smmPixel24.rG), rB(p_smmPixel24.rB), rA(0.0) {}

inline mmPixel8 operator + (mmPixel8 p_sP1, mmPixel8 const & p_sP2) {
	p_sP1.rI += p_sP2.rI;

	return p_sP1;
}

inline mmPixel8 operator - (mmPixel8 p_sP1, mmPixel8 const & p_sP2) {
	p_sP1.rI -= p_sP2.rI;

	return p_sP1;
}

inline mmPixel8 operator += (mmPixel8 & p_sP1, mmPixel8 const & p_sP2) {
	p_sP1.rI += p_sP2.rI;

	return p_sP1;
}

inline mmPixel8 operator -= (mmPixel8 & p_sP1, mmPixel8 const & p_sP2) {
	p_sP1.rI -= p_sP2.rI;

	return p_sP1;
}

inline mmPixel8 operator * (mmPixel8 p_sP, mmReal const p_rS) {
	p_sP.rI *= p_rS;

	return p_sP;
}

inline mmPixel8 operator * (mmReal const p_rS, mmPixel8 p_sP) {
	p_sP.rI *= p_rS;

	return p_sP;
}

inline mmPixel8 operator / (mmPixel8 p_sP, mmReal const p_rS) {
	p_sP.rI /= p_rS;

	return p_sP;
}

inline mmPixel8 operator *= (mmPixel8 & p_sP, mmReal const p_rS) {
	p_sP.rI *= p_rS;

	return p_sP;
}

inline mmPixel8 operator *= (mmReal const p_rS, mmPixel8 & p_sP) {
	p_sP.rI *= p_rS;

	return p_sP;
}

inline mmPixel8 operator /= (mmPixel8 & p_sP, mmReal const p_rS) {
	p_sP.rI /= p_rS;

	return p_sP;
}

inline mmPixel24 operator + (mmPixel24 p_sP1, mmPixel24 const & p_sP2) {
	p_sP1.rR += p_sP2.rR;
	p_sP1.rG += p_sP2.rG;
	p_sP1.rB += p_sP2.rB;

	return p_sP1;
}

inline mmPixel24 operator - (mmPixel24 p_sP1, mmPixel24 const & p_sP2) {
	p_sP1.rR -= p_sP2.rR;
	p_sP1.rG -= p_sP2.rG;
	p_sP1.rB -= p_sP2.rB;

	return p_sP1;
}

inline mmPixel24 operator += (mmPixel24 & p_sP1, mmPixel24 const & p_sP2) {
	p_sP1.rR += p_sP2.rR;
	p_sP1.rG += p_sP2.rG;
	p_sP1.rB += p_sP2.rB;

	return p_sP1;
}

inline mmPixel24 operator -= (mmPixel24 & p_sP1, mmPixel24 const & p_sP2) {
	p_sP1.rR -= p_sP2.rR;
	p_sP1.rG -= p_sP2.rG;
	p_sP1.rB -= p_sP2.rB;

	return p_sP1;
}

inline mmPixel24 operator * (mmPixel24 p_sP, mmReal const p_rS) {
	p_sP.rR *= p_rS;
	p_sP.rG *= p_rS;
	p_sP.rB *= p_rS;

	return p_sP;
}

inline mmPixel24 operator * (mmReal const p_rS, mmPixel24 p_sP) {
	p_sP.rR *= p_rS;
	p_sP.rG *= p_rS;
	p_sP.rB *= p_rS;

	return p_sP;
}

inline mmPixel24 operator / (mmPixel24 p_sP, mmReal const p_rS) {
	p_sP.rR /= p_rS;
	p_sP.rG /= p_rS;
	p_sP.rB /= p_rS;

	return p_sP;
}

inline mmPixel24 operator *= (mmPixel24 & p_sP, mmReal const p_rS) {
	p_sP.rR *= p_rS;
	p_sP.rG *= p_rS;
	p_sP.rB *= p_rS;

	return p_sP;
}

inline mmPixel24 operator *= (mmReal const p_rS, mmPixel24 & p_sP) {
	p_sP.rR *= p_rS;
	p_sP.rG *= p_rS;
	p_sP.rB *= p_rS;

	return p_sP;
}

inline mmPixel24 operator /= (mmPixel24 & p_sP, mmReal const p_rS) {
	p_sP.rR /= p_rS;
	p_sP.rG /= p_rS;
	p_sP.rB /= p_rS;

	return p_sP;
}

inline mmPixel32 operator + (mmPixel32 p_sP1, mmPixel32 const & p_sP2) {
	p_sP1.rR += p_sP2.rR;
	p_sP1.rG += p_sP2.rG;
	p_sP1.rB += p_sP2.rB;
	p_sP1.rA += p_sP2.rA;

	return p_sP1;
}

inline mmPixel32 operator - (mmPixel32 p_sP1, mmPixel32 const & p_sP2) {
	p_sP1.rR -= p_sP2.rR;
	p_sP1.rG -= p_sP2.rG;
	p_sP1.rB -= p_sP2.rB;
	p_sP1.rA -= p_sP2.rA;

	return p_sP1;
}

inline mmPixel32 operator += (mmPixel32 & p_sP1, mmPixel32 const & p_sP2) {
	p_sP1.rR += p_sP2.rR;
	p_sP1.rG += p_sP2.rG;
	p_sP1.rB += p_sP2.rB;
	p_sP1.rA += p_sP2.rA;

	return p_sP1;
}

inline mmPixel32 operator -= (mmPixel32 & p_sP1, mmPixel32 const & p_sP2) {
	p_sP1.rR -= p_sP2.rR;
	p_sP1.rG -= p_sP2.rG;
	p_sP1.rB -= p_sP2.rB;
	p_sP1.rA -= p_sP2.rA;

	return p_sP1;
}

inline mmPixel32 operator * (mmPixel32 p_sP, mmReal const p_rS) {
	p_sP.rR *= p_rS;
	p_sP.rG *= p_rS;
	p_sP.rB *= p_rS;
	p_sP.rA *= p_rS;

	return p_sP;
}

inline mmPixel32 operator * (mmReal const p_rS, mmPixel32 p_sP) {
	p_sP.rR *= p_rS;
	p_sP.rG *= p_rS;
	p_sP.rB *= p_rS;
	p_sP.rA *= p_rS;

	return p_sP;
}

inline mmPixel32 operator / (mmPixel32 p_sP, mmReal const p_rS) {
	p_sP.rR /= p_rS;
	p_sP.rG /= p_rS;
	p_sP.rB /= p_rS;
	p_sP.rA /= p_rS;

	return p_sP;
}

inline mmPixel32 operator *= (mmPixel32 & p_sP, mmReal const p_rS) {
	p_sP.rR *= p_rS;
	p_sP.rG *= p_rS;
	p_sP.rB *= p_rS;
	p_sP.rA *= p_rS;

	return p_sP;
}

inline mmPixel32 operator *= (mmReal const p_rS, mmPixel32 & p_sP) {
	p_sP.rR *= p_rS;
	p_sP.rG *= p_rS;
	p_sP.rB *= p_rS;
	p_sP.rA *= p_rS;

	return p_sP;
}

inline mmPixel32 operator /= (mmPixel32 & p_sP, mmReal const p_rS) {
	p_sP.rR /= p_rS;
	p_sP.rG /= p_rS;
	p_sP.rB /= p_rS;
	p_sP.rA /= p_rS;

	return p_sP;
}