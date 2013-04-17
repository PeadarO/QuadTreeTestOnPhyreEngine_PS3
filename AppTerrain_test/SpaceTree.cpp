#include "SpaceTree.h"

#ifdef _DEBUG_SPACETREE

bool CSpaceTree::constructTree(CTriContainer *pTriContainer_)
{
	if(pTriContainer_->getCountTries()<1) 
	{
		return false;
	}
	m_pTriContainer = pTriContainer_;
	m_pNodeRoot = allocNewNode( 0 );
	reserveNodeTableEstimate( m_vecNodeTable, m_pTriContainer );
	CreateRoot( m_vecNodeTable, m_pNodeRoot, pTriContainer_->m_vecTris );
	BuildupTree_recurse( m_vecNodeTable, m_pNodeRoot, pTriContainer_->m_vecTris, LIMITTRISPERNODE, 0 );

	return true;
}

void CSpaceTree::Release()
{
	for(unsigned int iNode=0; iNode<m_vecNodeTable.size(); ++iNode)
	{
		SPACETREENODE	*ptnode = m_vecNodeTable[iNode];

		m_pNodeRoot = NULL;

		SAFEDELETE(ptnode);
	}
}

SPACETREENODE *CSpaceTree::allocNewNode( int iDepthNode )
{
	SPACETREENODE	*pNewNode = new SPACETREENODE();

	for(int iChild=0; iChild<NUM_CHILDREN; ++iChild)
	{
		pNewNode->_iNodeChildIdx[iChild] = NULL_NODE;
	}

	for(int iNeighbor=0; iNeighbor<NUM_NEIGHBORS; ++iNeighbor)
	{
		pNewNode->_iNodeNeighborIdx[iNeighbor] = NULL_NODE;
	}

	pNewNode->_uiDepth = iDepthNode;

	return pNewNode;
}

void CSpaceTree::reserveNodeTableEstimate(vector<SPACETREENODE*> &vecNodeTable, CTriContainer *pTriContainer_)
{
	unsigned int uiCntEstNode = 0;
	uiCntEstNode = (pTriContainer_->getCountTries() / LIMITTRISPERNODE) * 4;
	vecNodeTable.reserve(uiCntEstNode);
}

void CSpaceTree::CreateRoot(vector<SPACETREENODE*> &vecNodeTable, SPACETREENODE *tnodeRoot, const vector<CTri>& vecTris)
{
	MakeUpBVInitializeBox(vecTris, tnodeRoot->_BVBoxSphere);

	for( int iIdxTri=0; iIdxTri<(int)vecTris.size(); ++iIdxTri )
	{
		tnodeRoot->_listSTris.insert(iIdxTri);
	}

	vecNodeTable.push_back(tnodeRoot);
}

void CSpaceTree::BuildupTree_recurse(	vector<SPACETREENODE*> &vecNodeTable, 
										SPACETREENODE *tnodeParent, 
										const vector<CTri>& vecTris, 
										unsigned int uiLimitTries,
										int iDepthNode )
{
	if(DEPTH_BUILDSPCTREE_MAX<=iDepthNode)
	{
		return;
	}

	vector<SPACETREENODE*>::iterator iterLast;

	if(tnodeParent->_listSTris.size() <= uiLimitTries)
	{
		return;
	}

	int iDepthNodeChld = iDepthNode+1;
	
	for(int iChild = 0;iChild<NUM_CHILDREN;++iChild)
	{	
		set<unsigned int>				listSTriChld_;
		set<unsigned int>::iterator		iter_;
		BV::_BV_AABB_Sphere				bvsphereChld_;
		unsigned int					iSeqTri = 0;
		bool							bintersectedTri = false;

		MakeupBVBoxSpaceTree( tnodeParent->_BVBoxSphere, bvsphereChld_, iChild );

		for(iter_=tnodeParent->_listSTris.begin();iter_!=tnodeParent->_listSTris.end();++iter_)
		{
			iSeqTri = static_cast<unsigned int>(*iter_);
			
			CTri &tri = (CTri&)(vecTris[iSeqTri]);
			
			if(IntersectTriBox( bvsphereChld_, tri ) == INSIDE)
			{
				listSTriChld_.insert( iSeqTri );
				bintersectedTri = true;
			}
		}

		if( true==bintersectedTri )
		{
			// insert New Node
			SPACETREENODE *pNewNodeChld = allocNewNode(iDepthNodeChld);
			bool bNodeChldIsLeaf = (listSTriChld_.size()<=uiLimitTries);
			
			pNewNodeChld->_bLeaf = bNodeChldIsLeaf;
			pNewNodeChld->setBV( bvsphereChld_ );
			pNewNodeChld->setlistTri( listSTriChld_ );	listSTriChld_.clear();		

			vecNodeTable.push_back(pNewNodeChld);
			tnodeParent->_iNodeChildIdx[iChild] = vecNodeTable.size()-1;

			if(false==bNodeChldIsLeaf)
			{
				BuildupTree_recurse( vecNodeTable, pNewNodeChld, vecTris, uiLimitTries, iDepthNodeChld );
			}
		}
	}
}

