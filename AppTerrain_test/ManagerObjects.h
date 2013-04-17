#pragma once

#include "CommonApp.h"

#ifdef _DEBUG_OBJECTS

#include "Object.h"
#include "TerrainMesh.h"

#include "ProcessorObjects.h"
#include "NaviCells.h"

namespace OBJ
{
#define DEFAULT_CNTOBJDYN			16
#define DEFAULT_SEC_FREQUENCY_CHANGE		8

#define DEFAULT_DIST_ARRIVED		10.f

	enum	E_STATE_OBJECT
	{
		OBJ_STATE_NULL=-1,
		OBJ_STATE_IDLE,
#ifdef _DEBUG_TERRAINSHAPE_TEST
#else // _DEBUG_TERRAINSHAPE_TEST
		OBJ_STATE_WALK_SPCTREE,
#endif // _DEBUG_TERRAINSHAPE_TEST
		OBJ_STATE_WALK_NAVI,
		OBJ_STATE_END,
	};

	struct S_TARGET_POS_FOR_WALK_RAND
	{
		Vector3		_v3PosTarget;
		int			_iTriSeq;
	};

typedef vector<CObject*>	VEC_OBJECT;

	struct S_OBJ_RESOURCE
	{
		PSSG::PDatabaseID		_DatabaseID;
		PSSG::PNode				*_pBNode_RenderRsc;
		Vector3					_v3SizeObj;

		void setObjRsc( PSSG::PDatabaseID DatabaseID_,
						PSSG::PNode *pBNode_RenderRsc_,
						Vector3 &v3SizeObj_ )
					{
						_DatabaseID=DatabaseID_;
						_pBNode_RenderRsc=pBNode_RenderRsc_;
						memcpy( &_v3SizeObj,&v3SizeObj_,sizeof(Vector3) );
					};
	};

	class CObjects
	{
	public:
		// @ Container
		S_OBJ_RESOURCE			m_arObjResource[TYPEOBJ_DYN_END];
		VEC_OBJECT				m_arrObjRenderInstance[OBJTYPE_END];
		vector<S_TARGET_POS_FOR_WALK_RAND>	m_vecTrisTargetMove_forSpcTree;
		
		// @ Process
		PSSG::PRootNode			*m_pnodeRoot;
		CObjectsProcessor		*m_pProcessorObj;
		E_STATE_OBJECT			m_eStateObj;
		bool					m_bLandingProcessForObjDynamic;
		CSpaceTree				*m_pTerrainSpaceTree;
		PDatabase				*m_pDatabaseScene;

	private:
		// @ Get / Set
		int	getRand_noOverlap(const int iNumLmit_in, const int iNum_noOverlap_in)
		{
			int iRandNum=0;
			do 
			{
				iRandNum=rand()%iNumLmit_in;
			} while(iRandNum==iNum_noOverlap_in);

			return iRandNum;
		}

		void setTargettoOBJ_forWalkRand(	TRIPART::CTri &triCurr_in, 
								const int iTriSeqTarget_in,
								S_TARGET_POS_FOR_WALK_RAND &targetArea_out, 
								CObjectDynamic &objDynamic_out )
		{
			Vector3		v3PosTriCntr, v3PosObj, v3DirBasement;
			triCurr_in.GetCenterTri(v3PosTriCntr);

			objDynamic_out.getPosObj(v3PosObj);
			v3PosObj.setY(v3PosTriCntr.getY());
			v3DirBasement = normalize(v3PosTriCntr-v3PosObj);


			targetArea_out._iTriSeq = iTriSeqTarget_in;
			targetArea_out._v3PosTarget=v3PosTriCntr;

			objDynamic_out.setDirBasement( v3DirBasement );
		}

	public:
		// @ Get / Set
		unsigned int getCntofObjsTotal()
		{
			unsigned int uiSumObjs=0;

			for(int iTypeObj=0; iTypeObj<OBJTYPE_END; ++iTypeObj)
			{
				uiSumObjs=+m_arrObjRenderInstance[iTypeObj].size();
			}

			return uiSumObjs;
		}

