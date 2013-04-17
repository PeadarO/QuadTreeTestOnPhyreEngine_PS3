#pragma once

#include <PSSG.h>

#include "CommonApp.h"

#ifdef _DEBUG_NAVIGATION
#include "BoundaryVolume.h"
#include "NaviCell.h"
#include "Tri.h"
#include "TriContainer.h"
#include <set>

using namespace PSSG;
using namespace Vectormath::Aos;

namespace NAVIPART
{
	#define NULL_NAVICONTAINER	-1

	using namespace TRIPART;

	typedef vector<CNaviCell>	VEC_NAVICELL;

	class CProcessorNavi
	{
	public:
		int							m_iCellGoal;
		int							m_iSeqNavis;
		int							m_iSeqLevelTotal;

		bool						m_bDidBuildGoal;

	private:
		VEC_NAVICELL				m_vecNavicells;

		// @ Process
	public:
		void Initialize(	CTriContainer &triContainer );
		void Release();

		
		// @ Get / Set
	public:
		bool isGoalCell( int iSeqCell )
		{
			return m_vecNavicells[iSeqCell].IsGoalCell();
		}

		int getLevelCells()	{return m_iSeqLevelTotal;};
		bool didBuildupGoal()	{ return m_bDidBuildGoal; };

		const vector<CNaviCell>	&getCells()	{ return m_vecNavicells; };
		unsigned int getCountCells()		{ return m_vecNavicells.size(); };
		bool getCell( const unsigned int iIdxCell, CNaviCell &naveCell);
		CNaviCell &getCell( const unsigned int iIdxCell ) { return m_vecNavicells[iIdxCell]; };

		bool getPosBestWayOnCell(	const int iSeqCell_in, Vector3 &v3Pos_out )
		{
#ifdef _DEBUG
			if(iSeqCell_in>(int)m_vecNavicells.size()||iSeqCell_in<0)
			{
				assert__(0,__FILE__,__LINE__,__FUNCTION__, "");
			}
#endif // _DEBUG

			CNaviCell &naviCell = m_vecNavicells[iSeqCell_in];
			if( CELL_STATUS_BLOCK==naviCell.m_eStatusCell)
			{
				return false;
			}

#ifdef _DEBUG
			if(	NULL_LEVELFROMGOAL==naviCell.m_iLevelFromGoal )
			{
				assert__(0,__FILE__,__LINE__,__FUNCTION__, "");
			}
#endif // _DEBUG

			v3Pos_out = naviCell.getPos_BestWay();
			return true;
		};

		// @ Function Private
	private:
		void buildupAdjacentAll(	VEC_NAVICELL &vecNaviCells );
		bool checkAllreadyAdj(	CNaviCell &cellCheck );
		void setAdjacentATri(	CNaviCell &cellLeft,
								VEC_NAVICELL &vecNaviCells );
		E_CELL_SIDEEDGE getTypeEdge(	const unsigned int uiSharedNum );
		void ConstructTrisToCells( CTriContainer &triContainer );
		void setClearAllcell_unprocessed(	VEC_NAVICELL &vecNaviCells );

#if defined(DEBUG_CEHCK_VALIDATION_NAVIGATION) && defined(APPTARGET_IS_WIN)
		void validateAllAdjcntCells(	VEC_NAVICELL &vecNavicells );
		void validateAdjcntRecurse(		int iSeqCell, VEC_NAVICELL &vecNavicells );
#endif // defined(DEBUG_CEHCK_VALIDATION_NAVIGATION) && defined(APPTARGET_IS_WIN)


		// @ Construct Navigation

		// No Recursion
		void mappingNavigation__( VEC_NAVICELL &vecNaviCells, set<int> &slistCellsGoal );

		// Recursion
		void mappingNavigationRecurse(	VEC_NAVICELL &vecNaviCells, 
										set<int> &slistCellsAdj,
										const int iLevelFromGoal = 0 );

		// @ process
	public:
		void setClearAllcells()
		{
			setClearAllcell_unprocessed(m_vecNavicells);
		}
		bool constructMappingNavigation( set<int> &slistCellsGoal, set<int> &slistCellsBlock );

	public:
		CProcessorNavi();
		~CProcessorNavi();
	};

}

#endif // _DEBUG_NAVIGATION
