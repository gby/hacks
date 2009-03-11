#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>


struct itimerspec tick = { {0, 10 * 1000}, {0, 10 * 1000} } ;

void sigusr(int signal) {

	return;
}

int main (void) {


	timer_t timerid;
	struct timespec ts;
	struct sigevent sigev = {
		.sigev_notify = SIGEV_SIGNAL,
		.sigev_signo = SIGUSR1
	};

	signal(SIGUSR1, sigusr);

	if (timer_create(CLOCK_MONOTONIC, &sigev, &timerid)  == -1) {  
		perror("timer_create");  
		exit(1);  
	} 

	if (timer_settime(timerid, 0, &tick, NULL )== -1) {
		perror("timer_create");  
		exit(1);  
	}

	while(1) {
		pause();
	}

	if (timer_delete(timerid) == -1) {  
		perror("timer_delete");  
		exit(1);  
	}  

	return 0;  
}
