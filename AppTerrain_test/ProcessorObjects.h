#pragma once

#include "CommonApp.h"

#ifdef _DEBUG_OBJECTS

#include "CommonSingleton.h"

#include "BoundaryVolume.h"

#include "Object.h"
#include "SpaceTree.h"

#include "NaviCells.h"

#include "CustomTime.h"

namespace OBJ
{
#define ADJUST_OBJRAY_UP_POS	5.f
#define MS_INTERVAL_COLLI_OBJ	0.2f

	typedef		vector<CObject*>	VEC_OBJECT;
	class CProcessorObjects : public CSingleton<CProcessorObjects>
	{
	public:
#ifdef _DEBUG_SPACETREE
		CSpaceTree						*m_pSpcTree;
#endif // _DEBUG_SPACETREE

#ifdef _DEBUG_NAVIGATION
		NAVIPART::CNaviCells			*m_pNavi;
#endif // _DEBUG_NAVIGATION

		CCustomTimer					m_timerIntrvl_OBJ;

	public:
		bool setLandingDirectly(	CObjectDynamic &objDyn, bool bDirAdjust_forNAVI = false );

	public:
#if defined(_DEBUG_SPACETREE) && defined(_DEBUG_NAVIGATION)
		void Initialize(	CSpaceTree *pSpaceTree, NAVIPART::CNaviCells *pNavi )
		{
			m_pSpcTree = pSpaceTree;
			m_pNavi=pNavi;
		};
#else
	#ifdef _DEBUG_SPACETREE
		void Initialize(	CSpaceTree *pSpaceTree )
		{
			m_pSpcTree=pSpaceTree;
		};
	#endif // _DEBUG_SPACETREE

	#ifdef _DEBUG_NAVIGATION
		void Initialize(	NAVIPART::CNaviCells *pNavi )
		{
			m_pNavi=pNavi;
		};
	#endif // _DEBUG_NAVIGATION
#endif // defined(_DEBUG_SPACETREE) && defined()_DEBUG_NAVIGATION)

		void updateObjDyn_update(	const double &dblElapsedTime,
									CObjectDynamic *pObj );	

		void updateAllObjDyn_useSpcTree(	const double &dblElapsedTime,
											VEC_OBJECT &vecObjects );
		void updateObjDyn_useSpcTree(	const double &dblElapsedTime,
										CObjectDynamic *pObj );	
#ifdef _DEBUG_NAVIGATION
		void updateAllObjDyn_useNAVI(	const double &dblElapsedTime,
										VEC_OBJECT &vecObjects );
		void updateObjDyn_useNAVI(	const double &dblElapsedTime,
									CObjectDynamic *pObj );	
#endif // _DEBUG_NAVIGATION

		void updateObjDyn_test(	const double &dblElapsedTime,
								CObjectDynamic *pObj );	


		void updateAllObjStatic(	const double &dblElapsedTime,
									VEC_OBJECT &vecObjects );

		void updateObjStatic(	const double &dblElapsedTime,
								CObjectDynamic *pObj );	

		void Release();

	public:
		CProcessorObjects(void);
		~CProcessorObjects(void);
	};
};

#endif // _DEBUG_OBJECTS
