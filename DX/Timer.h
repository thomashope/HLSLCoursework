// timer.h
#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>

class Timer
{
public:
	Timer();
	~Timer();

	void Frame();

	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerS;
	INT64 m_startTime;
	float m_frameTime;
};

#endif