		// @ Function
#ifdef _DEBUG_NAVIGATION
		bool SetStatus_ObjDyn_switching();
		void SetStatus_ObjDyn(	E_STATE_OBJECT eStateObj	);

		E_STATE_OBJECT GetStatus_ObjDyn(){	return m_eStateObj;};
#endif // _DEBUG_NAVIGATION
		void deleteObj(	OBJ::CObject *&pObj );

	public:
		// @ Process
		void resetObjDynAll_SPC_randomPos_Test(	VEC_OBJECT &vecObjDynamic,
												CObjectsProcessor *pProcessorObj,
												vector<S_TARGET_POS_FOR_WALK_RAND> &vecTrisTargetMove_forSpcTree,
												CTriContainer *triCollected );

		void resetObjDyn_SPC_randomPos_Test(	CObjectDynamic &objDyn,
											CObjectsProcessor *pProcessorObj,
											S_TARGET_POS_FOR_WALK_RAND &targetArea,
											CTriContainer *triCollected );

		void resetObjDynAll_NAVI(	VEC_OBJECT &vecObjDynamic,
									CObjectsProcessor *pProcessorObj );

		void InsertNewObjDyn(	const E_TYPE_OBJDYNAMIC eTypeObj_in,
								Vector3 &v3Position_in,
								const Vector3 &v3Dir_in,
								const float fSpeedAnimation_in,
								const float fSpeedPerSec_in );

		void deleteObjDyn(	unsigned int uiSeqObj );
		void deleteObjDynAll();
		void deleteObjDynTypeAll(	const E_TYPE_OBJDYNAMIC eTypeObj_in );

		void batchNewObjsDynType_test(	const E_TYPE_OBJDYNAMIC eTypeObj_in,
										const int iCntObj,
										const Vector3 &v3Position_in,
										const Vector3 &v3Dir_in,
										const float fSpeedAnimation_in=DEFAULT_SPEEDANIMATION,
										const float fSpeedPerSec_in=DEFAULT_SPEEDMOVEMENT_PERSEC );


		// @ Process
		bool InitializeObjDynamics(	PDatabase *databaseScene,
									PSSG::PRootNode *pnodeRoot, 
									CSpaceTree *pTerrainSpaceTree,
		#ifdef _DEBUG_NAVIGATION
									NAVIPART::CNaviCells *pNavi,
		#endif // _DEBUG_NAVIGATION
									const Vector3 &v3TransObj,
									PString szFileName_path,
									E_STATE_OBJECT eStateObj=OBJ_STATE_IDLE,
									unsigned int uiMutantCnt=DEFAULT_CNTOBJDYN,
									float fSpeedObjAnimation=DEFAULT_SPEEDANIMATION,
									float fObjectMovementSpeedperSec=DEFAULT_SPEEDMOVEMENT_PERSEC );

		void UpdateObj(	double &dElapsedTime );
		void UpdateTest(	double &dElapsedTime );

		void UpdateObjsDynamic(	VEC_OBJECT &vecObjects, const double &dElapsedTime );
		void UpdateObjsStatic(	VEC_OBJECT &vecObjects, const double &dElapsedTime );

		// 	void Render(		PSSG::PRenderInterface *renderInterface, 
		// 		int width, 
		// 		int height, 
		// 		PSSG::PCameraNode *nodeCamera );

		void Release();

	public:
		CObjects()
		{
			m_eStateObj = OBJ_STATE_IDLE;
			m_pTerrainSpaceTree = NULL;
			m_pDatabaseScene=NULL;
			m_pnodeRoot=NULL;
		};
		~CObjects()
		{
			m_eStateObj = OBJ_STATE_IDLE;
			m_pTerrainSpaceTree = NULL;
			m_pDatabaseScene=NULL;
			m_pnodeRoot=NULL;
		};
	};

};

#endif // _DEBUG_OBJECTS
