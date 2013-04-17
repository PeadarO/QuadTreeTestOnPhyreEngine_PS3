#include "BoundaryVolume.h"

namespace BV
{
// 광선 - AABB : 충돌 검사
bool IntersectInfiniteRayAABB( const _Ray &Ray_, const _BV_AABB_Sphere &bvAABB_ )
{	
	float flengthRay = length(Ray_._v3Direction);

	if( ABS(1.0f-flengthRay) >= EPSILON_4 ) 
	{
#ifdef _DEBUG 
		char	szDebug[256];
		float fX, fY, fZ;

		fX = Ray_._v3Direction.getX();
		fY = Ray_._v3Direction.getY();
		fZ = Ray_._v3Direction.getZ();

		sprintf( szDebug, "Not Normalize : x(%f), y(%f), z(%f) : Len(%f)\n", 
			fX, fY, fZ,	flengthRay ); 

#ifdef APPTARGET_IS_WIN
		OutputDebugString( szDebug );
#endif // APPTARGET_IS_WIN

#endif // #ifdef _DEBUG

		return false;
	}

#ifdef _DEBUG
	assert( bvAABB_._v3Max.getX() >= bvAABB_._v3Min.getX() && 
			bvAABB_._v3Max.getY() >= bvAABB_._v3Min.getY() && 
			bvAABB_._v3Max.getZ() >= bvAABB_._v3Min.getZ());
#endif // #ifdef _DEBUG

	float fT_maxofmin = -MAX_FLOAT;
	float fT_minofmax = MAX_FLOAT;

	Vector3 v3BvMax = bvAABB_._v3Max - Ray_._v3Origin;
	Vector3 v3BvMin = bvAABB_._v3Min - Ray_._v3Origin;

	// 3개 축에 대해 검사
	if ( fabs( Ray_._v3Direction.getX()) > EPSILON_4)
	{	// 광선과 slab 의 수평인지 체크
		float fT_min = v3BvMin.getX() / Ray_._v3Direction.getX();
		float fT_max = v3BvMax.getX() / Ray_._v3Direction.getX();
		// slab 에 대한 min, max 정하기
		if ( fT_min > fT_max)
		{
			Swap_( fT_min, fT_max);
		}
		// min, max 들의 max, min 정하기
		if ( fT_min > fT_maxofmin)
		{
			fT_maxofmin = fT_min;
		}
		if ( fT_max < fT_minofmax)
		{
			fT_minofmax = fT_max;
		}
		// 교차 검사
		if ( fT_maxofmin > fT_minofmax)
		{	// 박스의 바깥으로 지난다.
			return ( false);
		}
		if ( fT_minofmax < 0.0f)
		{	// 광선의 방향 뒤에서 만난다.
			return ( false);
		}
	}
	else
	{	// 광선과 slab 이 수평인 경우
		if ( v3BvMin.getX() > 0.0f || v3BvMax.getX() < 0.0f)
		{	// 광선이 slab 외부인지 검사한다.
			return ( false);
		}
	}

	if ( fabs( Ray_._v3Direction.getY()) > EPSILON_4)
	{	// 광선과 slab 의 수평인지 체크
		float fT_min = v3BvMin.getY() / Ray_._v3Direction.getY();
		float fT_max = v3BvMax.getY() / Ray_._v3Direction.getY();
		// slab 에 대한 min, max 정하기
		if ( fT_min > fT_max)
		{
			Swap_( fT_min, fT_max);
		}
		// min, max 들의 max, min 정하기
		if ( fT_min > fT_maxofmin)
		{
			fT_maxofmin = fT_min;
		}
		if ( fT_max < fT_minofmax)
		{
			fT_minofmax = fT_max;
		}
		// 교차 검사
		if ( fT_maxofmin > fT_minofmax)
		{	// 박스의 바깥으로 지난다.
			return ( false);
		}
		if ( fT_minofmax < 0.0f)
		{	// 광선의 방향 뒤에서 만난다.
			return ( false);
		}
	}
	else
	{	// 광선과 slab 이 수평인 경우
		if ( v3BvMin.getY() > 0.0f || v3BvMax.getY() < 0.0f)
		{	// 광선이 slab 외부인지 검사한다.
			return ( false);
		}
	}

	if ( fabs( Ray_._v3Direction.getZ()) > EPSILON_4)
	{	// 광선과 slab 의 수평인지 체크
		float fT_min = v3BvMin.getZ() / Ray_._v3Direction.getZ();
		float fT_max = v3BvMax.getZ() / Ray_._v3Direction.getZ();
// 		float fT_min = v3BvMin.z / Ray_._v3Direction.getZ();
// 		float fT_max = v3BvMax.z / Ray_._v3Direction.getZ();
		// slab 에 대한 min, max 정하기
		if ( fT_min > fT_max)
		{
			Swap_( fT_min, fT_max);
		}
		// min, max 들의 max, min 정하기
		if ( fT_min > fT_maxofmin)
		{
			fT_maxofmin = fT_min;
		}
		if ( fT_max < fT_minofmax)
		{
			fT_minofmax = fT_max;
		}
		// 교차 검사
		if ( fT_maxofmin > fT_minofmax)
		{	// 박스의 바깥으로 지난다.
			return ( false);
		}
		if ( fT_minofmax < 0.0f)
		{	// 광선의 방향 뒤에서 만난다.
			return ( false);
		}
	}
	else
	{	// 광선과 slab 이 수평인 경우
		if ( v3BvMin.getZ() > 0.0f || v3BvMax.getZ() < 0.0f)
//		if ( v3BvMin.z > 0.0f || v3BvMax.z < 0.0f)
		{	// 광선이 slab 외부인지 검사한다.
			return ( false);
		}
	}

	return ( true );
}

// 광선 - 구 : 충돌 검사
bool IntersectRaySphere( const _Ray &Ray_, const _BV_AABB_Sphere &BvSphere)
{
#ifdef _DEBUG
	assert( fabs( 1.0f - length(Ray_._v3Direction)) < EPSILON_4);
	assert( BvSphere._fRadius > 0.0f);
#endif // _DEBUG

	Vector3 v3DirBVtoRayorigin = BvSphere._v3Center - Ray_._v3Origin;

	float fDist_2	= lengthSqr(v3DirBVtoRayorigin);;
	float fRadius_2	= BvSphere._fRadius * BvSphere._fRadius;

	float fS		= dot(v3DirBVtoRayorigin,Ray_._v3Direction);

	if ( fS < 0.0f)
	{
		if ( fDist_2 > fRadius_2)
		{
			return ( false);
		}
		return ( true );	// t 를 얻을 필요가 없으면 여기서 return 해도 된다.
	}

	float fM2 = fDist_2 - fS * fS;
	if ( fM2 > fRadius_2)
	{
		return ( false );
	}

	return ( true);
}

bool IntersectRayBV(  const _Ray &Ray_, const _BV_AABB_Sphere &BvAABBSphere, ENUM_BV_TYPE eBvType )
{
	if( BV_AABB == eBvType )
	{
		return BV::IntersectInfiniteRayAABB( Ray_, BvAABBSphere );
	}
	else if( BV_SPHERE == eBvType )
	{
		return BV::IntersectRaySphere( Ray_, BvAABBSphere );
	}

	return true;
}

// 교차 검사 (외부/교차/포함) 교차지점 유
bool IntersectRayTriOriginDir( const Vector3& v3RayOrigin,
					 const Vector3& v3RayDir , 
					 const Vector3& v3T_1, 
					 const Vector3& v3T_2, 
					 const Vector3& v3T_3, 
					 float *pfT )
{
#ifdef _DEBUG
	float fNormalizeDiffer_debug = fabs(1.0f-length(v3RayDir));
	assert( fNormalizeDiffer_debug < EPSILON_4);
#endif // _DEBUG

	// e1 = v1 - v0, e2 = v2 - v0
	Vector3 v3Edge1 = v3T_2 - v3T_1;
	Vector3 v3Edge2 = v3T_3 - v3T_1;

	// 선분 두개가 완전 동일 하다면 바로 반환 (이런경우는 정말 거의 없겠지만)
	if(	v3Edge1.getX()==v3Edge2.getX() &&
		v3Edge1.getY()==v3Edge2.getY() &&
		v3Edge1.getZ()==v3Edge2.getZ() )
	{
		return ( false );
	}

	// 선분 길이가 0 인 경우
	if( length(v3Edge1)==0.0f || length(v3Edge2)== 0.0f )
	{
		return ( false );
	}


	// p = d x e2, q = s x e1 ( s = o - v0) : q 는 뒤에서 구함
	Vector3 v3Temp;

	v3Temp = cross(v3RayDir,v3Edge2);
	// det 구하기
	float fDet = dot( v3Edge1, v3Temp );

	float fInverseOfDet = 0.0f;
	Vector3 wvQ;

	if ( -EPSILON_4 < fDet && fDet < EPSILON_4)
	{

		return ( false);
	}
	fInverseOfDet = 1.0f / fDet;
	// s = o - v0
	Vector3 wvS = v3RayOrigin - v3T_1;
	// U 값 구하기

	float fU = dot( wvS, v3Temp) * fInverseOfDet;
	if ( fU < 0.0f || fU > 1.0f)
	{
		return ( false);
	}
	// q = s x e1

	wvQ = cross( wvS, v3Edge1 );

	// V 값 구하기
	float fV = dot( v3RayDir, wvQ) * fInverseOfDet;
	if ( fV < 0.0f || ( fU + fV) > 1.0f)
	{
		return ( false);
	}

	// Ray 의 시작점에서부터의 거리
	if ( 0 != pfT)
	{
		*pfT = dot( v3Edge2, wvQ) * fInverseOfDet;
	}

	return true;
}


// 교차 검사 (외부/교차/포함) 교차지점 무
bool IntersectRayTri( const _Ray& _ray,
					 const Vector3& v3T_1, 
					 const Vector3& v3T_2, 
					 const Vector3& v3T_3, 
					 float *pfT)
{
	return  IntersectRayTriOriginDir(	_ray._v3Origin, 
								_ray._v3Direction,
								v3T_1, 
								v3T_2, 
								v3T_3,
								pfT );
}

float FPointPlaneDist(	const Vector3 &Point,
						const Vector3 &PlaneBase,
						const Vector3 &PlaneNormal)
{
	return dot( (Point-PlaneBase), PlaneNormal );
}

Vector3 FLinePlaneIntersection(	const Vector3 &v3Start,
								const Vector3 &v3End,
								const Vector3 &PlaneAnyVertex,
								const Vector3 &PlaneNormal)
{
	return
		v3Start
		+	( (v3End-v3Start)
		*	( dot((PlaneAnyVertex - v3Start), PlaneNormal) 
		/ dot((v3End - v3Start), PlaneNormal)) );
}

bool IsPointOnPlane(	const Vector3 &v3PosCheckup, 
						const Vector3 *arv3Pos, 
						const int iCnt, 
						const Vector3 &v3PolygonNormal )
{
	Vector3  SidePlaneNormal;
	Vector3  Side;

	for( int x = 0 ; x < iCnt ; x++ )
	{
		// Create plane perpendicular to both this side and the polygon's normal.
		Side = arv3Pos[x] - arv3Pos[(x-1 < 0  ? iCnt-1 : x-1 )];

		SidePlaneNormal = cross( Side, v3PolygonNormal );
		SidePlaneNormal = normalize( SidePlaneNormal );

		// If point is not behind all the planes created by this polys edges, it's outside the poly.
		if( FPointPlaneDist( v3PosCheckup, arv3Pos[x], SidePlaneNormal ) > EPSILON_4 )
		{
			return 0;
		}
	}

	return 1;
}


bool IsLineIntersect_point( const Vector3& Start, 
						   const Vector3& End, 
						   const Vector3* arv3Pos, 
						   int iCnt,
						   const Vector3& v3PolygonNormal,
						   Vector3* Intersect )
{
	// 1단계 : Cross Over 판정
	// ray가 Plane을 교차하지 않았다면 검사 할 필요 없다.
	// If the ray doesn't cross the plane, don't bother going any further.
	const float DistStart	= FPointPlaneDist( Start, arv3Pos[0], v3PolygonNormal );
	const float DistEnd		= FPointPlaneDist( End, arv3Pos[0], v3PolygonNormal );

	if( (DistStart < 0.f && DistEnd < 0.f) || (DistStart > 0.f && DistEnd > 0.f ) )
	{
		return false;
	}

	// 2단계 : 평면 방정식을 이용한 평면 교차점 구하기
	// Get the intersection of the line and the plane.
	Vector3 Intersection = FLinePlaneIntersection(Start,End,arv3Pos[0],v3PolygonNormal);

	if( (	Intersection.getX() == Start.getX() && 
			Intersection.getY() == Start.getY() &&
			Intersection.getZ() == Start.getZ() ) ||
		(	Intersection.getX() == End.getX() && 
			Intersection.getY() == End.getY() && 
			Intersection.getZ() == End.getZ() ) )
	{
		return 0;
	}

	if ( Intersect )
	{
		Intersect->setX(Intersection.getX()); 
		Intersect->setY(Intersection.getY()); 
		Intersect->setZ(Intersection.getZ());
	}

	return IsPointOnPlane( Intersection, arv3Pos, iCnt, v3PolygonNormal );
}


void MakeUpBVSphere(	const Vector3* v3Poly_, 
						const int iPosCnt,
						_BV_Sphere& bvs )
{
	Vector3		v3Center, v3Temp( 0.f, 0.f, 0.f ); 
	float		fMaxEdge = 0.0f, fCurEdge = 0.0f;

	for( int i = 0; i < iPosCnt; ++i )
	{
		v3Temp = v3Temp + v3Poly_[i];
	}

	v3Center = v3Temp / (float)iPosCnt;

	for( int i = 0; i < iPosCnt; ++i )
	{
		fCurEdge = lengthSqr(v3Center-v3Poly_[i]);
		fMaxEdge =  fCurEdge > fMaxEdge ? fCurEdge : fMaxEdge;	
	}

	bvs._fRadius_Square	= fMaxEdge;
	bvs._fRadius		= sqrt( fMaxEdge );
	bvs._v3Center		= v3Center;
}

// @ 20101203 Triangle AABB Collision
///
//	FacePlane()
//   
//		Which of the six face-plane(s) is point P outside of? 
//
int FacePlane(Vector3 p)
{
	int outcode;

	outcode = 0;
	if (p.getX() >  .5) outcode |= 0x01;
	if (p.getX() < -.5) outcode |= 0x02;
	if (p.getY() >  .5) outcode |= 0x04;
	if (p.getY() < -.5) outcode |= 0x08;
	if (p.getZ() >  .5) outcode |= 0x10;
	if (p.getZ() < -.5) outcode |= 0x20;

	return(outcode);
}

///
//	Bevel2d()
//
//	 Which of the twelve edge plane(s) is point P outside of? 
//
int Bevel2d(Vector3 p)
{
	int outcode;

	outcode = 0;
	if ( p.getX() + p.getY() > 1.0) outcode |= 0x001;
	if ( p.getX() - p.getY() > 1.0) outcode |= 0x002;
	if (-p.getX() + p.getY() > 1.0) outcode |= 0x004;
	if (-p.getX() - p.getY() > 1.0) outcode |= 0x008;
	if ( p.getX() + p.getZ() > 1.0) outcode |= 0x010;
	if ( p.getX() - p.getZ() > 1.0) outcode |= 0x020;
	if (-p.getX() + p.getZ() > 1.0) outcode |= 0x040;
	if (-p.getX() - p.getZ() > 1.0) outcode |= 0x080;
	if ( p.getY() + p.getZ() > 1.0) outcode |= 0x100;
	if ( p.getY() - p.getZ() > 1.0) outcode |= 0x200;
	if (-p.getY() + p.getZ() > 1.0) outcode |= 0x400;
	if (-p.getY() - p.getZ() > 1.0) outcode |= 0x800;
	return(outcode);
}

///
//	Bevel3d()
//	
//		Which of the eight corner plane(s) is point P outside of? 
//
int Bevel3d(Vector3 p)
{
	int outcode;

	outcode = 0;
	if (( p.getX() + p.getY() + p.getZ()) > 1.5) outcode |= 0x01;
	if (( p.getX() + p.getY() - p.getZ()) > 1.5) outcode |= 0x02;
	if (( p.getX() - p.getY() + p.getZ()) > 1.5) outcode |= 0x04;
	if (( p.getX() - p.getY() - p.getZ()) > 1.5) outcode |= 0x08;
	if ((-p.getX() + p.getY() + p.getZ()) > 1.5) outcode |= 0x10;
	if ((-p.getX() + p.getY() - p.getZ()) > 1.5) outcode |= 0x20;
	if ((-p.getX() - p.getY() + p.getZ()) > 1.5) outcode |= 0x40;
	if ((-p.getX() - p.getY() - p.getZ()) > 1.5) outcode |= 0x80;
	return(outcode);
}

int checkIntersectlineWithPoint(Vector3 v3LineStart, Vector3 v3LineEnd, float alpha, long mask)
{
	Vector3 plane_point;

	plane_point.setX(LERP(alpha, v3LineStart.getX(), v3LineEnd.getX()));
	plane_point.setY(LERP(alpha, v3LineStart.getY(), v3LineEnd.getY()));
	plane_point.setZ(LERP(alpha, v3LineStart.getZ(), v3LineEnd.getZ()));
	return(FacePlane(plane_point) & mask);
}

///
//	checkIntersectlinewithFaces()
//
//		Compute intersection of P1 --> P2 line segment with face planes 
//		Then test intersection point to see if it is on cube face       
//		Consider only face planes in "outcode_diff"                     
//		Note: Zero bits in "outcode_diff" means face line is outside of */
//
int checkIntersectlinewithFaces(Vector3 v3LineStart, Vector3 v3LineEnd, int outcode_diff)
{

	if ((0x01 & outcode_diff) != 0)
		if (checkIntersectlineWithPoint(v3LineStart,v3LineEnd,( .5f-v3LineStart.getX())/(v3LineEnd.getX()-v3LineStart.getX()),0x3e) == INSIDE) return(INSIDE);
	if ((0x02 & outcode_diff) != 0)
		if (checkIntersectlineWithPoint(v3LineStart,v3LineEnd,(-.5f-v3LineStart.getX())/(v3LineEnd.getX()-v3LineStart.getX()),0x3d) == INSIDE) return(INSIDE);
	if ((0x04 & outcode_diff) != 0) 
		if (checkIntersectlineWithPoint(v3LineStart,v3LineEnd,( .5f-v3LineStart.getY())/(v3LineEnd.getY()-v3LineStart.getY()),0x3b) == INSIDE) return(INSIDE);
	if ((0x08 & outcode_diff) != 0) 
		if (checkIntersectlineWithPoint(v3LineStart,v3LineEnd,(-.5f-v3LineStart.getY())/(v3LineEnd.getY()-v3LineStart.getY()),0x37) == INSIDE) return(INSIDE);
	if ((0x10 & outcode_diff) != 0) 
		if (checkIntersectlineWithPoint(v3LineStart,v3LineEnd,( .5f-v3LineStart.getZ())/(v3LineEnd.getZ()-v3LineStart.getZ()),0x2f) == INSIDE) return(INSIDE);
	if ((0x20 & outcode_diff) != 0) 
		if (checkIntersectlineWithPoint(v3LineStart,v3LineEnd,(-.5f-v3LineStart.getZ())/(v3LineEnd.getZ()-v3LineStart.getZ()),0x1f) == INSIDE) return(INSIDE);
	return(OUTSIDE);
}





int PointTriangleIntersection(Vector3 p, TRIPART::CTri tri)
{
	int		sign12,sign23,sign31;
	Vector3 vect12,vect23,vect31,vect1h,vect2h,vect3h;
	Vector3 cross12_1p,cross23_2p,cross31_3p;

	///
	//	First, a quick bounding-box test:                               
	//  If P is outside triangle bbox, there cannot be an intersection. 
	//
	if (p.getX() > MAX3(tri.v3PT[0].getX(), tri.v3PT[1].getX(), tri.v3PT[2].getX())) return(OUTSIDE);  
	if (p.getY() > MAX3(tri.v3PT[0].getY(), tri.v3PT[1].getY(), tri.v3PT[2].getY())) return(OUTSIDE);
	if (p.getZ() > MAX3(tri.v3PT[0].getZ(), tri.v3PT[1].getZ(), tri.v3PT[2].getZ())) return(OUTSIDE);
	if (p.getX() < MIN3(tri.v3PT[0].getX(), tri.v3PT[1].getX(), tri.v3PT[2].getX())) return(OUTSIDE);
	if (p.getY() < MIN3(tri.v3PT[0].getY(), tri.v3PT[1].getY(), tri.v3PT[2].getY())) return(OUTSIDE);
	if (p.getZ() < MIN3(tri.v3PT[0].getZ(), tri.v3PT[1].getZ(), tri.v3PT[2].getZ())) return(OUTSIDE);

	///
	//	For each triangle side, make a vector out of it by subtracting vertexes; 
	//	make another vector from one vertex to point P.                          
	//  The crossproduct of these two vectors is orthogonal to both and the      
	//  signs of its X,Y,Z components indicate whether P was to the inside or    
	//  to the outside of this triangle side.                                    
	//
	vect12 = tri.v3PT[0]-tri.v3PT[1];		//	SUB(tri.v3PT[0], tri.v3PT[1], vect12);
	vect1h = tri.v3PT[0]-p;					//	SUB(tri.v3PT[0], p,		  vect1h);	
	cross12_1p = cross(vect12, vect1h);		//	CROSS(vect12, vect1h, cross12_1p)
		sign12 = SIGN3(cross12_1p);      /* Extract X,Y,Z signs as 0..7 or 0...63 integer */

	vect23 = tri.v3PT[1]-tri.v3PT[2];		//	SUB(tri.v3PT[1], tri.v3PT[2], vect23)
	vect2h = tri.v3PT[1]-p;					//	SUB(tri.v3PT[1],    p, vect2h);
	cross23_2p = cross(vect23, vect2h);		//	CROSS(vect23, vect2h, cross23_2p)
		sign23 = SIGN3(cross23_2p);

	vect31 = tri.v3PT[2]-tri.v3PT[0];		//	SUB(tri.v3PT[2], tri.v3PT[0], vect31)
	vect3h = tri.v3PT[2]-p;					//	SUB(tri.v3PT[2],    p, vect3h);
	cross31_3p = cross(vect31, vect3h);	//	CROSS(vect31, vect3h, cross31_3p)
		sign31 = SIGN3(cross31_3p);

	///
	//	If all three crossproduct vectors agree in their component signs, /
	//  then the point must be inside all three.                           
	//  P cannot be OUTSIDE all three sides simultaneously.                
	//
	return (((sign12 & sign23 & sign31) == 0) ? OUTSIDE : INSIDE);
}


int IntersectionTriCube(TRIPART::CTri tri)
{
	int v1_test,v2_test,v3_test;
	float d;
	Vector3 vect12,vect13,norm;
	Vector3 hitpp,hitpn,hitnp,hitnn;

	///
	//	First compare all three vertexes with all six face-planes 
	//	If any vertex is inside the cube, return immediately!     
	//
	if ((v1_test = FacePlane(tri.v3PT[0])) == INSIDE) return(INSIDE);
	if ((v2_test = FacePlane(tri.v3PT[1])) == INSIDE) return(INSIDE);
	if ((v3_test = FacePlane(tri.v3PT[2])) == INSIDE) return(INSIDE);

	///
	//	If all three vertexes were outside of one or more face-planes,
	//	return immediately with a trivial rejection!              
	//
	if ((v1_test & v2_test & v3_test) != 0) return(OUTSIDE);

	///
	//	Now do the same trivial rejection test for the 12 edge planes 
	//
	v1_test |= Bevel2d(tri.v3PT[0]) << 8; 
	v2_test |= Bevel2d(tri.v3PT[1]) << 8; 
	v3_test |= Bevel2d(tri.v3PT[2]) << 8;
	if ((v1_test & v2_test & v3_test) != 0) return(OUTSIDE);  

	///
	//	Now do the same trivial rejection test for the 8 corner planes
	//
	v1_test |= Bevel3d(tri.v3PT[0]) << 24; 
	v2_test |= Bevel3d(tri.v3PT[1]) << 24; 
	v3_test |= Bevel3d(tri.v3PT[2]) << 24; 
	if ((v1_test & v2_test & v3_test) != 0) return(OUTSIDE);   

	///
	//	If vertex 1 and 2, as a pair, cannot be trivially rejected 
	//  by the above tests, then see if the v1-->v2 triangle edge  
	//	intersects the cube.  Do the same for v1-->v3 and v2-->v3. 
	//  Pass to the intersection algorithm the "OR" of the outcode 
	//  bits, so that only those cube faces which are spanned by   
	//  each triangle edge need be tested.                         
	//
	if ((v1_test & v2_test) == 0)
		if (checkIntersectlinewithFaces(tri.v3PT[0],tri.v3PT[1],v1_test|v2_test) == INSIDE) return(INSIDE);
	if ((v1_test & v3_test) == 0)
		if (checkIntersectlinewithFaces(tri.v3PT[0],tri.v3PT[2],v1_test|v3_test) == INSIDE) return(INSIDE);
	if ((v2_test & v3_test) == 0)
		if (checkIntersectlinewithFaces(tri.v3PT[1],tri.v3PT[2],v2_test|v3_test) == INSIDE) return(INSIDE);

	///
	//	By now, we know that the triangle is not off to any side,     
	//	 and that its sides do not penetrate the cube.  We must now   
	//	 test for the cube intersecting the interior of the triangle. 
	//	 We do this by looking for intersections between the cube     
	//	 diagonals and the triangle...first finding the intersection  
	//	 of the four diagonals with the plane of the triangle, and    
	//	 then if that intersection is inside the cube, pursuing       
	//	 whether the intersection point is inside the triangle itself. 

	//	 To find plane of the triangle, first perform crossproduct on  
	//	 two triangle side vectors to compute the normal vector.                                       
	vect12 = tri.v3PT[0]-tri.v3PT[1];	//	SUB(tri.v3PT[0],tri.v3PT[1],vect12);
	vect13 = tri.v3PT[0]-tri.v3PT[2];	//	SUB(tri.v3PT[0],tri.v3PT[2],vect13);
	norm = cross(vect12,vect13);			//	CROSS(vect12,vect13,norm)

		///
		//	 The normal vector "norm" X,Y,Z components are the coefficients 
		//		 of the triangles AX + BY + CZ + D = 0 plane equation.  If we   
		//	 solve the plane equation for X=Y=Z (a diagonal), we get        
		//	 -D/(A+B+C) as a metric of the distance from cube center to the 
		//	 diagonal/plane intersection.  If this is between -0.5 and 0.5, 
		//	 the intersection is inside the cube.  If so, we continue by    
		//	 doing a point/triangle intersection.                           
		//	 Do this for all four diagonals.                                

	d = norm.getX() * tri.v3PT[0].getX() + norm.getY() * tri.v3PT[0].getY() + norm.getZ() * tri.v3PT[0].getZ();
//	hitpp.getX() = hitpp.getY() = hitpp.getZ() = d / (norm.getX() + norm.getY() + norm.getZ());
	hitpp.setZ(d / (norm.getX() + norm.getY() + norm.getZ()));
	hitpp.setY(hitpp.getZ());
	hitpp.setX(hitpp.getY()); 
	if (fabs(hitpp.getX()) <= 0.5)
		if (PointTriangleIntersection(hitpp,tri) == INSIDE) return(INSIDE);
	
//	hitpn.getZ() = -(hitpn.getX() = hitpn.getY() = d / (norm.getX() + norm.getY() - norm.getZ()));
	hitpn.setY(d / (norm.getX() + norm.getY() - norm.getZ()));
	hitpn.setX(hitpn.getY());
	hitpn.setZ(-(hitpn.getX()));
	if (fabs(hitpn.getX()) <= 0.5)
		if (PointTriangleIntersection(hitpn,tri) == INSIDE) return(INSIDE);
	
//	hitnp.getY() = -(hitnp.getX() = hitnp.getZ() = d / (norm.getX() - norm.getY() + norm.getZ()));
	hitnp.setZ(d / (norm.getX() - norm.getY() + norm.getZ()));
	hitnp.setX(hitnp.getZ());
	hitnp.setY(-(hitnp.getX()));
	if (fabs(hitnp.getX()) <= 0.5)
		if (PointTriangleIntersection(hitnp,tri) == INSIDE) return(INSIDE);

//	hitnn.getY() = hitnn.getZ() = -(hitnn.getX() = d / (norm.getX() - norm.getY() - norm.getZ()));
	hitnn.setX(d / (norm.getX() - norm.getY() - norm.getZ()));
	hitnn.setZ(-(hitnn.getX()));
	hitnn.setY(hitnn.getZ()); 
	if (fabs(hitnn.getX()) <= 0.5)
		if (PointTriangleIntersection(hitnn,tri) == INSIDE) return(INSIDE);

	///
	//	No edge touched the cube; no cube diagonal touched the triangle. 
	//  We're done...there was no intersection.                          
	//
	return(OUTSIDE);
}


int IntersectTriBox(_BV_AABB_Sphere BBox, TRIPART::CTri Tri)
{
	Vector3			Trans;
	Vector3			Scale;
	Vector3			TransMax;
	int				i;
	TRIPART::CTri	TestTri;

	///
	//	Compute the scale and transform required to make BBox
	//	a voxel
	//
	Trans.setX((BBox._v3Max.getX() + BBox._v3Min.getX()) / 2);
	Trans.setY((BBox._v3Max.getY() + BBox._v3Min.getY()) / 2);
	Trans.setZ((BBox._v3Max.getZ() + BBox._v3Min.getZ()) / 2);

	TransMax= BBox._v3Max - Trans;

	if(TransMax.getX() != 0)
		Scale.setX(0.5f / TransMax.getX());
	if(TransMax.getY() != 0)
		Scale.setY(0.5f / TransMax.getY());
	if(TransMax.getZ() != 0)
		Scale.setZ(0.5f / TransMax.getZ());

	///
	//	Put the triangle in voxel space
	//
	for(i= 0; i < 3; i++)
	{
		TestTri.v3PT[i].setX((Tri.v3PT[i].getX() - Trans.getX()) * Scale.getX());
		TestTri.v3PT[i].setY((Tri.v3PT[i].getY() - Trans.getY()) * Scale.getY());
		TestTri.v3PT[i].setZ((Tri.v3PT[i].getZ() - Trans.getZ()) * Scale.getZ());
	}

	///
	//	Test against the voxel
	//
	return(IntersectionTriCube(TestTri));	
}

void getPosIntersectedRay(	const Vector3 &v3posRayStart, 
						const Vector3 &v3dirRay, 
						const float fDistance, 
						Vector3 &v3PosIntersect )
{
	Vector3 v3DirNormal = normalize( v3dirRay );

	v3PosIntersect = v3posRayStart + (v3DirNormal*fDistance);
}

};