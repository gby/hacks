#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/**********************************
 * Compile with:
 * gcc bt.c -rdynamic -o bt
 * 
 * Note that you must use the -rdynamic
 * to see function names
 */ 


/* Obtain a backtrace and print it to stdout. */
void
print_trace (void)
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  printf ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
     printf ("%s\n", strings[i]);

  free (strings);
}

/* The fault handler function */
void
fault_handler (int dummy)
{
  print_trace ();
  exit(-1);
}


void * dummy_func1(int dummy) {

 int * p = NULL;

 *p = 1;
  return NULL;
}


void * dummy_func2(int dummy) {

 return dummy_func1(dummy);
}

int
main (void)
{

  signal(SIGSEGV, fault_handler);

  dummy_func2(12);


  return 0;
}
