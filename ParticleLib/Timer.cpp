#include <Timer.h>
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

float Timer::GetTime() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - timeStart).count() - timePaused;
}
