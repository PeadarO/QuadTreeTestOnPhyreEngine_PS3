#pragma once

#include "CommonApp.h"


#include <fw/include/FWTime.h>

#define DEFAULT_TIMEINTERVAL 1.0f

class CCustomTime
{
private:
	double			m_dblInitTime;
	double			m_dblCurrentTime;	
	double			m_dblLastTime;

public:
	void		InitTime();
	void		MarkThisTime( void );
	double		GetElapsedTime( void );

public:
	CCustomTime(void);
	~CCustomTime(void);
};


class CCustomTimer
{
private:
	double				m_dblSecInterval;
	unsigned long		m_ulTickCnt;
	double				m_dElapsedTimeSummary;

public:
	void setInterval(float fSec) 
	{ 
		m_dblSecInterval=fSec;
		m_ulTickCnt = 0;
		m_dElapsedTimeSummary = 0.0f;
	};
	bool getIntervalTick(double dblElapsedTime);

public:
	CCustomTimer()
	{
		m_dblSecInterval = DEFAULT_TIMEINTERVAL;
	};

	~CCustomTimer()
	{

	};
};

