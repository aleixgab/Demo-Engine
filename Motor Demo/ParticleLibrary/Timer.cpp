#include "Timer.h"
#include <ctime>

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Play()
{
	timeStart = Clock::now();
	if(state == TimerState::StatePaused)
		timePaused += std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - countPause).count();
	
	state = TimerState::StatePlayed;
}

void Timer::Stop()
{
	state = TimerState::StateStopped;
	timePaused = 0.0f;
}

void Timer::Pause()
{
	if (state != TimerState::StatePaused)
	{
		countPause = Clock::now();
		state = TimerState::StatePaused;
	}
}

float Timer::GetTimeMilisec() const
{
	Clock::time_point currentTime = Clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - timeStart).count() - timePaused;
}

float Timer::GetTimeSec() const
{
	Clock::time_point currentTime = Clock::now();
	return std::chrono::duration_cast<std::chrono::seconds>(currentTime - timeStart).count() - (timePaused / 1000);
}