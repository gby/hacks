#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


static void test_handler (int signal, siginfo_t * siginfo, void *context)
{

	char * msg = "\n*** TEST HANDLER CALLED ***\n";
        write(fileno(stdout), msg, strlen(msg)+1);
	return;
}

/* OK, we want a way to call the register function (not to mention load the library)
 * without messing with source files and re-compiling. Here how it works:
 * 
 * We create a library with a single init function that calls the registration for us
 * using a function constructor hook and we'll force the dynamic linker to loda it
 * in run time using LD_PRELOAD magic.
 * 
 * Pretty, clever, isn't it? :-)
 */

static void __attribute__ ((constructor)) magic_handler_init(void);

static void magic_handler_init(void) {

	struct sigaction act;
	
	/* Print some meaningfull message so they'll know that we're running */
	fprintf(stderr, "\n*** magic handler loaded. Registering...");

	/* Prepare a sigaction struct for exception handler registrations */
	memset(&act, 0, sizeof (act));
	act.sa_sigaction = test_handler;
	/* No signals during handler run, please */
	sigfillset (&act.sa_mask);
	/* We want the 3 parameter form of the handler with the siginfo_t addtional data */
	act.sa_flags = SA_SIGINFO;

	/* Register the handler for all exception signals. */
	sigaction (SIGUSR1, &act, NULL);

	return;
}

/* THE END */
