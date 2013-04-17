#include "Object.h"

#ifdef _DEBUG_OBJECTS

using namespace OBJ;

void CObjectDynamic::UpdateObject(const double &dblElapsedTime)
{
	Vector3			v3PosCurrent, v3PosNextCal;

	getPosObj(v3PosCurrent);


	v3PosNextCal	= v3PosCurrent + 
			(getDirection()*(getSpeedMovePerSec()*((float)dblElapsedTime/1.0f)));

	setPosObj(v3PosNextCal);
}

void CObjectDynamic::UpdateObjectFalling(	const double &dblElapsedTime )
{
	Vector3			v3PosCurrent, v3PosNextCal;

	getPosObj(v3PosCurrent);

	const float	fG = ACCELERATOR_G*10.0f;

	v3PosNextCal = v3PosCurrent + 
					( m_v3DirFalling*(fG*((float)dblElapsedTime/1.0f)) );

	setPosObj(v3PosNextCal);
}

#endif // _DEBUG_OBJECTS
