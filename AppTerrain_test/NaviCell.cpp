#include "NaviCell.h"

#ifdef _DEBUG_NAVIGATION

using namespace NAVIPART;

void CNaviCell::InitializeCell(const Vector3 *arv3TriPT, const Vector3 &v3NormalCell)
{
	memcpy( m_arrv3PT, arv3TriPT, sizeof(Vector3)*3 );
	m_v3NormalCell=v3NormalCell;
	computeCellData();
}

void CNaviCell::computeCellData()
{
	m_v3Center = 
		(m_arrv3PT[E_VERTEX_0]+m_arrv3PT[E_VERTEX_1]+m_arrv3PT[E_VERTEX_2])/3;

	m_arrv3MddlEdge[E_EDGE_01] = (m_arrv3PT[E_VERTEX_0]+m_arrv3PT[E_VERTEX_1])/2;
	m_arrv3MddlEdge[E_EDGE_12] = (m_arrv3PT[E_VERTEX_1]+m_arrv3PT[E_VERTEX_2])/2;
	m_arrv3MddlEdge[E_EDGE_20] = (m_arrv3PT[E_VERTEX_2]+m_arrv3PT[E_VERTEX_0])/2;

	float fdistEdgtoCntr_sum = 0.0f;
	for(int i=0;i<3;++i)
	{
		m_arrfDistEdgtoCntr[i]=fabs(length(m_arrv3MddlEdge[i]-m_v3Center));
		fdistEdgtoCntr_sum = fdistEdgtoCntr_sum + m_arrfDistEdgtoCntr[i];
	}

	m_fDistEdgetoCntr_avg = fdistEdgtoCntr_sum/3;
}

bool CNaviCell::setLinkcompute_(	const int iLevelFromGoal,
									const float fCostFromGoal,
									const int iSeqCellParent )
{
	if( CELL_STATUS_BLOCK==m_eStatusCell )
	{
		return true;
	}

	if( 0==iLevelFromGoal )	// This Cell is obviously Goal.
	{
		m_fcosttoGoal_avg	= 0.0f;
		m_fcosttoGoal_cen	= 0.0f;
		m_fcosttoGoal_ex	= 0.0f;
		m_iLevelFromGoal	= 0;
		m_eStatusCell		= CELL_STATUS_GOAL;
#ifdef _DEBUG
	#ifdef APPTARGET_IS_WIN
		OutputDebugString( "CNaviCell::setLinkcompute_() function set goal.\n" );
	#endif // APPTARGET_IS_WIN
		//assert__( m_iLevelFromGoal==0,__FILE__, __LINE__, __FUNCTION__, "" );
#endif // _DEBUG
		return true;
	}

	m_eStatusCell=CELL_STATUS_ROAD;

	if( E_EDGE_NULL!=m_eEdge_wayToGoal 
		&& m_arriAdjacentCells[m_eEdge_wayToGoal] == iSeqCellParent )
	{
#ifdef _DEBUG
		assert__( m_arriAdjacentCells[m_eEdge_wayToGoal]!=iSeqCellParent,__FILE__, __LINE__, __FUNCTION__, "" );
#endif // _DEBUG
		return false;
	}

	E_CELL_SIDEEDGE eEdgeBestWay = E_EDGE_NULL;
	for(int iE=0; iE<3; ++iE)
	{
		if(iSeqCellParent==m_arriAdjacentCells[iE])
		{
			if(m_fcosttoGoal_ex>fCostFromGoal)
			{
				eEdgeBestWay = (E_CELL_SIDEEDGE)iE;
				break;
			}
		}
	}

	if(E_EDGE_NULL==eEdgeBestWay)
	{
#ifdef _DEBUG
		// 더 짧은 Cost로 설정 된 경우.
		//assert__( 0, __FILE__, __LINE__, __FUNCTION__, "this navi has not any link." );
#endif // _DEBUG
		return true;
	}
	
	m_fcosttoGoal_ex = fCostFromGoal;
	m_eEdge_wayToGoal = eEdgeBestWay;

	m_fcosttoGoal_cen = m_fcosttoGoal_ex+m_arrfDistEdgtoCntr[m_eEdge_wayToGoal];
	m_fcosttoGoal_avg = m_fcosttoGoal_ex+m_fDistEdgetoCntr_avg;
	m_iLevelFromGoal = iLevelFromGoal;

	for(int iE=0; iE<3; ++iE)
	{
		if(eEdgeBestWay!=(E_CELL_SIDEEDGE)iE)
		{
			m_arrfcosttoGoal_edge[iE] = m_fcosttoGoal_cen+m_arrfDistEdgtoCntr[iE];
		}
	}

	return true;
}

