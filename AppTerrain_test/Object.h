#pragma once


#include "CommonApp.h"

#ifdef _DEBUG_OBJECTS

#include <PSSG.h>
#include <Extra/PSSGExtra.h>
#include <vector>

#include "CommonMath.h"

using namespace PSSG;
using namespace Vectormath::Aos;

namespace OBJ
{
#define NULL_TRI		-1

#define DEFAULT_SPEEDMOVEMENT_PERSEC	10.0f
#define DEFAULT_SPEEDANIMATION			1.0f

	enum E_TYPE_OBJECT
	{
		OBJTYPE_NULL = -1,
		OBJTYPE_STATIC = 0,
		OBJTYPE_DYNAMIC,
		OBJTYPE_END,
	};

	enum E_TYPE_OBJDYNAMIC 
	{	TYPEOBJ_DYN_NULL = 0, 
		TYPEOBJ_DYN_A,
		TYPEOBJ_DYN_B,
		TYPEOBJ_DYN_C,
		TYPEOBJ_DYN_D,
		TYPEOBJ_DYN_END,
	};

	struct S_OBJECT_PROPERTY
	{
		E_TYPE_OBJECT				_TypeObj;

		PNode						*_pnodeMesh;
		float						_fSpeedAnimation;
		float						_fSpeedMove;
		Vectormath::Aos::Vector3	_v3Direction;

	public:
		S_OBJECT_PROPERTY()
		{
			_TypeObj = OBJTYPE_NULL;

			_pnodeMesh = NULL;
			_fSpeedAnimation = DEFAULT_SPEEDANIMATION;
			_fSpeedMove = 0.f;
			memset(&_v3Direction, 0, sizeof(Vector3));			
		}
	};

	class CObject
	{
	protected:
		S_OBJECT_PROPERTY		m_PropertyObj;

	public:
		void setProperty(
						const E_TYPE_OBJECT	eTypeObj_in,
						const PNode	*pnodeMesh_in,
						Vector3	&v3Position_in,
						const Vector3 &v3Dir_in,
						const float	fSpeedAnimation_in	= DEFAULT_SPEEDANIMATION, 
						const float	fSpeedMove_in		= DEFAULT_SPEEDMOVEMENT_PERSEC )
		{
			m_PropertyObj._TypeObj = eTypeObj_in;
			m_PropertyObj._pnodeMesh = const_cast<PNode*>(pnodeMesh_in);
			memcpy( &(m_PropertyObj._v3Direction), &(v3Dir_in), sizeof(Vector3) );
			m_PropertyObj._fSpeedAnimation = fSpeedAnimation_in;
			m_PropertyObj._fSpeedMove = fSpeedMove_in;

			m_PropertyObj._pnodeMesh->m_matrix.setCol3((Vector4)v3Position_in);
		}


	public:
		// @ Get / Set
		PNode *getNodeObj()
		{
			return m_PropertyObj._pnodeMesh;
		}

		void getPosObj(Vector3 &v3PosCurrent)
		{
			PNode	*pNode	= m_PropertyObj._pnodeMesh;
			v3PosCurrent	= pNode->m_matrix.getCol3().getXYZ();
		}

		void setPosObj(Vector3 &v3PosCurrent)
		{
			PNode	*pNode	= m_PropertyObj._pnodeMesh;
			pNode->m_matrix.setCol3((Vector4)v3PosCurrent);
		}

		const Vector3 &getDirection()
		{
			return m_PropertyObj._v3Direction;
		}

		void setDirection(Vector3 &v3Dir)
		{
			memcpy(&(m_PropertyObj._v3Direction), &v3Dir, sizeof(Vector3));
		}

		E_TYPE_OBJECT getTypeObj()
		{
			return m_PropertyObj._TypeObj;
		}

		float getSpeedMovePerSec()
		{
			return m_PropertyObj._fSpeedMove;
		}


