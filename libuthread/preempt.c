#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
#define ONE_MILLION 1000000

struct itimerval timer;

void preempt_disable(void)
{
	int result;
	//printf("P-disable\n");
	// Initialize set for use in enable and disable
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGVTALRM);
	result = sigprocmask(SIG_BLOCK, &set, NULL);
	assert(result == 0);
}

void preempt_enable(void)
{
	int result;
	//printf("P-enable\n");
	// Initialize set for use in enable and disable
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGVTALRM);
	result = sigprocmask(SIG_UNBLOCK, &set, NULL);
	assert(result == 0);
}

void alarm_handler(int signum)
{
	uthread_yield();
}

void preempt_start(void)
{
	//printf("Starting alarm\n");
	struct sigaction sa;
	sa.sa_handler = alarm_handler;
	// sa_mask specifies a mask of signals that should blocked
	// sigemptyset empties the mask so no signals are blocked
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL); //signal handler is called on alarm

	// Configure time to go off after one timespan
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 1 * (ONE_MILLION / HZ);
	// And to then go off after every one timespan
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 1 * (ONE_MILLION / HZ);

	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void preempt_stop(void)
{
	struct sigaction sa;
	sa.sa_handler = NULL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 0;

	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

