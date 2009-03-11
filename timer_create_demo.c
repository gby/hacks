#include <stdio.h>
#include <syscall.h>
#include <errno.h>
#include <time.h>
#include <signal.h>



 int main(int ac, char **av)
{
        timer_t created_timer_id; /* holds the returned timer_id*/
        struct sigevent evp;
        int retval;

        evp.sigev_value = (sigval_t) 0;
        evp.sigev_signo = SIGALRM; /* Any signal value works */ 
        evp.sigev_notify = SIGEV_NONE;

        retval = timer_create(CLOCK_REALTIME, &evp,
                                                &created_timer_id);

        if (retval < 0) {
                perror("timer_crete");
                printf("timer_create returned %d\n", retval);
        } else {
                printf("timer_create success");
        }
        return 0;
} /* End of main */ 
