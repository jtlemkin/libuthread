#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction sa;
static bool p_enabled = false;

void preempt_disable(void)
{
	printf("Prempt disabled\n");
	p_enabled = false;
}

void preempt_enable(void)
{
	printf("Prempt enabled\n");
	p_enabled = true;
}

void alarm_handler(int signum)
{
	printf("Tick\n");

	// Only yield if interrupts enabled
	if (p_enabled) {
		uthread_yield();
	}

	// Perpetuate this so even if p_enabled is temporary false
	// that the timer isn't killed
	alarm(1/HZ);
}

void preempt_start(void)
{
	printf("Starting alarm\n");
	sa.sa_handler = &alarm_handler;
	// sa_mask specifies a mask of signals that should blocked
	// sigemptyset empties the mask so no signals are blocked
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL); //signal handler is called on alarm

	alarm(1/HZ);
}

void preempt_stop(void)
{
	printf("Stopping alarm\n");
	sa.sa_handler = NULL;
}

