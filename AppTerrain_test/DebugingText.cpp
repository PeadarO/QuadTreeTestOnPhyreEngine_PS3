#include "DebugingText.h"

CDebugingText::CDebugingText(void)
{
	m_debugTexture = NULL;
}

CDebugingText::~CDebugingText(void)
{
}


void CDebugingText::Init()
{
	if(!m_debugTexture)
	{
		m_debugTexture = PSSG::Extra::debugTextureCreate(64, 128);	// 64列、128行
		g_debugPrintfCallback.m_texture = m_debugTexture;			// PSSG_PRINTFオーバーライドで登録する
		PSSG::PSSGSetPrintfCallback(g_debugPrintfCallback);			// PSSG_PRINTFオーバーライドを登録する

		char outs[256];
		PSSGOS::getUserMachineString(outs,255);
		int d = PUtilityDebug::getAppPort();
		PSSG_PRINTF("This is PhyreEngine version %s\n", PSSGVersion());
#ifdef ENABLE_REMOTE_CONNECTIONS
		PSSG_PRINTF("Viewer running on %s : %d (Remote connections enabled)\n\n",outs,d);
#else // ENABLE_REMOTE_CONNECTIONS
		PSSG_PRINTF("Viewer running on %s : %d (Remote connections disabled)\n\n",outs,d);
#endif // ENABLE_REMOTE_CONNECTIONS
	}


	// ??
	if(m_debugTexture)
		m_debugTexture->getDBase().setPersistence(true);
}

void CDebugingText::printText( PChar *szOutput )
{
	PSSG_PRINTF(szOutput);
};
