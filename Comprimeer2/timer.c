#include "timer.h"
#include <time.h>
#include <stdio.h>

int timer_started = 0;
clock_t start;

void start_timer() {
	timer_started = 1;
	start = clock();
}


void print_time() {
	int s, ms; /*seconds and milliseconds*/
	clock_t diff;

	if (timer_started) {
		diff = clock() - start;
		ms = diff * 1000 / CLOCKS_PER_SEC;
		s = ms / 1000;
		ms = ms % 1000;
		printf("%d seconds %d milliseconds \n", s, ms);
	}
	else {
		printf("error: timer must be started first \n");
	}
}
