#include <stdint.h>
#include <stdio.h>

inline uint64_t rdtsc() {
   uint32_t lo, hi;
   asm volatile (      // serialize
     "xorl %%eax,%%eax \n        cpuid"
     ::: "%rax", "%rbx", "%rcx", "%rdx");
   /* We cannot use "=A", since this would use %rax on x86_64 */
   asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
   return (uint64_t)hi << 32 | lo;
}

int main(void) {

	printf("TSC: %lld\n",  rdtsc());
t1:
	goto t1;
	
	return 0;
}
