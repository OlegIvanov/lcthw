#ifndef __ex23_h__
#define __ex23_h__

#include <math.h>

#ifndef DL

#define	DL		8

#endif

#define H		DL - 1

#define X0(H)		case (int)(H): *to++ = *from++;
#define X1(H)		X0(H) X0(H - powl(2, 0))
#define X2(H) 		X1(H) X1(H - powl(2, 1))
#define X3(H) 		X2(H) X2(H - powl(2, 2))
#define X4(H) 		X3(H) X3(H - powl(2, 3))
#define X5(H) 		X4(H) X4(H - powl(2, 4))
#define X6(H) 		X5(H) X5(H - powl(2, 5))
#define X7(H) 		X6(H) X6(H - powl(2, 6))

#define A0		DL / powl(2, 0) - 1
#define A1		DL / powl(2, 1) - 1
#define A2		DL / powl(2, 2) - 1
#define A3		DL / powl(2, 3) - 1
#define A4		DL / powl(2, 4) - 1
#define A5		DL / powl(2, 5) - 1
#define A6		DL / powl(2, 6) - 1
#define A7		DL / powl(2, 7) - 1

#define DC2		X0(A0)
#define DC4		X1(A0) X0(A1)
#define DC8		X2(A0) X1(A1) X0(A2)
#define DC16		X3(A0) X2(A1) X1(A2) X0(A3)
#define DC32		X4(A0) X3(A1) X2(A2) X1(A3) X0(A4)
#define DC64		X5(A0) X4(A1) X3(A2) X2(A3) X1(A4) X0(A5)
#define DC128		X6(A0) X5(A1) X4(A2) X3(A3) X2(A4) X1(A5) X0(A6)
#define DC256		X7(A0) X6(A1) X5(A2) X4(A3) X3(A4) X2(A5) X1(A6) X0(A7)

#define DCHH(X)		DC ## X

#define DCH(X)		DCHH(X)	

#define DUFFS_CASES	DCH(DL)

#endif
