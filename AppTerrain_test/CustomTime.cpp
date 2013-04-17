#include "CustomTime.h"

#include <PSSG.h>

void CCustomTime::InitTime()
{
	FWTime::update();
	m_dblInitTime = (double)FWTime::getCurrentTime();
}

void CCustomTime::MarkThisTime( void )
{
	FWTime::update();
	m_dblLastTime = (double)FWTime::getCurrentTime();
}


double CCustomTime::GetElapsedTime( void )
{
	FWTime::update();
	m_dblCurrentTime = (double)FWTime::getCurrentTime();

 	double dblElapsed = (m_dblCurrentTime - m_dblLastTime);
	return (dblElapsed);
}

CCustomTime::CCustomTime(void)
{	
	m_dblInitTime		= 0;

	m_dblLastTime		= 0;
	m_dblCurrentTime	= 0;
}

CCustomTime::~CCustomTime(void)
{
}



bool CCustomTimer::getIntervalTick(double dblElapsedTime)
{
	// 1. Frame Per Second
	m_dElapsedTimeSummary += dblElapsedTime;
	m_ulTickCnt++;

	if( m_dElapsedTimeSummary >= m_dblSecInterval )
	{
		m_ulTickCnt = 0;
		m_dElapsedTimeSummary = 0.0f;	

		return true;
	}
	else
	{
		return false;
	}
};


