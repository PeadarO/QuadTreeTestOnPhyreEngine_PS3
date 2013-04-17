#include "Tri.h"

using namespace TRIPART;

CTri::CTri()
{
	memset( &v3PT, 0, sizeof(Vector3)*3 );
	memset( &v3FaceNormal, 0, sizeof(Vector3) );
}

CTri::~CTri()
{
}

void CTri::Set(const CTri &triSrc)
{
	memcpy( &(this->v3FaceNormal), &(triSrc.v3FaceNormal), sizeof(Vector3) );
	memcpy( (this->v3PT), (triSrc.v3PT), sizeof(Vector3)*3 );
	memcpy( (this->uiIV), (triSrc.uiIV), sizeof(unsigned int)*3 );
}

void CTri::Set(const Vector3 *v3Ver_, const unsigned int *uiIV_)
{
	memcpy( v3PT, v3Ver_, sizeof(Vector3)*3 );
	memcpy( uiIV, uiIV_, sizeof(unsigned int)*3 );
	v3FaceNormal = cross((v3PT[1]-v3PT[0]), (v3PT[2]-v3PT[0]));
	v3FaceNormal = normalize( v3FaceNormal );
}

void CTri::Set(	const Vector3 &v01_,const Vector3 &v02_,const Vector3 &v03_,
				const unsigned int uiIV01, const unsigned int uiIV02, const unsigned int uiIV03 )
{
	memcpy( &v3PT[0], &v01_, sizeof(Vector3) );
	memcpy( &v3PT[1], &v02_, sizeof(Vector3) );
	memcpy( &v3PT[2], &v03_, sizeof(Vector3) );

	v3FaceNormal = cross((v3PT[1]-v3PT[0]), (v3PT[2]-v3PT[0]));
	uiIV[0]=uiIV01;		uiIV[1]=uiIV02;		uiIV[2]=uiIV03;
}

void CTri::GetCenterTri(	Vector3 &v3Center )
{
	v3Center = (v3PT[0]+v3PT[1]+v3PT[2])/3;
}