		// @ Process
		virtual void UpdateObject(const double &dblElapsedTime) = 0;



	public:
		CObject(){};
		virtual ~CObject(){};
	}; // class CObject

	class CObjectStatic : public CObject
	{
	public: 
		Vector3					m_v3OffsetPos;

	public:
		// @ Process
		void UpdateObject(const double &dblElapsedTime);

	public:
		CObjectStatic() 
		{
			m_v3OffsetPos.setX(0.0f);
			m_v3OffsetPos.setY(0.0f);
			m_v3OffsetPos.setZ(0.0f);
		};
		~CObjectStatic() 
		{

		};
	}; // class CObjectStatic : public CObject

	class CObjectDynamic : public CObject
	{
	private: 
		// @ Basement
		E_TYPE_OBJDYNAMIC		m_eTypeDynamicObj;
		Vector3					m_v3OffsetPos;
		Vector3					m_v3SizeObj;

		// @ Navigation Mesh
		int						m_iTriLanding;

		// @ Status
		bool					m_bReachtheGoal;
		bool					m_bLanding;
		Vector3					m_v3DirFalling;
		Vector3					m_v3DirBasement;

	public:
		// @ Get / Set
		void setPropertyDyn(
					const E_TYPE_OBJECT	eTypeObj_in,
					const E_TYPE_OBJDYNAMIC eTypeObjDyn_in,
					const PNode *pnodeMesh_in,
					Vector3	&v3Position_in,
					const Vector3	&v3Dir_in,
					const Vector3	&v3SizeObj_in,
					const float	fSpeedAnimation_in = DEFAULT_SPEEDANIMATION, 
					const float	fSpeedMove_in = DEFAULT_SPEEDMOVEMENT_PERSEC )
		{
			CObject::setProperty(	eTypeObj_in,
									pnodeMesh_in,
									v3Position_in,
									v3Dir_in,
									fSpeedAnimation_in,
									fSpeedMove_in );
			setTypeObjDyn(eTypeObjDyn_in);
			memcpy(&m_v3SizeObj,&v3SizeObj_in,sizeof(Vector3));
		}

		bool getReachtheGoal()
		{
			return m_bReachtheGoal;
		}

		void setReachtheGoal(	const bool bReachtheGoal )
		{
			m_bReachtheGoal = bReachtheGoal;
		}

		void setDirBasement(	const Vector3 &v3DirBasement_in )
		{
			memcpy( &m_v3DirBasement, &v3DirBasement_in, sizeof(Vector3) );
		}

		const Vector3 &getDirBasement()
		{
			return m_v3DirBasement;
		}

		void setTypeObjDyn( E_TYPE_OBJDYNAMIC eTypeDynamicObj_ ) {
			m_eTypeDynamicObj = eTypeDynamicObj_;
		};

		int getTypeObjDyn() {
			return m_eTypeDynamicObj;
		};

		void setLandingTri(	int iTriLanding ) {	m_iTriLanding = iTriLanding; };
		int getLandingTri() {	return m_iTriLanding; };

		void setLanding(bool bLanding) {	m_bLanding = bLanding; };
		bool getLanding() {	return m_bLanding; };

	public:
		// @ Process
		void UpdateObject(	const double &dblElapsedTime );
		void UpdateObjectFalling(	const double &dblElapsedTime );


	public:
		CObjectDynamic() 
		{
			m_v3OffsetPos.setX(0.0f);
			m_v3OffsetPos.setY(0.0f);
			m_v3OffsetPos.setZ(0.0f);

			m_iTriLanding = NULL_TRI;
			m_bLanding = true;

			m_v3DirFalling.setX(0.0f);
			m_v3DirFalling.setY(-1.0f);
			m_v3DirFalling.setZ(0.0f);

			m_bReachtheGoal = false;
		};
		~CObjectDynamic() 
		{

		};
	}; // class CObjectDynamic : public CObject

} // namespace OBJ

#endif // _DEBUG_OBJECTS
