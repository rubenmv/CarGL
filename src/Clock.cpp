#include "Clock.h"

Clock::Clock()
{
	startedAt = 0;
	pausedAt = 0;
	paused = false;
	started = false;
}

Clock::~Clock()
{
}

bool Clock::isStarted()
{
	return started;
}

bool Clock::isStopped()
{
	return !started;
}

bool Clock::isPaused()
{
	return paused;
}

bool Clock::isActive()
{
	return !paused & started;
}

void Clock::pause()
{
	if( paused || !started )
		return;

	paused = true;
	pausedAt = std::clock();
}

void Clock::resume()
{
	if( !paused )
		return;

	paused = false;
	startedAt += std::clock() - pausedAt;
}

void Clock::stop()
{
	started = false;
}

void Clock::start()
{
	if( started )
		return;

	started = true;
	paused = false;
	startedAt = std::clock();
}

void Clock::reset()
{
	paused = false;
	startedAt = std::clock();
}

std::clock_t Clock::getTicks()
{
	if( !started )
		return 0;

	if( paused )
		return pausedAt - startedAt;

	return std::clock() - startedAt;
}

