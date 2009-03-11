#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>

int
main()
{
  char name[128];
  confstr (_CS_GNU_LIBPTHREAD_VERSION, name, sizeof(name));
  printf ("Pthreads lib: %s\n", name);
  return 0;
}
