#include "ManagerNavi.h"

#ifdef _DEBUG_NAVIGATION
using namespace NAVIPART;


void CManagerNavi::BuildupNewMappingNavi( set<int> &slistCellsGoal, set<int> &slistCellsBlock )
{
	CNaviCells	&naviCells = m_NaviCells_basement;
	naviCells.constructMappingNavigation( slistCellsGoal, slistCellsBlock );
}

void CManagerNavi::BuildNewNavi(	set<int> &slistCellsGoal, set<int> &slistCellsBlock )
{
	BuildupNewMappingNavi(slistCellsGoal, slistCellsBlock);
}

void CManagerNavi::Initialize(	CTriContainer	*ptriContainer,
								CSpaceTree		*pSpaceTree )
{
	m_ptriContainer = ptriContainer;
	m_pSpaceTree = pSpaceTree;

	m_NaviCells_basement.Initialize(*m_ptriContainer);
}

void CManagerNavi::Release()
{
	if(m_vecNaviCells.size())
	{
		for(int i=0; i<(int)m_vecNaviCells.size(); ++i)
		{
			m_vecNaviCells[i].Release();
		}

		m_vecNaviCells.clear();
	}
}

CManagerNavi::CManagerNavi()
{
	m_ptriContainer = NULL;
	m_pSpaceTree = NULL;

	m_uiCntGoal = 0;
}

CManagerNavi::~CManagerNavi()
{

}

#endif // _DEBUG_NAVIGATION
