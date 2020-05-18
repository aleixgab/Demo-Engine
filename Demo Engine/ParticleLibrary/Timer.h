#ifndef __Timer_H__
#define __Timer_H__

#include <chrono>

class Timer
{
	typedef std::chrono::high_resolution_clock Clock;
	
	enum TimerState
	{
		StateStopped,
		StatePlayed,
		StatePaused
	};

public:
	Timer();
	~Timer();

	//Start or continue the timer
	void Play();
	//Stop the timer
	void Stop();
	//Pause the timer
	void Pause();

	//Returns the time running with miliseconds
	float GetTime() const;

private:
	Clock::time_point timeStart;
	Clock::time_point countPause;

	float timePaused;
	float timeScale = 1.0f;

	TimerState state = TimerState::StateStopped;
};

#endif