void CSpaceTree::MakeUpBVInitializeBox(const vector<CTri> &vecTris, BV::_BV_AABB_Sphere &BBox)
{
	Vector3	v3Max, v3Min;
	int	    i,j, iTriCnt = vecTris.size();

	for( i=0; i<3; ++i )
	{
		v3Max.setElem( i, -(float)FLT_MAX );
	}

	for( i=0; i<3; ++i )
	{
		v3Min.setElem( i, (float)FLT_MAX );
	}

	for(i= 0; i < iTriCnt; i++)
	{
		CTri &Tri= (CTri&)(vecTris[i]);

		for(j= 0; j < 3; j++)
		{
			if(Tri.v3PT[j].getX() > v3Max.getX())
				v3Max.setX(Tri.v3PT[j].getX());

			if(Tri.v3PT[j].getY() > v3Max.getY())
				v3Max.setY(Tri.v3PT[j].getY());

			if(Tri.v3PT[j].getZ() > v3Max.getZ())
				v3Max.setZ(Tri.v3PT[j].getZ());


			if(Tri.v3PT[j].getX() < v3Min.getX())
				v3Min.setX(Tri.v3PT[j].getX());

			if(Tri.v3PT[j].getY() < v3Min.getY())
				v3Min.setY(Tri.v3PT[j].getY());

			if(Tri.v3PT[j].getZ() < v3Min.getZ())
				v3Min.setZ(Tri.v3PT[j].getZ());
		}
	}

	BBox._v3Max.setX( v3Max.getX()+5.0f );
	BBox._v3Max.setY( v3Max.getY()+5.0f );
	BBox._v3Max.setZ( v3Max.getZ()+5.0f );

	BBox._v3Min.setX( v3Min.getX()-5.0f );
	BBox._v3Min.setY( v3Min.getY()-5.0f );
	BBox._v3Min.setZ( v3Min.getZ()-5.0f );
}

