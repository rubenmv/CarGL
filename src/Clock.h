#ifndef CLOCK_H
#define CLOCK_H

#include <ctime>

class Clock
{
public:
	Clock();
	virtual ~Clock();
	bool isStarted();
	bool isStopped();
	bool isPaused();
	bool isActive();

	void pause();
	void resume();
	void stop();
	void start();
	void reset();

	std::clock_t getTicks();
private:
	std::clock_t startedAt;
	std::clock_t pausedAt;
	bool started;
	bool paused;
};

#endif // CLOCK_H
