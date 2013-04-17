#pragma once

#include "CommonApp.h"

#ifdef _DEBUG_SPACETREE

#include "TriContainer.h"
#include "BoundaryVolume.h"

#ifdef _DEBUGFOR_RENDER
#include "DrawForDebug.h"
#endif // _DEBUGFOR_RENDER


#include <set>

#define LIMITTRISPERNODE		12

#define NUM_CHILDREN			4
#define NUM_NEIGHBORS			4
#define NULL_NODE				-1

#define ROOT_NODE				0

#define DEPTHNODE_INIT			-1

#define DEPTH_BUILDSPCTREE_MAX	8

using namespace TRIPART;

struct TRIIDX_
{
	unsigned int uiTriIdx;
	float fDistance;

	bool operator < (const TRIIDX_ &rhs)
	{
		return (fDistance < rhs.fDistance);
	};

public:
	TRIIDX_()
	{
		uiTriIdx = 0;
		fDistance = 0.0f;
	}

	~TRIIDX_()
	{

	}
};

struct SPACETREENODE
{
	BV::_BV_AABB_Sphere	_BVBoxSphere;
	int					_iNodeChildIdx[NUM_CHILDREN];
	int					_iNodeNeighborIdx[NUM_NEIGHBORS];
	set<unsigned int>	_listSTris;
	bool				_bLeaf;

	int					_uiDepth;

	void setBV( const BV::_BV_AABB_Sphere &aabbSphere )
	{
		memcpy( &_BVBoxSphere, &aabbSphere, sizeof(BV::_BV_AABB_Sphere) );
	}

	void setlistTri( set<unsigned int> &listTris )
	{
		if( _listSTris.size()>0 )
		{
			_listSTris.clear();
		}

		set<unsigned int>::iterator		iterTri;
		for( iterTri=listTris.begin();iterTri!=listTris.end(); iterTri++ )
		{
			_listSTris.insert(*iterTri);
		}
	}

	SPACETREENODE()
	{
		memset(_iNodeChildIdx, 0, sizeof(int)*NUM_CHILDREN);
		memset(_iNodeNeighborIdx, 0, sizeof(int)*NUM_NEIGHBORS);

		_bLeaf = false;

		_uiDepth = DEPTHNODE_INIT;
	};

	~SPACETREENODE()
	{
 		if( _listSTris.size()>0 )
 		{
 			_listSTris.clear();
 		}
	}
};

class CSpaceTree : public CSingleton< CSpaceTree >
{
public:
	vector<SPACETREENODE*>	m_vecNodeTable;
	SPACETREENODE			*m_pNodeRoot;
	CTriContainer			*m_pTriContainer;

#ifdef _DEBUGFOR_RENDER
	CDrawForDebug			*m_pdrawFordebug;
#endif // _DEBUGFOR_RENDER
	
public:
	// @ Build up TREE
	bool constructTree(CTriContainer *pTriContainer_);
	void Release();

private:
	// @ Allocation
	SPACETREENODE *allocNewNode(int iDepthNode=DEPTHNODE_INIT);

	// @ Build up TREE
	void reserveNodeTableEstimate(	vector<SPACETREENODE*> &vecNodeTable, 
									CTriContainer *pTriContainer_);
	void CreateRoot(	vector<SPACETREENODE*> &vecNodeTable, 
						SPACETREENODE *tnodeRoot, 
						const vector<CTri>& vecTris );
	void BuildupTree_recurse(	vector<SPACETREENODE*> &vecNodeTable, 
								SPACETREENODE *tnodeParent, 
								const vector<CTri>& vecTris, 
								unsigned int uiLimitTries=LIMITTRISPERNODE, 
								int iDepthNode = DEPTHNODE_INIT );

	// @ Make BV Box Per NODE
	void MakeUpBVInitializeBox(	const vector<CTri> &vecTris, 
								BV::_BV_AABB_Sphere &BBox);
	void MakeupBVBoxSpaceTree(	const BV::_BV_AABB_Sphere &boxParent, 
								BV::_BV_AABB_Sphere &boxChild, 
								int iSeqPos);

	// @ Find Triangle
	bool rayIntersectCollectedTrisinTree(	const BV::_Ray &ray, 
											vector<TRIIDX_> &listTrisFound );

	bool rayIntersectcollectNodes(	const BV::_Ray &ray, 
									set<SPACETREENODE*> &slistNodeFound);
	bool rayIntersectCollectNodesRecurse(	const BV::_Ray &ray, 
											const SPACETREENODE *pNodeParent, 
											const vector<SPACETREENODE*> &vecNodeTable, 
											set<SPACETREENODE*> &slistNodeFound);
	bool rayIntersectCollectTrisInNodes(	const BV::_Ray &ray, 
											const TRIPART::VEC_TRIS &vecTris, 
											SPACETREENODE* ptnode, 
											vector<TRIIDX_> &listTrisFound );

	void rayIntersectFindNearstTri(	vector<TRIIDX_> &listTrisFound,
									TRIIDX_ &triFindout );
public:
	// @ rayIntersect Recursive
	bool rayIntersectGetPos( const Vector3 &v3Position, const Vector3 &v3Raydirection, Vector3 &v3IntersectPos_out );
	bool rayIntersectGetPos( const BV::_Ray &ray, Vector3 &v3IntersectPos_out );
	bool rayIntersectGetPos( const BV::_Ray &ray, CTri &triPicked_out, Vector3 &v3IntersectPos_out );
	bool rayIntersectGetIdxTri( const BV::_Ray &ray, int &iIdxTri );
	

	// @ Get Set
	TRIPART::CTri *getTri( unsigned int uiIdxTri )
	{
		if(uiIdxTri>=m_pTriContainer->getCountTries())
		{
			return NULL;
		}

		return &m_pTriContainer->getTri(uiIdxTri);
	}
	
#if defined(_DEBUG_SPACETREE) && defined(_DEBUGFOR_RENDER)
	// @ For DEBUG
	void renderAllNodes_debug(	const Vector3	&v3colorLine,
								const Vectormath::Aos::Matrix4 *matViewPrjCamera );

	void renderAllNodesRecurse_debug(	const int iIdxParentNode,
										const Vector3	&v3colorLine,
										const Vectormath::Aos::Matrix4 *matViewPrjCamera );

	void renderoutLineBV(		const BV::_BV_AABB_Sphere	&_BVBoxSphere, 
								const Vector3				&v3color,
								const Vectormath::Aos::Matrix4 *matViewPrjCamera );
#endif // defined(_DEBUG_SPACETREE) && defined(_DEBUGFOR_RENDER)

	void validateNodeConstructed_Recurse( SPACETREENODE *pParentNode, set<unsigned int> &listSTris );

public:
	CSpaceTree(void);
	~CSpaceTree(void);
};

#endif // _DEBUG_SPACETREE
