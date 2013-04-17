
#include "ProcessorNavi.h"

#ifdef _DEBUG_NAVIGATION

using namespace NAVIPART;

void CProcessorNavi::Initialize( CTriContainer &triContainer )
{
	m_iCellGoal = NULL_CELL;
	m_iSeqLevelTotal = -1;
	ConstructTrisToCells( triContainer );

	m_bDidBuildGoal = false;
}

void CProcessorNavi::Release()
{
	m_vecNavicells.clear();
}

void CProcessorNavi::buildupAdjacentAll( VEC_NAVICELL &vecNaviCells )
{
	VEC_NAVICELL::iterator	it_, iterBegin, iterEnd;
	iterBegin = vecNaviCells.begin();
	iterEnd = vecNaviCells.end();

	for( it_=iterBegin; it_!=iterEnd ;++it_ )
	{
		CNaviCell &naviCell = (*it_);

		setAdjacentATri(naviCell, vecNaviCells);
	}

#if defined(DEBUG_CEHCK_VALIDATION_NAVIGATION) && defined(APPTARGET_IS_WIN)
	validateAllAdjcntCells(vecNaviCells);
#endif // defined(DEBUG_CEHCK_VALIDATION_NAVIGATION) && defined(APPTARGET_IS_WIN)
}

bool CProcessorNavi::checkAllreadyAdj(	CNaviCell &cellCheck )
{
	for(int i=0; i<(int)E_EDGE_CNT; ++i)
	{
		if(NULL_CELL==cellCheck.m_arriAdjacentCells[i])
			return false;
	}

	return true;
}

void CProcessorNavi::setAdjacentATri(	CNaviCell &cellLeft,
									VEC_NAVICELL &vecNaviCells )
{
	if(true==checkAllreadyAdj(cellLeft))
	{
		return;
	}

	VEC_NAVICELL::iterator	it_, iterBegin, iterEnd;
	unsigned int *aruiIVleft = cellLeft.m_pTri->uiIV, *aruiLVRight;
	bool bFoundShared = false;
	int iFoundShared = 0;
	int ariPointsShared_left[3], ariPointsShared_right[3];
//	E_CELL_SIDEEDGE	eCellsSideEdge;

	iterBegin = vecNaviCells.begin(); iterEnd = vecNaviCells.end();

	for( it_=iterBegin; it_!=iterEnd; ++it_ )
	{
		CNaviCell	&cellRight = (*it_);
		CTri		*ptriRight = cellRight.m_pTri;

		if(cellRight.m_iSeq==cellLeft.m_iSeq)
		{
			continue;
		}

#if _DEBUG
		// For TESTDEBUG
		if(NULL==ptriRight)
		{
			//assert(0 && "NULL==ptriRight in CProcessorNavi::setAdjacentATri().");
			assert__(ptriRight,__FILE__,__LINE__,__FUNCTION__,"NULL==ptriRight in CProcessorNavi::setAdjacentATri().");
		}
		// For TESTDEBUG
#endif // _DEBUG

		for( int i=0; i<3; ++i )
		{
			ariPointsShared_left[i] = ariPointsShared_right[i] = NULL_CELL;
		}

		aruiLVRight = ptriRight->uiIV;
		
		iFoundShared = 0;
		int iSharedIV_left=0, iSharedIV_right=0;

		for( int iL=0; iL<3; ++iL )
		{
			for( int iR=0; iR<3; ++iR )
			{
				if( aruiIVleft[iL]==aruiLVRight[iR] )
				{
					ariPointsShared_left[iL] = aruiIVleft[iL];
					ariPointsShared_right[iR] = aruiLVRight[iR];
					
					iSharedIV_left	= iSharedIV_left+iL;
					iSharedIV_right = iSharedIV_right+iR;

					++iFoundShared;
				} 
			} // for( int iR=0; iR<3; ++iR )
			
		} // for( int iL=0; iL<3; ++iL )

		bFoundShared = (iFoundShared>1);
		if( true==bFoundShared )
		{
#ifdef _DEBUG
			// For TESTDEBUG
			if(iFoundShared>2)
			{
				assert__((iFoundShared<3),__FILE__,__LINE__,__FUNCTION__,"iFoundShared>2 in CProcessorNavi::setAdjacentATri().");
				break;
			}

			int iCntNullCell = 0;
			for(int i=0; i<3; ++i)
			{
				if(NULL_CELL==ariPointsShared_left[i]) ++iCntNullCell;
			}

			if(iCntNullCell>1)
			{
				assert__(0,__FILE__,__LINE__,__FUNCTION__,"if(iCntNullCell>1)");
			}
			// For TESTDEBUG
#endif // _DEBUG

			cellLeft.SetAdjacentCell(getTypeEdge(iSharedIV_left), cellRight.m_iSeq);
			cellRight.SetAdjacentCell(getTypeEdge(iSharedIV_right), cellLeft.m_iSeq);
		} // if( iFoundShared>1 )
	} // for( it_=iterBegin; it_!=iterEnd; ++it_ )
}