void CSpaceTree::MakeupBVBoxSpaceTree(const BV::_BV_AABB_Sphere &boxParent, BV::_BV_AABB_Sphere &boxChild, int iSeqPos)
{
	Vector3 BoxSize;
	Vector3 Center;
	Vector3 NewCenter;

	Center.setX((boxParent._v3Max.getX() + boxParent._v3Min.getX()) / 2);
	Center.setY((boxParent._v3Max.getY() + boxParent._v3Min.getY()) / 2);
	Center.setZ((boxParent._v3Max.getZ() + boxParent._v3Min.getZ()) / 2);

	BoxSize.setX((boxParent._v3Max.getX() - boxParent._v3Min.getX()));
	BoxSize.setY((boxParent._v3Max.getY() - boxParent._v3Min.getY()));
	BoxSize.setZ((boxParent._v3Max.getZ() - boxParent._v3Min.getZ()));

	if(iSeqPos % 2 == 0)
		NewCenter.setX((Center.getX() - (BoxSize.getX() / 4)));
	else
		NewCenter.setX((Center.getX() + (BoxSize.getX() / 4)));


	if(iSeqPos== 2 || iSeqPos== 3)
		NewCenter.setZ((Center.getZ() - (BoxSize.getZ() / 4)));
	else
		NewCenter.setZ((Center.getZ() + (BoxSize.getZ() / 4)));

	boxChild._v3Min.setX((NewCenter.getX() - BoxSize.getX() / 4));
	boxChild._v3Min.setY(boxParent._v3Min.getY());
	boxChild._v3Min.setZ((NewCenter.getZ() - BoxSize.getZ() / 4));


	boxChild._v3Max.setX((NewCenter.getX() + BoxSize.getX() / 4));
	boxChild._v3Max.setY(boxParent._v3Max.getY());
	boxChild._v3Max.setZ((NewCenter.getZ() + BoxSize.getZ() / 4));
}


// @ Ray로 충돌 Triangle 가져오기
bool CSpaceTree::rayIntersectCollectedTrisinTree(	const BV::_Ray &ray, 
													vector<TRIIDX_> &listTrisFound )
{
	set<SPACETREENODE*>					slistNodesFound;
	set<SPACETREENODE*>::iterator		iterNode;

	bool bfoundedNode = rayIntersectcollectNodes( ray, slistNodesFound );
	bool bfoundedTris = false;

	if( bfoundedNode )
	{
		for( iterNode=slistNodesFound.begin(); iterNode != slistNodesFound.end(); ++iterNode )
		{
			SPACETREENODE* ptnodeCurrent = (SPACETREENODE*)(*iterNode);
			bfoundedTris |= rayIntersectCollectTrisInNodes( ray, 
														m_pTriContainer->getTris(), 
														ptnodeCurrent,
														listTrisFound );
		}
	}

	return bfoundedTris;
}

// @ Ray로 충돌 Triangle 가져오기
bool CSpaceTree::rayIntersectcollectNodes( const BV::_Ray &ray, set<SPACETREENODE*> &slistNodeFound)
{
	SPACETREENODE *pRootNode = m_vecNodeTable[0];

	return rayIntersectCollectNodesRecurse( ray, pRootNode, m_vecNodeTable, slistNodeFound );
}

bool CSpaceTree::rayIntersectCollectNodesRecurse(	const BV::_Ray &ray, 
													const SPACETREENODE *pNodeParent, 
													const vector<SPACETREENODE*> &vecNodeTable, 
													set<SPACETREENODE*> &slistNodeFound )
{
	unsigned int uiSizeNodeTable = vecNodeTable.size(), iSeqNode;
	bool bfoundleastOnemore = false, bfound = false;

	for( int iNode=0; iNode<NUM_CHILDREN; ++iNode )
	{
		if( NULL_NODE==pNodeParent->_iNodeChildIdx[iNode] )
		{
			continue;
		}
		iSeqNode=pNodeParent->_iNodeChildIdx[iNode];

		if( uiSizeNodeTable <= iSeqNode )
		{
			// Problem
			assert( "Out of range NodeTable Accessed" && 0 );
		}

		SPACETREENODE *pnodecurrent = vecNodeTable[pNodeParent->_iNodeChildIdx[iNode]];
		
		if( pnodecurrent )
		{
			bool bResultIntersect = IntersectInfiniteRayAABB(ray, pnodecurrent->_BVBoxSphere );
			bool bLeafNode = pnodecurrent->_bLeaf;

			if(true==bResultIntersect)
			{
				// 노드 말단이면 현재노드를 추가하고 다음 자식 노드로 GO.
				// 노드 말단이 아니면 현재노드의 자식 노드들 검색
				if(true==bLeafNode)
				{
					slistNodeFound.insert( pnodecurrent );
					bfound = true;
				}
				else
				{
					bfound = rayIntersectCollectNodesRecurse( ray, pnodecurrent, vecNodeTable, slistNodeFound );
				}

				bfoundleastOnemore |= bfound;
			}
		}
	}

	return bfoundleastOnemore;
}

