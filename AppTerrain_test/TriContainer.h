#pragma once

#include <PSSG.h>

#include "CommonApp.h"

#include "CommonSingleton.h"

#include "BoundaryVolume.h"
#include "Tri.h"

#include <set>

using namespace PSSG;
using namespace Vectormath::Aos;

namespace TRIPART
{
	typedef vector<CTri>	VEC_TRIS;

	class CTriContainer : public CSingleton<CTriContainer>
	{
	public:
		VEC_TRIS				m_vecTris;
		BV::_BV_AABB			m_bvAABB;

	public:
		void Initialize();
		void Release();

	private:
		int getSizeDataType( PTypeID EDataType );

// 		template<typename T>
// 		bool constructAllTris_refIV_template(	T					*pArrIndexVertex,
// 												const unsigned int	iCntIV,
// 												const float			*parVertexData,
// 												vector<CTri>		&vecTris,
// 												Matrix4				*matTransform )
// 		{
// 			Vector3			v3TriVer[3];
// 			Vector4			v4CurrentVertex(1.0f);
// 			unsigned int	ariTriIdx[3];
// 			unsigned int	iCntTries = (iCntIV/3)+1;
// 			CTri			triCurrent;
// 			bool			bTransform = (matTransform!=NULL);
// 
// 			T *pArrIV = (T*)pArrIndexVertex;
// 
// 			for( unsigned int iIdx=0, iTri=0; iIdx<iCntIV; iIdx+=3,++iTri )
// 			{
// 				ariTriIdx[0] = (unsigned int)pArrIV[iIdx];
// 				ariTriIdx[1] = (unsigned int)pArrIV[iIdx+1];
// 				ariTriIdx[2] = (unsigned int)pArrIV[iIdx+2];
// 
// 				for( int itV=0; itV<3; ++itV )
// 				{
// 					v4CurrentVertex.setX( *(parVertexData + (ariTriIdx[itV]*3)) );
// 					v4CurrentVertex.setY( *(parVertexData + (ariTriIdx[itV]*3+1)) );
// 					v4CurrentVertex.setZ( *(parVertexData + (ariTriIdx[itV]*3+2)) );
// 
// 					if(bTransform)
// 					{
// 						v4CurrentVertex = *(matTransform)*v4CurrentVertex;
// 					}
// 
// 					vectorCopy4to3(v3TriVer[itV],v4CurrentVertex);
// 
// 					if(v3TriVer[itV].getX()>m_bvAABB._v3Max.getX()) { m_bvAABB._v3Max.setX(v3TriVer[itV].getX()); }
// 					if(v3TriVer[itV].getY()>m_bvAABB._v3Max.getY()) { m_bvAABB._v3Max.setY(v3TriVer[itV].getY()); }
// 					if(v3TriVer[itV].getZ()>m_bvAABB._v3Max.getZ()) { m_bvAABB._v3Max.setZ(v3TriVer[itV].getZ()); }
// 
// 					if(v3TriVer[itV].getX()<m_bvAABB._v3Min.getX()) { m_bvAABB._v3Min.setX(v3TriVer[itV].getX()); }
// 					if(v3TriVer[itV].getY()<m_bvAABB._v3Min.getY()) { m_bvAABB._v3Min.setY(v3TriVer[itV].getY()); }
// 					if(v3TriVer[itV].getZ()<m_bvAABB._v3Min.getZ()) { m_bvAABB._v3Min.setZ(v3TriVer[itV].getZ()); }
// 				}
// 
// 				triCurrent.Set(v3TriVer, ariTriIdx);
// 				vecTris.push_back( triCurrent );
// 		};

	public:
		const VEC_TRIS	&getTris(){ return m_vecTris; };
		bool	getTriIdxAll(	vector<int> &vecTriidx__)
		{
			int iCntAllTris=m_vecTris.size();
			if(iCntAllTris<1) 
			{
				return false;
			}

			vecTriidx__.resize(iCntAllTris);

			for(int iTriIdx=0;iTriIdx<iCntAllTris;++iTriIdx)
			{
				vecTriidx__[iTriIdx]=iTriIdx;
			}

			return true;
		};
		unsigned int getCountTries(){ return m_vecTris.size(); };
		bool getTri(const unsigned int iIdxTriangle, CTri &triangle);
		CTri &getTri(unsigned int uiSeqTri)	{ return m_vecTris[uiSeqTri]; };

		bool constructAllTris_noREF(	const void		*pArrIndexVertex,
										PTypeID			eTypeData,
										const int		iCntVB,
										const int		iCntIV,
										const float		*parVertexData,
										vector<CTri>	&vecTris,
										Matrix4			*matTransform=NULL );

		bool constructAllTris_refIV(	const void		*pArrIndexVertex,
										PTypeID			eTypeData,
										const int		iCntVB,
										const int		iCntIV,
										const float		*parVertexData,
										vector<CTri>	&vecTris,
										Matrix4			*matTransform=NULL );

		bool constructAllTriangles(		PRenderDataSource *renderDataSource, 
										Matrix4 *matTransform=NULL, 
										bool bNewsettingRefIV=false );
		
		//	void constructAllTrianglesWithoutIV_test( PRenderDataSource *renderDataSource );

	public:
		CTriContainer();
		~CTriContainer();
	};
}



