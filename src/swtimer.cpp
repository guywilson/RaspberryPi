#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>

#include "swtimer.h"
#include "exception.h"

#define MAX_NANOSECOND_VAL			999999999L

static int 				_maxTimerId = 0;
static uint64_t			_timer[NUM_TIMERS];

void SWTimer::_shortDelay(unsigned long delay)
{
	struct timeval		tNow;
	struct timeval		tEnd;
	struct timeval		tDelay;

	gettimeofday(&tNow, NULL);
	
	tDelay.tv_sec = (time_t)0;
	tDelay.tv_usec = (long)delay;
	
	timeradd(&tNow, &tDelay, &tEnd);
	
	while (timercmp(&tNow, &tEnd, <)) {
		gettimeofday(&tNow, NULL);
	}
}

void SWTimer::usDelay(unsigned long delay)
{
	struct timespec t;

	if (delay < 150) {
		_shortDelay(delay);
	}
	
	t.tv_sec  = (time_t)(delay / 1000000);
	t.tv_nsec = (long)(delay % 1000000) * 1000;

	nanosleep(&t, NULL);
}

void SWTimer::msDelay(unsigned long delay)
{
	struct timespec t;

	t.tv_sec  = (time_t)(delay / 1000);
	t.tv_nsec = (long)(delay % 1000) * 1000000;

	nanosleep(&t, NULL);
}

void SWTimer::ssDelay(unsigned long delay)
{
	struct timespec t;

	t.tv_sec  = (time_t)delay;
	t.tv_nsec = 0L;

	nanosleep(&t, NULL);
}

int SWTimer::start()
{
	struct timespec tNow;
	int				id;
	
	if (_maxTimerId >= (NUM_TIMERS - 1)) {
		throw new Exception(
					ERR_NO_TIMERS_AVAILABLE,
					"Unable to allocate timer, no timers available",
					__FILE__,
					"SWTimer",
					"start()",
					__LINE__);
	}
	
	clock_gettime(CLOCK_MONOTONIC_RAW, &tNow);
	
	_timer[_maxTimerId] = (tNow.tv_sec * 1000000) + (tNow.tv_nsec / 1000);
	
	id = _maxTimerId;
	
	_maxTimerId++;
	
	return id;
}

unsigned long SWTimer::end(int timerId)
{
	struct timespec tNow;
	uint64_t		usNow;
	unsigned long	usTimePassed;
	
	clock_gettime(CLOCK_MONOTONIC_RAW, &tNow);
	
	usNow = (tNow.tv_sec * 1000000) + (tNow.tv_nsec / 1000);
	
	usTimePassed = usNow - _timer[timerId];
	
	_maxTimerId--;
	
	return usTimePassed;
}