bool CSpaceTree::rayIntersectCollectTrisInNodes(	const BV::_Ray &ray, 
													const TRIPART::VEC_TRIS &vecTris,
													SPACETREENODE* ptnode, 
													vector<TRIIDX_> &listTrisFound )
{
	unsigned int				uiCurrentTri = 0;
	bool	bIntersected=false, bCollected=false, bCollectedIntersectTriOneMore=false, bExistedAlready=false;
	float	fDistIntersect = 0.0f;
	set<unsigned int>::iterator		itertIdx;
	int		iCntTriFound=0;

	for( itertIdx = ptnode->_listSTris.begin(); itertIdx!=ptnode->_listSTris.end(); ++itertIdx )
	{
		uiCurrentTri = (*itertIdx);

		CTri &triCurrent = (CTri&)vecTris[uiCurrentTri];
		bIntersected = BV::IntersectRayTri( ray, triCurrent.v3PT[0], triCurrent.v3PT[1], triCurrent.v3PT[2], &fDistIntersect );

		bExistedAlready=false; bCollected=false;
		if(	true==bIntersected 
			&& 0<fDistIntersect )
		{
			TRIIDX_ triIdxForSort;
			triIdxForSort.fDistance = fDistIntersect;
			triIdxForSort.uiTriIdx = uiCurrentTri;

			// @Insert only Unique Tri
			iCntTriFound=(int)(listTrisFound.size());
			for(int iTriFnd=0; iTriFnd<iCntTriFound; ++iTriFnd)
			{
				if(uiCurrentTri==listTrisFound[iTriFnd].uiTriIdx)
				{
					bExistedAlready=true;
					break;
				}
			}
			
			if(false==bExistedAlready)
			{
				bCollected=true;
				listTrisFound.push_back( triIdxForSort );
			}
		} // if(true == bIntersected)

		bCollectedIntersectTriOneMore |= bCollected;
	} // for( itertIdx = ptnode->_listSTris.begin(); itertIdx!=ptnode->_listSTris.end(); ++itertIdx )
	
	return bCollectedIntersectTriOneMore;
}


void CSpaceTree::rayIntersectFindNearstTri( vector<TRIIDX_> &listTrisFound, TRIIDX_ &triFindout )
{
	vector<TRIIDX_>::iterator	iterBegin	= listTrisFound.begin(), 
								iterEnd		= listTrisFound.end();

	if( listTrisFound.size()>1 )
	{
		//listTrisFound.sort();
		sort(iterBegin, iterEnd);
	}

	vector<TRIIDX_>::iterator iterTri = listTrisFound.begin();
	TRIIDX_ &triSpot = (*iterTri);

	memcpy(&triFindout, &triSpot, sizeof(TRIIDX_));
}



bool CSpaceTree::rayIntersectGetPos( const Vector3 &v3Position, 
										const Vector3 &v3Raydirection, 
										Vector3 &v3IntersectPos_out )
{
	BV::_Ray		ray;
	ray.set( v3Position, v3Raydirection, true);

	return rayIntersectGetPos( ray, v3IntersectPos_out );
}

bool CSpaceTree::rayIntersectGetPos( const BV::_Ray &ray, 
									   Vector3 &v3IntersectPos_out )

{	
	vector<TRIIDX_>		listTrifound;
	bool bfound = rayIntersectCollectedTrisinTree( ray, listTrifound );

	if(true==bfound)
	{
		TRIIDX_ triNearst;

		if( listTrifound.size() > 1 )
		{
			rayIntersectFindNearstTri( listTrifound, triNearst );
		}
		else
		{
			vector<TRIIDX_>::iterator	iterTri = listTrifound.begin();
			triNearst = (*iterTri);
		}

		BV::getPosIntersectedRay( ray._v3Origin, ray._v3Direction, triNearst.fDistance, v3IntersectPos_out );
	}

	return bfound;
}

