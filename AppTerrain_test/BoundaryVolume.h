#pragma once

#include <PSSG.h>
#include <PSSGTerrain/PSSGTerrain.h>
#include <Extra/PSSGExtra.h>

#include "CommonApp.h"
#include "CommonMath.h"

#include "Tri.h"

namespace BV
{

	enum		ENUM_BV_TYPE { BV_NULL = -1, BV_AABB = 0, BV_SPHERE, };

#define	DEFAULT_BV_TYPE			BV_AABB
#define THRESH_POINT_ON_PLANE	(0.01f)		/* Thickness of plane for front/back/inside test */


	/// @name 광선
	struct _Ray
	{
		Vector3	_v3Origin;		///< 시작점
		Vector3	_v3Direction;	///< 방향 ( unit vector 이어야 한다.)
		float	_fDistance;
		bool	_bInfinite;

		void set(const Vector3 &v3RayOrigin_,const Vector3 &v3Dir_,const bool bInfinite = false )
		{
			memcpy( &_v3Origin, &v3RayOrigin_, sizeof(Vector3) );
			memcpy( &_v3Direction, &v3Dir_, sizeof(Vector3) );

			if( false == bInfinite )
			{
				setCalToDist();
			}
		}
	private:
		void setCalToDist()
		{
			_fDistance = length(_v3Direction);
		};


	public:
		_Ray()
		{
			_fDistance = 0.0f;
			_bInfinite = false;
		};

		~_Ray()
		{
			_fDistance = 0.0f;
			_bInfinite = false;			
		};
	};

	/// @name Bounding volume - AABB ( Axis Aligned Bounding Box)
	struct _BV_AABB
	{
		Vector3	_v3Min;	///< v min
		Vector3	_v3Max;	///< v max

		Vector3	_v3Center;

		void CalCentralize()
		{
			_v3Center = _v3Min - _v3Max; 
			_v3Center.setX(fabs(_v3Center.getX()));
			_v3Center.setY(fabs(_v3Center.getY()));
			_v3Center.setZ(fabs(_v3Center.getZ()));

			_v3Center /= 2;
		}
	public:
		_BV_AABB()
		{

		}

		~_BV_AABB()
		{
			
		}
	};

	/// @name Bounding volume - AABB ( Axis Aligned Bounding Box) + Sphere
	struct _BV_AABB_Sphere
	{
		Vector3	_v3Min;	///< v min
		Vector3	_v3Max;	///< v max

		Vector3	_v3Center;
		float	_fRadius;
		float	_fRadius_Square;

		Vector3	_v3E_Slab;

		void CalculateToSphere()
		{
			_v3Center = ( _v3Max + _v3Min ) * 0.5f;
			Vector3 v3Radius = _v3Max - _v3Center;

			_fRadius = length(v3Radius);
			//_fRadius = v3Radius.Length();
			_fRadius_Square = lengthSqr(v3Radius);
		}

		void CalculateToSphere_slab()
		{
			CalculateToSphere();

			_v3E_Slab = _v3Min - _v3Max; 
			_v3E_Slab.setX(fabs(_v3E_Slab.getX()));
			_v3E_Slab.setY(fabs(_v3E_Slab.getY()));
			_v3E_Slab.setZ(fabs(_v3E_Slab.getZ()));

			_v3E_Slab /= 2;
		}

	public:
		_BV_AABB_Sphere()
		{

		};

		~_BV_AABB_Sphere()
		{

		};
	};


	/// @name Bounding volume - AABB ( Axis Aligned Bounding Box)
	struct _BV_Sphere
	{
		Vector3	_v3Center;
		float	_fRadius;
		float	_fRadius_Square;

		_BV_Sphere()
		{
			//_v3Center.Set(0.0f,0.0f,0.0f);
			_v3Center.setX(0.f);
			_v3Center.setY(0.f);
			_v3Center.setZ(0.f);

			_fRadius		= 0.f;
			_fRadius_Square = 0.f;
		}
	};



	// 광선 - AABB : 충돌 검사
	bool IntersectInfiniteRayAABB( const _Ray &Ray_, const _BV_AABB_Sphere &bvAABB_ );

	// 광선 - 구 : 충돌 검사
	bool IntersectRaySphere( const _Ray &Ray_, const _BV_AABB_Sphere &BvSphere);

	bool IntersectRayBV(  const _Ray &Ray_, const _BV_AABB_Sphere &BvAABBSphere, ENUM_BV_TYPE eBvType );

	// 교차 검사 (외부/교차/포함) 교차지점 유
	bool IntersectRayTriOriginDir( const Vector3& v3RayOrigin,
									const Vector3& v3RayDir , 
									const Vector3& v3T_1, 
									const Vector3& v3T_2, 
									const Vector3& v3T_3, 
									float *pfT = 0);


	// 교차 검사 (외부/교차/포함) 교차지점 유
	bool IntersectRayTri( const _Ray& _ray,
							const Vector3& v3T_1, 
							const Vector3& v3T_2, 
							const Vector3& v3T_3, 
							float *pfT = 0);

	float FPointPlaneDist(	const Vector3 &Point,
							const Vector3 &PlaneBase,
							const Vector3 &PlaneNormal);


	Vector3 FLinePlaneIntersection(	const Vector3 &v3Start,
									const Vector3 &v3End,
									const Vector3 &PlaneAnyVertex,
									const Vector3 &PlaneNormal);


	bool IsPointOnPlane(	const Vector3 &v3PosCheckup, 
							const Vector3 *arv3Pos, 
							const int iCnt, 
							const Vector3 &v3PolygonNormal );


	bool IsLineIntersect_point( const Vector3& Start, 
							const Vector3& End, 
							const Vector3* arv3Pos, 
							int iCnt,
							const Vector3& v3PolygonNormal,
							Vector3* Intersect );



	void MakeUpBVSphere(const Vector3* v3Poly_, 
						const int iPosCnt,
						_BV_Sphere& bvs );


	// @ 20101203:신규 
	// -----------------------------------------------------------------------------
	///
	//	FacePlane()
	//   
	//		Which of the six face-plane(s) is point P outside of? 
	//
	int FacePlane(Vector3 p);
	///
	//	Bevel2d()
	//
	//	 Which of the twelve edge plane(s) is point P outside of? 
	//
	int Bevel2d(Vector3 p);
	///
	//	Bevel3d()
	//	
	//		Which of the eight corner plane(s) is point P outside of? 
	//
	int Bevel3d(Vector3 p);

	int checkIntersectlineWithPoint(Vector3 v3LineStart, Vector3 v3LineEnd, float alpha, long mask);

	///
	//	checkIntersectlinewithFaces()
	//
	//		Compute intersection of P1 --> P2 line segment with face planes 
	//		Then test intersection point to see if it is on cube face       
	//		Consider only face planes in "outcode_diff"                     
	//		Note: Zero bits in "outcode_diff" means face line is outside of */
	//
	int checkIntersectlinewithFaces(Vector3 v3LineStart, Vector3 v3LineEnd, int outcode_diff);

	int PointTriangleIntersection(Vector3 p, TRIPART::CTri tri);

	int IntersectionTriCube(TRIPART::CTri t);

	int IntersectTriBox(_BV_AABB_Sphere BBox, TRIPART::CTri Tri);
	// @ 20101203:신규 / 까지
	// -----------------------------------------------------------------------------

	void getPosIntersectedRay(	const Vector3 &v3posRayStart, 
							const Vector3 &v3dirRay, 
							const float fDistance, 
							Vector3 &v3PosIntersect );
};

