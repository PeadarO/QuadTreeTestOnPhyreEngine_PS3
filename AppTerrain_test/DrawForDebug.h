#pragma once


#include "commonApp.h"

#ifdef _DEBUGFOR_RENDER
#include <PSSG.h>

#include "CommonMath.h"
#include "CommonSingleton.h"




class CDrawForDebug : public CSingleton<CDrawForDebug>
{
private:
	PSSG::PRenderInterface		*m_prenderInterface;

public:
	void Intialize(PSSG::PRenderInterface *prenderInterface)
	{
		m_prenderInterface = prenderInterface;
	}


	void renderLine(	const Vector3 &v3Startline, 
						const Vector3 &v3Endline,
						const Vector3 &v3colorline,
						const Vectormath::Aos::Matrix4 *matViewPrjCamera=NULL);

	void renderCube(	const Vector3 *arrv3TopPoint4,
						const Vector3 *arrv3BottomPoint4,
						const Vector3 &v3color,
						const Vectormath::Aos::Matrix4 *matViewPrjCamera=NULL);


public:
	CDrawForDebug(void);
	~CDrawForDebug(void);
};
#endif // _DEBUGFOR_RENDER