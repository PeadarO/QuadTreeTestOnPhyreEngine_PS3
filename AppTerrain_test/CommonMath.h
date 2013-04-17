#pragma once

#include <math.h>

// @ literal
#define EPSILON_8		(1.e-8f)
#define EPSILON_6		(1.e-6f)
#define EPSILON_4		(1.e-4f)
#define EPSILON_2		(1.e-2f)
#define EPSILON_1		(1.e-1f)

#define APPROX_ZERO(x)			((fabs(x)<= EPSILON_4)?true:false)
#define APPROX_EQ(x,v)			(APPROX_ZERO((x)-(v)))

#define INFINITE32				(unsigned int)(1<<30)
#define INFINITE32_UNSIGNED		(unsigned int)(1<<31)
#define INFINITE32_FLOAT		(float)(unsigned int)(1<<29)

#define INFINITE16				(unsigned int)(1<<14)
#define INFINITE16_UNSIGNED		(unsigned int)(1<<15)

#define ACCELERATOR_G				(9.8f)

// @ Signiture
#define INSIDE		0
#define OUTSIDE		1


// @ Simple Calculate
template<typename T>
inline void Swap_(T &a,T &b) { T c = a;a = b;b = c; }

#define ABS(x)	((x)<0?(-x):(x))
#define MAX_FLOAT	FLT_MAX
#define MAX_INT		INT_MAX

#define LERP( A, B, C) ((B)+(A)*((C)-(B)))
#define MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))


#define EPS 10e-5
#define SIGN3( A ) \
	(((A).getX() < EPS) ? 4 : 0 | ((A).getX() > -EPS) ? 32 : 0 | \
	((A).getY() < EPS) ? 2 : 0 | ((A).getY() > -EPS) ? 16 : 0 | \
	((A).getZ() < EPS) ? 1 : 0 | ((A).getZ() > -EPS) ? 8 : 0)


// @ Simple Vector
#define APPROX_EQ_V3(v301,v302)		(APPROX_EQ(v301.getX(),v302.getX())\
									&& APPROX_EQ(v301.getY(),v302.getY())\
									&& APPROX_EQ(v301.getZ(),v302.getZ()))

#define EQ_V3(v301,v302)	((v301.getX()==v302.getX()\
							&& v301.getY()==v302.getY()\
							&& v301.getZ()==v302.getZ()))


#define vectorCopy4to3(dst, src) \
						{	dst.setX(src.getX()); \
							dst.setY(src.getY()); \
							dst.setZ(src.getZ()); }
#define vectorCopy3to4(dst, src, w) \
						{	dst.setX(src.getX()); \
							dst.setY(src.getY()); \
							dst.setZ(src.getZ()); \
							dst.setW(w); }
#define setvector3(dst,a,b,c) {dst.setX(a); dst.setY(b); dst.setZ(c);}
