#include "Timer.hpp"

void Timer::Start() {
	state = STARTED;
	start_time = SDL_GetTicks();
	if (state == STOPPED) {
		pause_offset = 0;
	} else if (state == PAUSED) {
		pause_offset += (SDL_GetTicks() - pause_time);
	}
}

void Timer::Pause() {
	state = PAUSED;
	pause_time = SDL_GetTicks();
}

void Timer::Stop() {
	state = STOPPED;
}

int Timer::GetMilliseconds() {
	if (state == STOPPED) {
		return 0;
	} else if (state == STARTED) {
		return SDL_GetTicks() - start_time - pause_offset;
	} else { // state PAUSED
		return pause_time - start_time - pause_offset;
	}

	return 0;
}

float Timer::GetSeconds() {
	return GetMilliseconds() / 1000.0f;
}

TIMER_STATE Timer::GetState() {
	return state;
}
