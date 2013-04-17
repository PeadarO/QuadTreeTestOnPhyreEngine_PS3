#include "TriContainer.h"

using namespace TRIPART;

void CTriContainer::Initialize()
{

}

void CTriContainer::Release()
{
	m_vecTris.clear();
}



int CTriContainer::getSizeDataType(PTypeID EDataType )
{
	switch(EDataType)
	{
	case PSSG::PE_TYPE_UCHAR:
		{
			return sizeof(unsigned char);
		}
		break;
	case PSSG::PE_TYPE_USHORT:
		{
			return sizeof(unsigned short);
		}
		break;
	case PSSG::PE_TYPE_UINT:
		{
			return sizeof(unsigned int);
		}
		break;
	default:
		return -1;
	}
}


bool CTriContainer::constructAllTris_noREF(	const void		*pArrIndexVertex,
											PTypeID			eTypeData,
											const int		iCntVB,
											const int		iCntIV,
											const float		*parVertexData,
											vector<CTri>	&vecTris,
											Matrix4			*matTransform )
{
	Vector3			v3TriVer[3], v3Ver;
	Vector4			v4CurrentVertex(1.0f);
	unsigned int	ariTriIdx[3];
	unsigned int	iCntTries = (iCntIV/3)+1;
	bool			bTransform = (matTransform!=NULL);
	vector<Vector3>	vecVB;
	vector<unsigned int> vecIB, vecIBConvert;

	vecVB.reserve(iCntIV);
	vecTris.reserve( vecTris.size()+iCntTries );

	switch( eTypeData )
	{
	case PE_TYPE_UCHAR:
		{
			unsigned char *pArrIV = (unsigned char*)pArrIndexVertex;

			for( int iSeqIdx=0; iSeqIdx<iCntIV; ++iSeqIdx )
			{
				vecIB.push_back( (unsigned int)pArrIV[iSeqIdx] );
			}
		}
		break;
	case PE_TYPE_USHORT:
		{
			unsigned short *pArrIV = (unsigned short*)pArrIndexVertex;

			for( int iSeqIdx=0; iSeqIdx<iCntIV; ++iSeqIdx )
			{
				vecIB.push_back( (unsigned int)pArrIV[iSeqIdx] );
			}
		}
		break;
	case PE_TYPE_UINT:
		{
			unsigned int *pArrIV = (unsigned int*)pArrIndexVertex;

			for( int iSeqIdx=0; iSeqIdx<iCntIV; ++iSeqIdx )
			{
				vecIB.push_back( (unsigned int)pArrIV[iSeqIdx] );
			}
		}
		break;
	default:
		{
			assert( 0 && "Error : Undefinition type of indexbyffer while Construct Triangle." );
			return false;
		}
	} // switch( eTypeData )

	for(int iVB=0; iVB<iCntVB; ++iVB)
	{
		v3Ver.setX( *(parVertexData+(iVB*3)) );
		v3Ver.setY( *(parVertexData+(iVB*3+1)) );
		v3Ver.setZ( *(parVertexData+(iVB*3+2)) );

		vecVB.push_back(v3Ver);
	}

// 	for(int iVB=0; iVB<iCntIV; ++iVB)
// 	{
// 		int iIVCurr=vecIB[iVB];
// 		for( int itV=0; itV<3; ++itV )
// 		{
// 			v3Ver.setX( *(parVertexData + (iIVCurr*3)) );
// 			v3Ver.setY( *(parVertexData + (iIVCurr*3+1)) );
// 			v3Ver.setZ( *(parVertexData + (iIVCurr*3+2)) );
// 		}
// 
// 		vecVB.push_back(v3Ver);
// 	}

	{
		int iIVConvert=0, iIVCurr=0;
		bool bfindEQ=false;
#if defined(_DEBUG) && defined(APPTARGET_IS_WIN)
		int iCntExactlySameTris=0, iCntNoValidatePoly=0;
		int iCntSameVer__=0;

//		char	szOutput[512];
		OutputDebugString( "EQ Tris" );
		OutputDebugString( "===============================================================\n" );
#endif // defined(_DEBUG) && defined(APPTARGET_IS_WIN)

		vecIBConvert.resize(iCntIV);
		for(int iIV=0; iIV<iCntIV; ++iIV)
		{
			vecIBConvert[iIV]=vecIB[iIV];
		} // for(int iIV=0; iIV<iCntIV; ++iIV)
		
		for(int iIV=0; iIV<iCntVB; ++iIV)
		{
			Vector3 &v3VB=vecVB[iIV];
			iIVCurr=iIV; bfindEQ=false;
			for(int iIV_=0; iIV_<iIVCurr; ++iIV_)
			{
				Vector3 &v3VB_=vecVB[iIV_];
				//bfindEQ=EQ_V3(v3VB,v3VB_);
				//bfindEQ=APPROX_EQ_V3(v3VB,v3VB_);
				bfindEQ=(	v3VB.getX()==v3VB_.getX()
							&&v3VB.getY()==v3VB_.getY()
							&&v3VB.getZ()==v3VB_.getZ() );

				if(bfindEQ)
				{
					iIVConvert=iIV_;
					break;
				}
			} // for(int iIV_=0; iIV_<iIV; ++IV_)
			
			if(true==bfindEQ)
			{
#if defined(_DEBUG) && defined(APPTARGET_IS_WIN)
				++iCntSameVer__;
#endif // defined(_DEBUG) && defined(APPTARGET_IS_WIN)
				for(int iSeqIV=0; iSeqIV<iCntIV; ++iSeqIV)
				{
					int iIBCurr=(int)vecIB[iSeqIV];
					if(iIBCurr==iIV)
					{
						vecIBConvert[iSeqIV]=iIVConvert;
					}
				}// for(int iSeqIV=0; iSeqIV<vecIB.size(); ++iSeqIV)
			}

		} // for(int iIV=0; iIV<vecVB.size(); ++iIV)


		// @Insert only validation succeeded triangle.
		CTri			triCurrent;
		for( int iSeqIdx=0, iTri=0; iSeqIdx<iCntIV; iSeqIdx+=3,++iTri )
		{
			ariTriIdx[0] = (unsigned int)vecIBConvert[iSeqIdx];
			ariTriIdx[1] = (unsigned int)vecIBConvert[iSeqIdx+1];
			ariTriIdx[2] = (unsigned int)vecIBConvert[iSeqIdx+2];

			for( int itV=0; itV<3; ++itV )
			{
				vectorCopy3to4(v4CurrentVertex,vecVB[ariTriIdx[itV]],1.0f);

				if(bTransform)
				{
					v4CurrentVertex = *(matTransform)*v4CurrentVertex;
				}

				vectorCopy4to3(v3TriVer[itV],v4CurrentVertex);

				if(v3TriVer[itV].getX()>m_bvAABB._v3Max.getX()) { m_bvAABB._v3Max.setX(v3TriVer[itV].getX()); }
				if(v3TriVer[itV].getY()>m_bvAABB._v3Max.getY()) { m_bvAABB._v3Max.setY(v3TriVer[itV].getY()); }
				if(v3TriVer[itV].getZ()>m_bvAABB._v3Max.getZ()) { m_bvAABB._v3Max.setZ(v3TriVer[itV].getZ()); }

				if(v3TriVer[itV].getX()<m_bvAABB._v3Min.getX()) { m_bvAABB._v3Min.setX(v3TriVer[itV].getX()); }
				if(v3TriVer[itV].getY()<m_bvAABB._v3Min.getY()) { m_bvAABB._v3Min.setY(v3TriVer[itV].getY()); }
				if(v3TriVer[itV].getZ()<m_bvAABB._v3Min.getZ()) { m_bvAABB._v3Min.setZ(v3TriVer[itV].getZ()); }
			}

			triCurrent.Set(v3TriVer, ariTriIdx);

			if(true==triCurrent.isValidateASPoly())
			{
				bool bBeExistExactlysame=false;
				int iCntTris=vecTris.size();
				for(int iSeqIdx__=0; iSeqIdx__<iCntTris; ++iSeqIdx__)
				{
					if(iSeqIdx__!=iSeqIdx)
					{
						CTri	tri__=vecTris[iSeqIdx__];
						if(tri__.isEqual(triCurrent))
						{
#if defined(_DEBUG) && defined(APPTARGET_IS_WIN)
							++iCntExactlySameTris;
#endif // defined(_DEBUG) && defined(APPTARGET_IS_WIN)
							bBeExistExactlysame=true;
							break;
						}
					}
				}

				if(false==bBeExistExactlysame)
				{
					vecTris.push_back( triCurrent );
				}
			} // if(true==triCurrent.isValidateASPoly())
#if defined(_DEBUG) && defined(APPTARGET_IS_WIN)
			else
			{
				++iCntNoValidatePoly;
			}
#endif // defined(_DEBUG) && defined(APPTARGET_IS_WIN)
		} // for( unsigned int iSeqIdx=0, iTri=0; iSeqIdx<iCntIV; iSeqIdx+=3,++iTri )
//#if defined(_DEBUG) && defined(APPTARGET_IS_WIN)
//		int iBreakPoint = 0;
//#endif // defined(_DEBUG) && defined(APPTARGET_IS_WIN)
	}

	return true;
}