E_CELL_SIDEEDGE CProcessorNavi::getTypeEdge(	const unsigned int uiSharedNum )
{
	E_CELL_SIDEEDGE	eSideEdge = E_EDGE_NULL;
	switch(uiSharedNum)
	{
	case 1:
		{
			eSideEdge=E_EDGE_01;
		}
		break;
	case 2:
		{
			eSideEdge=E_EDGE_20;
		}
		break;
	case 3:
		{
			eSideEdge=E_EDGE_12;
		}
		break;
	default:
		{
#ifdef _DEBUG
			assert__(false, __FILE__, __LINE__, __FUNCTION__, "SHARED IV NAVICELL.");
#endif // _DEBUG		
		}
	}

	return eSideEdge;
}


// Non Recursive
void CProcessorNavi::mappingNavigation__( VEC_NAVICELL &vecNaviCells, set<int> &slistCellsGoal )
{
	set<int>::iterator		iterC_, iterCBegin, iterCEnd, iterF_;
	bool bOverlappedWith=false, bMappingComplete=false, bComputeOK=false;
	int iSeqCellCurr_=0, iLevelFromGoal=0, iLevelFromGoalIterator=0;
	set<int> slistCellsAdj, slistCellsIterator, slistCellsAll;

	//copy( slistCellsGoal.begin(), slistCellsGoal.end(), slistCellsAdj );
	VEC_NAVICELL::iterator	iterN_, iterNBegin_, iterNEnd_;
	iterNBegin_=vecNaviCells.begin(); iterNEnd_=vecNaviCells.end();
	for(iterN_=iterNBegin_; iterN_!=iterNEnd_; ++iterN_)
	{
		slistCellsAll.insert((*iterN_).m_iSeq);
	}

	iterCBegin=slistCellsGoal.begin();		iterCEnd=slistCellsGoal.end();
	for(iterC_=iterCBegin;iterC_!=iterCEnd;++iterC_)
	{
		CNaviCell &naviCellCurr = vecNaviCells[(*iterC_)];
		naviCellCurr.setLinkcompute_( 0, 0.0f, 0 );
		 slistCellsIterator.insert(*iterC_);
	}


	// For TESTDEBUG
#ifdef _DEBUG_OUTPUTDEBUGSTRING
	unsigned int uiBuf_ = 0;
	char	szOutputDebugstr[512];
	memset(szOutputDebugstr,0,sizeof(char)*512);
	OutputDebugString( "//======================================================================" );
	OutputDebugString( " Mapping Navigation. " );
	OutputDebugString( "\n" );
	// For TESTDEBUG
#endif // _DEBUG_OUTPUTDEBUGSTRING


	do	// while( false==bMappingComplete )
	{
		m_iSeqLevelTotal = iLevelFromGoal = ++iLevelFromGoalIterator;

		iterCBegin=slistCellsIterator.begin();	iterCEnd=slistCellsIterator.end();

		for( iterC_=iterCBegin; iterC_!=iterCEnd; ++iterC_ )
		{
			iSeqCellCurr_ = (int)(*iterC_);
			CNaviCell	&naviCellCurr = vecNaviCells[iSeqCellCurr_];
#ifdef _DEBUG
			naviCellCurr.m_bAdjacentProcessed_fordebug = true;
#endif // _DEBUG
			E_CELL_SIDEEDGE		eEdgeAdj;

#ifdef _DEBUG_OUTPUTDEBUGSTRING
			sprintf(szOutputDebugstr, "%4d.LevelFromGoal(Seq:%4d)  \n", iLevelFromGoal, iSeqCellCurr_);
			OutputDebugString(szOutputDebugstr);
			memset( szOutputDebugstr,0,sizeof(char)*512 );
			uiBuf_ = 0;
#endif // _DEBUG_OUTPUTDEBUGSTRING

			for( int iEdge=0; iEdge<3; ++iEdge )
			{
				eEdgeAdj = (E_CELL_SIDEEDGE)iEdge;
				int iSeqCellAdj = naviCellCurr.m_arriAdjacentCells[iEdge];
				bool bThisisBlockCell = naviCellCurr.IsBlockCell();

				if(( NULL_CELL!=iSeqCellAdj 
					&& false==bThisisBlockCell
					&& eEdgeAdj!=naviCellCurr.m_eEdge_wayToGoal) )
				{
					CNaviCell &naviCellAdj=vecNaviCells[iSeqCellAdj];

					if( NULL_LEVELFROMGOAL==naviCellAdj.m_iLevelFromGoal ||
						naviCellAdj.m_iLevelFromGoal>naviCellCurr.m_iLevelFromGoal) 
					{
						iterF_=find(iterCBegin,iterCEnd,iSeqCellAdj);
						bOverlappedWith=(iterCEnd!=iterF_);
						if(false==bOverlappedWith)
						{
#ifdef _DEBUG_OUTPUTDEBUGSTRING
							// For TESTDEBUG
							uiBuf_ = uiBuf_+sprintf(uiBuf_+szOutputDebugstr, 
												"\t\t LevelCell(%4d) // Edge(%1d) // CellSEQ(%4d)", 
												naviCellAdj.m_iLevelFromGoal , iEdge, iSeqCellAdj);
							// For TESTDEBUG
#endif // _DEBUG_OUTPUTDEBUGSTRING

							bComputeOK=naviCellAdj.setLinkcompute_( iLevelFromGoal, 
								naviCellCurr.m_arrfcosttoGoal_edge[iEdge],
								iSeqCellCurr_ );

							if(true==bComputeOK)
							{
								slistCellsAdj.insert(iSeqCellAdj);

								slistCellsAll.erase(iSeqCellAdj);
									
#ifdef _DEBUG_OUTPUTDEBUGSTRING
								// For TESTDEBUG
								if(NULL_LEVELFROMGOAL==naviCellAdj.m_iLevelFromGoal)
								{
									assert__(0,__FILE__, __LINE__, __FUNCTION__, "");
								}

								uiBuf_ = uiBuf_+sprintf(uiBuf_+szOutputDebugstr, 
									" inserted(LevelCell(%4d).", 
									naviCellAdj.m_iLevelFromGoal );
								// For TESTDEBUG

#endif // _DEBUG_OUTPUTDEBUGSTRING
							}
							else
							{
#ifdef _DEBUG_OUTPUTDEBUGSTRING
								// For TESTDEBUG
								uiBuf_ = uiBuf_+sprintf(uiBuf_+szOutputDebugstr, 
									" not inserted.", 
									naviCellAdj.m_iLevelFromGoal );
								// For TESTDEBUG
#endif // _DEBUG_OUTPUTDEBUGSTRING
							}


						} // if(false==bOverlappedWith)

					}
				}
				

			} // for( int iEdge=0; iEdge<3; ++iEdge 

#ifdef _DEBUG_OUTPUTDEBUGSTRING
			// For TESTDEBUG
			OutputDebugString( szOutputDebugstr );
			OutputDebugString( "\n" );
			// For TESTDEBUG
#endif // _DEBUG_OUTPUTDEBUGSTRING

		} // for( iterC_=iterCBegin; iterC_!=iterCEnd ; ++iterC_ )


		if(slistCellsAdj.size() < 1)
		{
			bMappingComplete=true;
			--m_iSeqLevelTotal;
			break;
		}

		slistCellsIterator.clear();

		for(iterC_=slistCellsAdj.begin();iterC_!=slistCellsAdj.end();++iterC_)
		{
			slistCellsIterator.insert(*iterC_);
		}

		slistCellsAdj.clear();
	}while( false==bMappingComplete );
}


