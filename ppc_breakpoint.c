#include <stdio.h>
#include <signal.h>

#define BREAKPOINT() asm(".long 0x7d821008"); /* twge r2, r2 */


void trap_this(int signal) {
	printf("BREAKPOINT!\n");
	return;
}

int main(void) {

	signal(SIGTRAP, &trap_this);

	printf("About to call breakpoint...\n");
	BREAKPOINT();
	printf("After breakpoint.\n");

	return 0;
}