bool CTriContainer::constructAllTris_refIV(	const void		*pArrIndexVertex,
											  PTypeID		eTypeData,
											  const int		iCntVB,
											  const int		iCntIV,
											  const float	*parVertexData,
											  vector<CTri>	&vecTris,
											  Matrix4		*matTransform )
{
	Vector3			v3TriVer[3];
	Vector4			v4CurrentVertex(1.0f);
	unsigned int	ariTriIdx[3];
	unsigned int	iCntTries = (iCntIV/3)+1;
	CTri			triCurrent;
	bool			bTransform = (matTransform!=NULL);


	m_vecTris.reserve( m_vecTris.size()+iCntTries );

	switch( eTypeData )
	{
		case PE_TYPE_UCHAR:
			{
				unsigned char *pArrIV = (unsigned char*)pArrIndexVertex;

				for( int iSeqIdx=0, iTri=0; iSeqIdx<iCntIV; iSeqIdx+=3,++iTri )
				{
					ariTriIdx[0] = (unsigned int)pArrIV[iSeqIdx];
					ariTriIdx[1] = (unsigned int)pArrIV[iSeqIdx+1];
					ariTriIdx[2] = (unsigned int)pArrIV[iSeqIdx+2];

					for( int itV=0; itV<3; ++itV )
					{
						v4CurrentVertex.setX( *(parVertexData + (ariTriIdx[itV]*3)) );
						v4CurrentVertex.setY( *(parVertexData + (ariTriIdx[itV]*3+1)) );
						v4CurrentVertex.setZ( *(parVertexData + (ariTriIdx[itV]*3+2)) );

						if(bTransform)
						{
							v4CurrentVertex = *(matTransform)*v4CurrentVertex;
						}

						vectorCopy4to3(v3TriVer[itV],v4CurrentVertex);

						if(v3TriVer[itV].getX()>m_bvAABB._v3Max.getX()) { m_bvAABB._v3Max.setX(v3TriVer[itV].getX()); }
						if(v3TriVer[itV].getY()>m_bvAABB._v3Max.getY()) { m_bvAABB._v3Max.setY(v3TriVer[itV].getY()); }
						if(v3TriVer[itV].getZ()>m_bvAABB._v3Max.getZ()) { m_bvAABB._v3Max.setZ(v3TriVer[itV].getZ()); }

						if(v3TriVer[itV].getX()<m_bvAABB._v3Min.getX()) { m_bvAABB._v3Min.setX(v3TriVer[itV].getX()); }
						if(v3TriVer[itV].getY()<m_bvAABB._v3Min.getY()) { m_bvAABB._v3Min.setY(v3TriVer[itV].getY()); }
						if(v3TriVer[itV].getZ()<m_bvAABB._v3Min.getZ()) { m_bvAABB._v3Min.setZ(v3TriVer[itV].getZ()); }
					}

					triCurrent.Set(v3TriVer, ariTriIdx);
					vecTris.push_back( triCurrent );
				}
			}
			break;
		case PE_TYPE_USHORT:
			{
				unsigned short *pArrIV = (unsigned short*)pArrIndexVertex;

				for( int iSeqIdx=0, iTri=0; iSeqIdx<iCntIV; iSeqIdx+=3,++iTri )
				{
					ariTriIdx[0] = (unsigned int)pArrIV[iSeqIdx];
					ariTriIdx[1] = (unsigned int)pArrIV[iSeqIdx+1];
					ariTriIdx[2] = (unsigned int)pArrIV[iSeqIdx+2];

					for( int itV=0; itV<3; ++itV )
					{
						v4CurrentVertex.setX( *(parVertexData + (ariTriIdx[itV]*3)) );
						v4CurrentVertex.setY( *(parVertexData + (ariTriIdx[itV]*3+1)) );
						v4CurrentVertex.setZ( *(parVertexData + (ariTriIdx[itV]*3+2)) );

						if(bTransform)
						{
							v4CurrentVertex = *(matTransform)*v4CurrentVertex;
						}

						vectorCopy4to3(v3TriVer[itV],v4CurrentVertex);

						if(v3TriVer[itV].getX()>m_bvAABB._v3Max.getX()) { m_bvAABB._v3Max.setX(v3TriVer[itV].getX()); }
						if(v3TriVer[itV].getY()>m_bvAABB._v3Max.getY()) { m_bvAABB._v3Max.setY(v3TriVer[itV].getY()); }
						if(v3TriVer[itV].getZ()>m_bvAABB._v3Max.getZ()) { m_bvAABB._v3Max.setZ(v3TriVer[itV].getZ()); }

						if(v3TriVer[itV].getX()<m_bvAABB._v3Min.getX()) { m_bvAABB._v3Min.setX(v3TriVer[itV].getX()); }
						if(v3TriVer[itV].getY()<m_bvAABB._v3Min.getY()) { m_bvAABB._v3Min.setY(v3TriVer[itV].getY()); }
						if(v3TriVer[itV].getZ()<m_bvAABB._v3Min.getZ()) { m_bvAABB._v3Min.setZ(v3TriVer[itV].getZ()); }
					}

					triCurrent.Set(v3TriVer, ariTriIdx);
					vecTris.push_back( triCurrent );
				}
			}
			break;
		case PE_TYPE_UINT:
			{
				unsigned int *pArrIV = (unsigned int*)pArrIndexVertex;

				for( int iSeqIdx=0, iTri=0; iSeqIdx<iCntIV; iSeqIdx+=3,++iTri )
				{
					ariTriIdx[0] = pArrIV[iSeqIdx];
					ariTriIdx[1] = pArrIV[iSeqIdx+1];
					ariTriIdx[2] = pArrIV[iSeqIdx+2];

					for( int itV=0; itV<3; ++itV )
					{
						v4CurrentVertex.setX( *(parVertexData + (ariTriIdx[itV]*3)) );
						v4CurrentVertex.setY( *(parVertexData + (ariTriIdx[itV]*3+1)) );
						v4CurrentVertex.setZ( *(parVertexData + (ariTriIdx[itV]*3+2)) );

						if(bTransform)
						{
							v4CurrentVertex = *(matTransform)*v4CurrentVertex;
						}

						vectorCopy4to3(v3TriVer[itV],v4CurrentVertex);

						if(v3TriVer[itV].getX()>m_bvAABB._v3Max.getX()) { m_bvAABB._v3Max.setX(v3TriVer[itV].getX()); }
						if(v3TriVer[itV].getY()>m_bvAABB._v3Max.getY()) { m_bvAABB._v3Max.setY(v3TriVer[itV].getY()); }
						if(v3TriVer[itV].getZ()>m_bvAABB._v3Max.getZ()) { m_bvAABB._v3Max.setZ(v3TriVer[itV].getZ()); }

						if(v3TriVer[itV].getX()<m_bvAABB._v3Min.getX()) { m_bvAABB._v3Min.setX(v3TriVer[itV].getX()); }
						if(v3TriVer[itV].getY()<m_bvAABB._v3Min.getY()) { m_bvAABB._v3Min.setY(v3TriVer[itV].getY()); }
						if(v3TriVer[itV].getZ()<m_bvAABB._v3Min.getZ()) { m_bvAABB._v3Min.setZ(v3TriVer[itV].getZ()); }
					}

					triCurrent.Set(v3TriVer, ariTriIdx);
					vecTris.push_back( triCurrent );
				}
			}
			break;
		default:
			{
				assert( 0 && "Error : Undefinition type of indexbyffer while Construct Triangle." );
				return false;
			}
	}

	return true;
}