void CProcessorNavi::mappingNavigationRecurse(	VEC_NAVICELL &vecNaviCells_, 
											set<int> &slistCells_,
											const int iLevelFromGoal_ )
{
	if(slistCells_.size()<1)
	{
		return;
	}

	set<int>::iterator		iterC_, iterF_, iterCBegin, iterCEnd;
	bool bOverlappedWith=false;
	int iSeqCellCurr_ = 0, iLevelFromGoal_chld=iLevelFromGoal_+1;
	set<int> slistCellsAdj_;

	//	sort(slistCells_.begin(), slistCells_.end());
	iterCBegin = slistCells_.begin(); iterCEnd	= slistCells_.end();

	set<int>::iterator		iterCEnd_end = slistCellsAdj_.end();

	for( iterC_=iterCBegin; iterC_!=iterCEnd ; ++iterC_ )
	{
		iSeqCellCurr_ = (int)(*iterC_);
		CNaviCell	&naviCellCurr = vecNaviCells_[iSeqCellCurr_];

#ifdef _DEBUG
		naviCellCurr.m_bAdjacentProcessed_fordebug = true;
#endif // _DEBUG

		E_CELL_SIDEEDGE		eEdgeAdj;

		for( int iEdge=0; iEdge<3; ++iEdge )
		{
			eEdgeAdj = (E_CELL_SIDEEDGE)iEdge;
			int iSeqCellAdj = naviCellCurr.m_arriAdjacentCells[iEdge];

			if(NULL_CELL!=iSeqCellAdj && naviCellCurr.m_eEdge_wayToGoal!=eEdgeAdj)
			{
				bOverlappedWith=(iterCEnd==find(iterCBegin,iterCEnd,iSeqCellAdj));
				if(false==bOverlappedWith)
				{
					CNaviCell &naviCellAdj=vecNaviCells_[iSeqCellAdj];					
					naviCellAdj.setLinkcompute_( iLevelFromGoal_chld, 
						naviCellCurr.m_arrfcosttoGoal_edge[iEdge], 
						iSeqCellAdj );
					slistCellsAdj_.insert(iSeqCellAdj);
				}
			}
		} // for( int iEdge=0; iEdge<3; ++iEdge )
	} // for( iterC_=iterCBegin; iterC_!=iterCEnd ; ++iterC_ )

	mappingNavigationRecurse(	vecNaviCells_, 
								slistCellsAdj_, 
								iLevelFromGoal_chld );
}


