#pragma once


#include "CommonApp.h"

#ifdef _DEBUG_NAVIGATION

#include <PSSG.h>

#include "Tri.h"
#include "BoundaryVolume.h"

namespace NAVIPART
{
#define NULL_CELL			-1
#define BLOCK_CELL			-2

#define NULL_LEVELFROMGOAL	-1

	enum E_CELL_STATUS
	{
		CELL_STATUS_NULL=-1,
		CELL_STATUS_ROAD,
		CELL_STATUS_BLOCK,
		CELL_STATUS_GOAL,
		CELL_STATUS_CNT,
	};

	enum E_CELL_VERTEX
	{
		E_VERTEX_0=0,
		E_VERTEX_1,
		E_VERTEX_2,
		E_VERTEX_CNT,
	};

	enum E_CELL_SIDEEDGE
	{
		E_EDGE_NULL=-1,
		E_EDGE_01=0,
		E_EDGE_12,
		E_EDGE_20,
		E_EDGE_CNT,
	};

	class CNaviCell
	{
	public:
		// @ Internal Values
		Vector3				m_arrv3PT[E_VERTEX_CNT];
		Vector3				m_v3Center;
		Vector3				m_arrv3MddlEdge[E_EDGE_CNT];
		Vector3				m_v3NormalCell;
		float				m_arrfDistEdgtoCntr[E_EDGE_CNT];
		float				m_fDistEdgetoCntr_avg;

		// @ Relation Values
		int					m_iSeq;
		int					m_iLevelFromGoal;
		E_CELL_SIDEEDGE		m_eEdge_wayToGoal;
		float				m_fcosttoGoal_ex;					// 역진입점 까지 비용
		float				m_fcosttoGoal_cen;					// 역진입점 부터 중심까지 실비용
		float				m_fcosttoGoal_avg;					// 역진입점 부터 각 변과의 거리의 평균 비용
		float				m_arrfcosttoGoal_edge[E_EDGE_CNT];	// 역진입점-중심-각진입점들 까지 거리.
		TRIPART::CTri		*m_pTri;							// Adjacent Tri 설정 할때 필요
		E_CELL_STATUS		m_eStatusCell;
		int					m_arriAdjacentCells[E_EDGE_CNT];
		

#ifdef _DEBUG
		int					m_iAdjacented_debug;
		bool				m_bAdjacentProcessed_fordebug;
#endif // _DEBUG

	private:
		void InitializeCell(const Vector3	*arv3TriPT, const Vector3 &v3NormalCell);
		void computeCellData();

	public:
		// @ Get / Set
		bool setLinkcompute_(	const int iLevelFromGoal,
								const float fCostFromGoal,
								const int iSeqCellParent );

		void setClear_unprocessed();

		const Vector3 &getPos_BestWay();

		unsigned int getIdxCellNext();

		void SetAdjacentCell(	E_CELL_SIDEEDGE eSideedgeType, int iSeqCellAdjacent )
		{
			m_arriAdjacentCells[eSideedgeType] = iSeqCellAdjacent;
		};

		void SetBlockCell()	{	m_eStatusCell=CELL_STATUS_BLOCK; };

		// @ Request / Response
		bool IsPosOnCell(const Vector3 &v3Pos);
		bool IsPosCrossBestEdge( const Vector3 &v3Pos );
		bool DidSetAllAdjacent();
		bool IsBlockCell() { return m_eStatusCell==CELL_STATUS_BLOCK; };
		bool IsGoalCell() { return m_eStatusCell==CELL_STATUS_GOAL; };
		//bool IsGoalCell() { return (m_iLevelFromGoal==0); };

		// @ Process
		void Initialize();
		void InitCell(int iSequence, TRIPART::CTri *pTri);

	public:
		CNaviCell();
		~CNaviCell();
	};

};	// namespace NAVIPART

#endif // _DEBUG_NAVIGATION