bool CTriContainer::constructAllTriangles(	PRenderDataSource *renderDataSource, 
											Matrix4 *matTransform,
											bool bNewsettingRefIV )
{
	// Vertex Array
	PRenderStream *pRenderStream = renderDataSource->getStream(0);
	PDataBlock *vertexDatablock = pRenderStream->getDataBlock();
	float *parVertexData = (float*)vertexDatablock->getData();
	int iCntVB=vertexDatablock->getElementCount();

	// Index Vertex Array
	PRenderIndexSource *pIndexSource = renderDataSource->getIndexSource();
	int iCntIV = pIndexSource->getElementCount();

	bool bResult=false, bNewsettingRefIV__=bNewsettingRefIV;
	
	if(true==bNewsettingRefIV__)
	{
		bResult=constructAllTris_noREF(	pIndexSource->getData(),
										pIndexSource->getDataType().m_type,
										iCntVB,
										iCntIV, 
										parVertexData, 
										m_vecTris,
										matTransform );
	}
	else
 	{
		bResult=constructAllTris_refIV(	pIndexSource->getData(),
										pIndexSource->getDataType().m_type,
										iCntVB,
										iCntIV, 
										parVertexData, 
										m_vecTris,
										matTransform );
	}

	return bResult;
}


// void CTriContainer::constructAllTrianglesWithoutIV_test( PRenderDataSource *renderDataSource )
// {
// 	PRenderStream *pRenderStream = renderDataSource->getStream(0);
// 	PDataBlock *vertexDatablock = pRenderStream->getDataBlock();
// 	float *parVertexData = (float*)vertexDatablock->getData();
// 
// 	unsigned int iCntVertex = vertexDatablock->getElementCount();
// 
// 
// 	unsigned int	iCntIdxVertices_est = (iCntVertex+1) * 3,
// 					iCntIdxVertices = 0,
// 					iSeq = 0, iIdxVertex = 0;
// 	vector<unsigned int>	vecIdxVertices;
// 	vecIdxVertices.reserve( iCntIdxVertices_est );
// 
// 	for( iIdxVertex=0, iSeq=0; iIdxVertex<iCntVertex; ++iIdxVertex,iSeq+=3 )
// 	{
// 		vecIdxVertices.push_back(iIdxVertex);
// 		vecIdxVertices.push_back(iIdxVertex+1);
// 		vecIdxVertices.push_back(iIdxVertex+2);
// 	}
// 	iCntIdxVertices = vecIdxVertices.size();
// 
// 	Vector3		v3CurrentVertex, v3TriVer[3];
// 	unsigned int ariTriIdx[3];
// 	unsigned int iCntTries = (iCntIdxVertices/3)+1;
// 	CTri		triCurrent;
// 	m_vecTris.reserve(iCntTries);
// 
// 	// For TESTDEBUG
// 	char			szoutputdebug[1024];
// 	unsigned int	uiBuf_=0;
// 	memset(szoutputdebug, 0, sizeof(char) * 1024);
// 	// For TESTDEBUG
// 
// 	for( unsigned int iSeqIdx=0, iTri=0; iSeqIdx<iCntIdxVertices; iSeqIdx+=3,++iTri )
// 	{
// 
// 
// 		if( 10038==iSeqIdx )
// 		{
// 			int iBreakPoint = 0;
// 		}
// 
// 		ariTriIdx[0] = vecIdxVertices[iSeqIdx];
// 		ariTriIdx[1] = vecIdxVertices[iSeqIdx+1];
// 		ariTriIdx[2] = vecIdxVertices[iSeqIdx+2];
// 
// 		// For TESTDEBUG
// 		sprintf( szoutputdebug, "(%d/%d) // %d, %d, %d // (%d/%d)\n", 
// 					iSeqIdx, 
// 					iCntIdxVertices, 
// 					ariTriIdx[0], 
// 					ariTriIdx[1], 
// 					ariTriIdx[2], 
// 					iTri, 
// 					iCntTries);
// 		OutputDebugString( szoutputdebug );
// 		// For TESTDEBUG
// 
// 		if( iTri > iCntTries )
// 		{
// 			int iBreakPoint = 0;
// 		}
// 
// 		for( int itV=0; itV<3; ++itV )
// 		{
// 			v3TriVer[itV].setX(*(parVertexData + (ariTriIdx[itV]*3)));
// 			v3TriVer[itV].setY(*(parVertexData + (ariTriIdx[itV]*3+1)));
// 			v3TriVer[itV].setZ(*(parVertexData + (ariTriIdx[itV]*3+2)));
// 
// 			if(v3TriVer[itV].getX()>m_bvAABB._v3Max.getX()) { m_bvAABB._v3Max.setX(v3TriVer[itV].getX()); }
// 			if(v3TriVer[itV].getY()>m_bvAABB._v3Max.getY()) { m_bvAABB._v3Max.setY(v3TriVer[itV].getY()); }
// 			if(v3TriVer[itV].getZ()>m_bvAABB._v3Max.getZ()) { m_bvAABB._v3Max.setZ(v3TriVer[itV].getZ()); }
// 
// 			if(v3TriVer[itV].getX()<m_bvAABB._v3Min.getX()) { m_bvAABB._v3Min.setX(v3TriVer[itV].getX()); }
// 			if(v3TriVer[itV].getY()<m_bvAABB._v3Min.getY()) { m_bvAABB._v3Min.setY(v3TriVer[itV].getY()); }
// 			if(v3TriVer[itV].getZ()<m_bvAABB._v3Min.getZ()) { m_bvAABB._v3Min.setZ(v3TriVer[itV].getZ()); }
// 		}
// 
// 		triCurrent.Set( v3TriVer );
// 		m_vecTris.push_back( triCurrent );
// 	}
// 
// 
// 	// For TESTDEBUG
// 	for ( unsigned int iSeqIdx=0; iSeqIdx<m_vecTris.size(); ++iSeqIdx )
// 	{
// 		CTri		&triCurr = m_vecTris[iSeqIdx];
// 
// 		uiBuf_ = uiBuf_ + sprintf(uiBuf_+szoutputdebug, "%d. Triangle P1(%f, %f, %f) \t",
// 			iSeqIdx,	triCurr.v3PT[0].getX(), 
// 			triCurr.v3PT[0].getY(), 
// 			triCurr.v3PT[0].getZ() );
// 
// 		uiBuf_ = uiBuf_ + sprintf(uiBuf_+szoutputdebug, "P2(%f, %f, %f) \t",
// 			triCurr.v3PT[1].getX(), 
// 			triCurr.v3PT[1].getY(), 
// 			triCurr.v3PT[1].getZ() );
// 
// 		uiBuf_ = uiBuf_ + sprintf(uiBuf_+szoutputdebug, "P3(%f, %f, %f) \n",
// 			triCurr.v3PT[2].getX(), 
// 			triCurr.v3PT[2].getY(), 
// 			triCurr.v3PT[2].getZ() );
// 
// 
// 		OutputDebugString( szoutputdebug );
// 		uiBuf_ = 0;
// 
// 	}
// 
// 	int iBreakPoint = 0;
// 	// For TESTDEBUG
// }


bool CTriContainer::getTri(const unsigned int iIdxTriangle, CTri &triangle)
{
	if( iIdxTriangle > m_vecTris.size() )	
	{
		assert("Accessed Index is out of range in Triangle Container" && 0);
		return false;
	}

	triangle = m_vecTris[iIdxTriangle];

	return true;
}

CTriContainer::CTriContainer()
{
	m_bvAABB._v3Max.setX(-FLT_MAX);
	m_bvAABB._v3Max.setY(-FLT_MAX);
	m_bvAABB._v3Max.setZ(-FLT_MAX);

	m_bvAABB._v3Min.setX(FLT_MAX);
	m_bvAABB._v3Min.setY(FLT_MAX);
	m_bvAABB._v3Min.setZ(FLT_MAX);
}

CTriContainer::~CTriContainer()
{

}