bool CProcessorNavi::constructMappingNavigation( set<int> &slistCellsGoal, set<int> &slistCellsBlock )
{
	if(slistCellsGoal.size()<1)
	{
		return false;
	}

	setClearAllcell_unprocessed( m_vecNavicells );
	
	// @ set Goal Cells
	set<int>::iterator	iter_, iterBegin, iterEnd;
	iterBegin=slistCellsGoal.begin();
	iterEnd=slistCellsGoal.end();

	for( iter_=iterBegin; iter_!=iterEnd; ++iter_ )
	{
		CNaviCell &naviCellCurr = m_vecNavicells[(*iter_)];
		naviCellCurr.setLinkcompute_( 0, 0.0f, NULL_CELL );
	}

	// @ set Block Cells
	iterBegin=slistCellsBlock.begin();
	iterEnd=slistCellsBlock.end();

	for( iter_=iterBegin; iter_!=iterEnd; ++iter_ )
	{
		CNaviCell &naviCellCurr = m_vecNavicells[(*iter_)];
		naviCellCurr.SetBlockCell();
	}

	mappingNavigation__( m_vecNavicells, slistCellsGoal);

	m_bDidBuildGoal = true;
	
	return true;
}


bool CProcessorNavi::getCell(	const unsigned int iIdxNavicell, 
							CNaviCell &naviCell)
{
	if(iIdxNavicell>m_vecNavicells.size())
	{
		return false;
	}

	naviCell = m_vecNavicells[iIdxNavicell];
	
	return true;
}

void CProcessorNavi::ConstructTrisToCells( CTriContainer &triContainer )
{
	unsigned int uiCntTris = triContainer.getCountTries();
	CNaviCell naviCellNew;

	for( int iTri=0; iTri<(int)uiCntTris; ++iTri )
	{
		TRIPART::CTri &triCurr = triContainer.getTri(iTri);
		naviCellNew.InitCell( iTri, &triCurr );
		m_vecNavicells.push_back(naviCellNew);
	}

	buildupAdjacentAll( m_vecNavicells );
}

