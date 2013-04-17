#include "DrawForDebug.h"

#ifdef _DEBUGFOR_RENDER

void CDrawForDebug::renderLine(const Vector3 &v3Startline, 
							   const Vector3 &v3Endline,
							   const Vector3 &v3colorline,
							   const Vectormath::Aos::Matrix4 *matViewPrjCamera )
{
	unsigned int indices[2] = {0, 1};

	Vector4 points[2], color;

	vectorCopy3to4( points[0], v3Startline, 1.0f);
	vectorCopy3to4( points[1], v3Endline,	1.0f);
	vectorCopy3to4( color, v3colorline, 1.0f );

	m_prenderInterface->debugDraw(PSSG::PRenderInterface::PE_PRIMITIVE_LINES, 
								points, color, indices, 2, matViewPrjCamera );
}

void CDrawForDebug::renderCube(	const Vector3 *arrv3TopPoint4,
								const Vector3 *arrv3BottomPoint4,
								const Vector3 &v3color,
								const Vectormath::Aos::Matrix4 *matViewPrjCamera)
{
	Vector4 v4Cubepoints[8], color;

	for(unsigned int i=0; i<4; ++i)
	{
		vectorCopy3to4(v4Cubepoints[i], arrv3BottomPoint4[i],1.0f);
	}

	for(unsigned int i=0; i<4; ++i)
	{
		vectorCopy3to4(v4Cubepoints[i+4], arrv3TopPoint4[i],1.0f);
	}

	vectorCopy3to4( color, v3color, 1.0f );

	unsigned int indices[24] = {	0,1, 1,2, 2,3, 3,0,
									0,4, 1,5, 3,7, 2,6,
									4,5, 5,6, 6,7, 7,4 };

	m_prenderInterface->debugDraw(PSSG::PRenderInterface::PE_PRIMITIVE_LINES, 
								v4Cubepoints, color, indices, 24, matViewPrjCamera);
}


CDrawForDebug::CDrawForDebug(void)
{
}

CDrawForDebug::~CDrawForDebug(void)
{
}

#endif // _DEBUGFOR_RENDER