void CNaviCell::setClear_unprocessed()
{
	for(int i=0; i<3; ++i)
	{
		m_arrfcosttoGoal_edge[i]=-1.0f;
	}

	m_iLevelFromGoal = NULL_LEVELFROMGOAL;
	m_eEdge_wayToGoal = E_EDGE_NULL;

	m_fcosttoGoal_ex = MAX_FLOAT;
	m_fcosttoGoal_cen = MAX_FLOAT;
	m_fcosttoGoal_avg = MAX_FLOAT;

#ifdef _DEBUG
	m_iAdjacented_debug = 0;
	m_bAdjacentProcessed_fordebug = false;
#endif // _DEBUG

	m_eStatusCell = CELL_STATUS_NULL;
}

const Vector3 &CNaviCell::getPos_BestWay()
{
#ifdef _DEBUG
	if(E_EDGE_NULL==m_eEdge_wayToGoal)
	{
		assert__(0,__FILE__, __LINE__, __FUNCTION__, "");
	}
#endif // _DEBUG
	return m_arrv3MddlEdge[m_eEdge_wayToGoal];
}

unsigned int CNaviCell::getIdxCellNext()
{
	return m_arriAdjacentCells[m_eEdge_wayToGoal];
}

bool CNaviCell::IsPosOnCell(const Vector3 &v3Pos)
{
	return BV::IsPointOnPlane(v3Pos, m_arrv3PT, 3, m_v3NormalCell);
}

bool CNaviCell::IsPosCrossBestEdge( const Vector3 &v3Pos )
{
	Vector3	v3DirMidtoCen = normalize(m_arrv3MddlEdge[m_eEdge_wayToGoal]-m_v3Center);
	Vector3 v3DirMidtoPos = normalize(m_arrv3MddlEdge[m_eEdge_wayToGoal]-v3Pos);

	if(dot( v3DirMidtoCen, v3DirMidtoPos)<0)
	{
		return true;
	}

	return false;
}

bool CNaviCell::DidSetAllAdjacent()
{
	for( int i=0; i<3; ++i )
	{
		if(NULL_CELL==m_arriAdjacentCells[i])
		{
			return false;
		}
	}

	return true;
}

void CNaviCell::Initialize()
{
	m_iSeq=NULL_CELL;

	for(int i=0; i<3; ++i)
	{
		m_arriAdjacentCells[i]=NULL_CELL;
		m_arrfcosttoGoal_edge[i]=-1.0f;
	}

	m_fDistEdgetoCntr_avg = 0.f;

	m_iLevelFromGoal = NULL_LEVELFROMGOAL;
	m_eEdge_wayToGoal = E_EDGE_NULL;

	m_fcosttoGoal_ex = MAX_FLOAT;
	m_fcosttoGoal_cen = MAX_FLOAT;
	m_fcosttoGoal_avg = MAX_FLOAT;

#ifdef _DEBUG
	m_iAdjacented_debug = 0;
	m_bAdjacentProcessed_fordebug = false;
#endif // _DEBUG

	m_pTri = NULL;
	m_eStatusCell = CELL_STATUS_NULL;
}

void CNaviCell::InitCell(int iSequence, TRIPART::CTri *pTri)
{
	m_iSeq=iSequence;
	m_pTri = pTri;
	InitializeCell( pTri->v3PT,pTri->v3FaceNormal );
}

CNaviCell::CNaviCell()
{
	Initialize();
}

CNaviCell::~CNaviCell()
{
	m_iSeq=NULL_CELL;
	m_pTri=NULL;
}

#endif // _DEBUG_NAVIGATION