void CProcessorNavi::setClearAllcell_unprocessed(	VEC_NAVICELL &vecNaviCells )
{
	VEC_NAVICELL::iterator	it_, iterBegin, iterEnd;
	iterBegin = vecNaviCells.begin(); iterEnd = vecNaviCells.end();
	for( it_=iterBegin; it_!=iterEnd; ++it_ )
	{
		(*it_).setClear_unprocessed();
	}
}

#if defined(DEBUG_CEHCK_VALIDATION_NAVIGATION) && defined(APPTARGET_IS_WIN)
// For TESTDEBUG
void CProcessorNavi::validateAllAdjcntCells(	VEC_NAVICELL &vecNavicells )
{
	char			szOutputDebug[512];
	unsigned int	uiBuf = 0;
	unsigned int	uiCnt = 0;
	VEC_NAVICELL::iterator	it_, iterBegin, iterEnd;

	OutputDebugString( "\n" );
	OutputDebugString( "Check Adjacent Tris" );
	OutputDebugString( "\n" );
	OutputDebugString( "============================================================================" );
	iterBegin = vecNavicells.begin(); iterEnd = vecNavicells.end();
	for( it_=iterBegin; it_!=iterEnd; ++it_, ++uiCnt )
	{
		uiBuf = 0;
		CNaviCell	&cellRight = (*it_);
		uiBuf=uiBuf+sprintf(uiBuf+szOutputDebug,"\n %4d. TriSeq(%4d)  Adjacent Tri : ", uiCnt, cellRight.m_iSeq);

		for(int i=0; i<(int)E_EDGE_CNT; ++i)
		{
			int iAdjcntTri = cellRight.m_arriAdjacentCells[i];

			uiBuf=uiBuf+sprintf(uiBuf+szOutputDebug," %4d ", iAdjcntTri);
		}

		OutputDebugString( szOutputDebug );
	}

	validateAdjcntRecurse( 0, vecNavicells );

	OutputDebugString( "\n" );
	OutputDebugString( "\n" );
	OutputDebugString( "Check Call by Neibor Tri" );
	OutputDebugString( "\n" );
	OutputDebugString( "============================================================================" );
	iterBegin = vecNavicells.begin(); iterEnd = vecNavicells.end();
	for( it_=iterBegin; it_!=iterEnd; ++it_, ++uiCnt )
	{
		uiBuf = 0;
		CNaviCell	&cellRight = (*it_);
		uiBuf=uiBuf+sprintf(uiBuf+szOutputDebug,"\n %4d. TriSeq(%4d) AnyCallByNeiborTri?(%s) Cnt CallByNeiborTri(%d) ", 
					uiCnt, cellRight.m_iSeq,
					(true==cellRight.m_bAdjacentProcessed_fordebug)?"TRUE":"FALSE",
					cellRight.m_iAdjacented_debug );

// 		if(	true!=cellRight.m_bAdjacentProcessed_fordebug || 
// 			3<cellRight.m_iAdjacented_debug )
// 		{
// 			assert__(0,__FILE__, __LINE__, __FUNCTION__, "");
// 		}

		OutputDebugString( szOutputDebug );
	}	
}

void CProcessorNavi::validateAdjcntRecurse(	int iSeqCell, VEC_NAVICELL &vecNavicells )
{
	CNaviCell	&naviCellCurr = vecNavicells[iSeqCell];

#ifdef _DEBUG
	++(naviCellCurr.m_iAdjacented_debug);
#endif // _DEBUG

 	if(true==naviCellCurr.m_bAdjacentProcessed_fordebug)
 	{
 		return;
 	}

	naviCellCurr.m_bAdjacentProcessed_fordebug = true;

	for( int i=0; i<3; ++i )
	{
		int iSeqCell = naviCellCurr.m_arriAdjacentCells[i];

		if(NULL_CELL!=iSeqCell)
		{
			validateAdjcntRecurse( iSeqCell, vecNavicells );
		}
	}
}
// For TESTDEBUG
#endif // defined(DEBUG_CEHCK_VALIDATION_NAVIGATION) && defined(APPTARGET_IS_WIN)

CProcessorNavi::CProcessorNavi()
{
	m_iSeqNavis = NULL_NAVICONTAINER;
	m_iSeqLevelTotal = -1;

	m_bDidBuildGoal = false;
}
CProcessorNavi::~CProcessorNavi()
{

}

#endif // _DEBUG_NAVIGATION