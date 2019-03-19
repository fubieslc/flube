#pragma once

#include <SDL.h>

enum TIMER_STATE { STOPPED , STARTED , PAUSED };
struct Timer {
	void Start();
	void Pause();
	void Stop();

	int GetMilliseconds();
	float GetSeconds();

	TIMER_STATE GetState();
private:
	int start_time, pause_time, pause_offset = 0;
	TIMER_STATE state = STOPPED;
};