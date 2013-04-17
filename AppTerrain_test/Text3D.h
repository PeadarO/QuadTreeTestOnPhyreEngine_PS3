#pragma once

#include "CommonApp.h"

#ifdef DEBUG_TEXT3D_

#include <PSSG.h>
#include <Framework/PSSGFramework.h>
#include <Extra/PSSGExtra.h>
#include <PSSGText/PSSGText.h>

#include <vector>

using namespace PSSG;

namespace TEXT3D
{
	struct S_TEXT3DUNIT
	{
		PFont *_pfont;

	};

	typedef vector<S_TEXT3DUNIT*>	VEC_FONT;

	class CText3D
	{
		PText		*m_textInstance;
		VEC_FONT	m_vecFont;

		void InitText3D();
		void setNewText3D();

		void ReleaseText3D();
	};
};

#endif // DEBUG_TEXT3D_