bool CSpaceTree::rayIntersectGetPos( const BV::_Ray &ray, 
									    CTri &triPicked_out,
										Vector3 &v3IntersectPos_out )
{
	vector<TRIIDX_>		listTrifound;
	bool bfound = rayIntersectCollectedTrisinTree( ray, listTrifound );

	if(true==bfound)
	{
		Vector3	v3PosIntersect;
		TRIIDX_ triNearst;

		if( listTrifound.size() > 1 )
		{
			rayIntersectFindNearstTri( listTrifound, triNearst );
		}
		else
		{
			vector<TRIIDX_>::iterator	iterTri = listTrifound.begin();
			triNearst = (*iterTri);
		}

		if(triNearst.uiTriIdx > m_pTriContainer->getCountTries())
		{
#ifdef _DEBUG
			assert( 0 && "Picked Intersect Triangle Index is out of range of Triangle Container.!" );
#endif // _DEBUG
			return false;
		}

		bool bFoundTri = m_pTriContainer->getTri(triNearst.uiTriIdx, triPicked_out);
		if(!bFoundTri)
		{
#ifdef _DEBUG
			assert( 0 && "Picked Intersect Triangle Index is out of range of Triangle Container.!" );
#endif // _DEBUG
			return false;
		}
	
		BV::getPosIntersectedRay( ray._v3Origin, ray._v3Direction, triNearst.fDistance, v3IntersectPos_out );

		return true;
	}

	return false;
}


bool CSpaceTree::rayIntersectGetIdxTri( const BV::_Ray &ray, int &iIdxTri )
{
	vector<TRIIDX_>		listTrifound;

	bool bfound = rayIntersectCollectedTrisinTree( ray, listTrifound );

	if(true==bfound)
	{
		TRIIDX_ triNearst;

		if( listTrifound.size() > 1 )
		{
			rayIntersectFindNearstTri( listTrifound, triNearst );
		}
		else
		{
			vector<TRIIDX_>::iterator	iterTri = listTrifound.begin();
			triNearst = (*iterTri);
		}

		iIdxTri = triNearst.uiTriIdx;

		return true;
	}

	return false;
}


#if defined(_DEBUG_SPACETREE) && defined(_DEBUGFOR_RENDER)
void CSpaceTree::renderAllNodes_debug(	const Vector3	&v3colorLine,
										const Vectormath::Aos::Matrix4 *matViewPrjCamera )
{
	renderAllNodesRecurse_debug( ROOT_NODE, v3colorLine, matViewPrjCamera );
}

void CSpaceTree::renderAllNodesRecurse_debug(	const int iIdxParentNode,
												const Vector3	&v3colorLine,
												const Vectormath::Aos::Matrix4 *matViewPrjCamera )
{
	SPACETREENODE	*pParentNode = m_vecNodeTable[iIdxParentNode];

	if(iIdxParentNode>=(int)m_vecNodeTable.size())
	{
		return;
	}

	if( false==pParentNode->_bLeaf )
	{
		for( int iSeqChld=0; iSeqChld<NUM_CHILDREN; ++iSeqChld )
		{
			int iSeqChldNode = pParentNode->_iNodeChildIdx[iSeqChld];

			if(NULL_NODE==iSeqChldNode)
			{
				continue;
			}

			renderAllNodesRecurse_debug( iSeqChldNode, v3colorLine, matViewPrjCamera );
		}
	}

	renderoutLineBV(pParentNode->_BVBoxSphere, v3colorLine, matViewPrjCamera);
}

