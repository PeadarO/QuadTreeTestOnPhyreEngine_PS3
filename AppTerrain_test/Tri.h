#pragma once

#include <PSSG.h>

namespace TRIPART
{
#define CNTPNTTRI		3

	class CTri
	{
	public:
		Vector3			v3FaceNormal;
		Vector3			v3PT[CNTPNTTRI];
		unsigned int	uiIV[CNTPNTTRI];

	public:
		// @ Set / Get
		void Set(	const CTri &triSrc);
		void Set(	const Vector3 *v3Ver_, const unsigned int *uiIV_);
		void Set(	const Vector3 &v01_,const Vector3 &v02_,const Vector3 &v03_,
					const unsigned int uiIV01, const unsigned int uiIV02, const unsigned int uiIV03 );

		bool isValidateASPoly()
		{
			for(int iPnt=0; iPnt<CNTPNTTRI; ++iPnt)
			{
				unsigned int uiCurr=uiIV[iPnt];
				for(int iPnt__=0; iPnt__<CNTPNTTRI; ++iPnt__)
				{
					if(iPnt==iPnt__)
					{
						continue;
					}
					unsigned int uiCurr__=uiIV[iPnt__];

					if(uiCurr==uiCurr__)
					{
						return false;
					}
				}
			} // for(int iPnt=0; iPnt<CNTPNTTRI; ++iPnt)

			return true;
		}

		bool isEqual(	const CTri &rValue )
		{
			bool bEqual=true;
			for( int iPnt=0; iPnt<CNTPNTTRI; ++iPnt )
			{
				if(uiIV[iPnt]!=rValue.uiIV[iPnt])
				{
					bEqual=false;
					break;
				}
			}

			if(true==bEqual)
			{
				return true;
			}

			bEqual=true;
			for( int iPnt=0; iPnt<CNTPNTTRI; ++iPnt )
			{
				int iPnt_=(iPnt+1)%3;
				if(uiIV[iPnt]!=rValue.uiIV[iPnt_])
				{
					bEqual=false;
					break;
				}
			}

			if(true==bEqual)
			{
				return true;
			}

			bEqual=true;
			for( int iPnt=0; iPnt<CNTPNTTRI; ++iPnt )
			{
				int iPnt_=(iPnt+2)%3;
				if(uiIV[iPnt]!=rValue.uiIV[iPnt_])
				{
					bEqual=false;
					break;
				}
			}

			return bEqual;
		};

		void GetCenterTri(	Vector3 &v3Center );

	public:
		CTri();
		~CTri();
	};
}; // TRIPART

