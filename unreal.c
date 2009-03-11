#include <stdio.h>
#include <sched.h>
#include <sys/types.h>

int main(int argc, char * argv[]) {
 
	pid_t pid=0;
	struct sched_param sp = { .sched_priority=0 };

	if(2==argc) 
		pid = atoi(argv[1]);

	if(!pid) {
		printf("Usage: %s [PID]\nSteal a proccess real time priority.\n", argv[0]);
		exit(0);
	}

       return (sched_setscheduler(pid, SCHED_OTHER, &sp));
}
 