void CSpaceTree::renderoutLineBV(		const BV::_BV_AABB_Sphere	&_BVBoxSphere, 
										const Vector3				&v3color,
										const Vectormath::Aos::Matrix4 *matViewPrjCamera )
{
	Vector3	v3MinBV, v3MaxBV;
	Vector3 v3Bottom[4], v3Up[4];

	v3MinBV = _BVBoxSphere._v3Min;
	v3MaxBV = _BVBoxSphere._v3Max;

	v3Bottom[0].setX( v3MinBV.getX() );
	v3Bottom[0].setY( v3MinBV.getY() );
	v3Bottom[0].setZ( v3MaxBV.getZ() );

	v3Bottom[1].setX( v3MaxBV.getX() );
	v3Bottom[1].setY( v3MinBV.getY() );
	v3Bottom[1].setZ( v3MaxBV.getZ() );

	v3Bottom[2].setX( v3MaxBV.getX() );
	v3Bottom[2].setY( v3MinBV.getY() );
	v3Bottom[2].setZ( v3MinBV.getZ() );

	v3Bottom[3] = v3MinBV;



	v3Up[0].setX( v3MinBV.getX() );
	v3Up[0].setY( v3MaxBV.getY() );
	v3Up[0].setZ( v3MaxBV.getZ() );

	v3Up[1]	= v3MaxBV;

	v3Up[2].setX( v3MaxBV.getX() );
	v3Up[2].setY( v3MaxBV.getY() );
	v3Up[2].setZ( v3MinBV.getZ() );

	v3Up[3].setX( v3MinBV.getX() );
	v3Up[3].setY( v3MaxBV.getY() );
	v3Up[3].setZ( v3MinBV.getZ() );


	for( int i=0; i<= 3; ++i )
	{
// 		m_pdrawFordebug->renderLine( v3Bottom[i], v3Up[i], v3color, matViewPrjCamera);
// 
// 		int iS = (i+3)%4;
// 		int iE = i;
// 		m_pdrawFordebug->renderLine( v3Bottom[iS], v3Bottom[iE], v3color, matViewPrjCamera);
// 		m_pdrawFordebug->renderLine( v3Up[iS], v3Up[iE], v3color, matViewPrjCamera);

		m_pdrawFordebug->renderCube( v3Up, v3Bottom, v3color, matViewPrjCamera );
	}
}
#endif // defined(_DEBUG_SPACETREE) && defined(_DEBUGFOR_RENDER)


void CSpaceTree::validateNodeConstructed_Recurse( SPACETREENODE *pParentNode, set<unsigned int> &listSTris )
{
	for(int iChldSeq=0; iChldSeq<NUM_CHILDREN; ++iChldSeq)
	{
		int iChldNode = pParentNode->_iNodeChildIdx[iChldSeq];
		if(NULL_NODE!=iChldNode)
		{
			SPACETREENODE *pchldNodeCurr = m_vecNodeTable[iChldNode];

			if(true==pchldNodeCurr->_bLeaf)
			{
				if(0<pchldNodeCurr->_listSTris.size())
				{
					set<unsigned int>::iterator		iterTri;

					for( iterTri=pchldNodeCurr->_listSTris.begin();iterTri!=pchldNodeCurr->_listSTris.end(); iterTri++ )
					{
						listSTris.insert(*iterTri);
					}
				}
			}
			else
			{
				validateNodeConstructed_Recurse( pchldNodeCurr, listSTris );
			}	// if(false==pchldNodeCurr->_bLeaf)

		}	// if(NULL_NODE!=iChldNode)

	}	// for(int iChldSeq=0; iChldSeq<NUM_CHILDREN; ++iChldSeq)
}


CSpaceTree::CSpaceTree(void)
{
	m_pNodeRoot = NULL;
	m_pTriContainer = NULL;

#ifdef _DEBUGFOR_RENDER
	m_pdrawFordebug = NULL;
	m_pdrawFordebug = m_pdrawFordebug->getthis();
#endif // _DEBUGFOR_RENDER
}

CSpaceTree::~CSpaceTree(void)
{
}

#endif // _DEBUG_SPACETREE
