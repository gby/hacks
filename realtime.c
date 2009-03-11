#include <sched.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char * argv[]) {
  
  struct sched_param sched;

  
  if(argc < 3) {
    printf("%s priority program [ parameters... ]\n", argv[0]);
    exit(0);
  }

  sched.sched_priority = atoi(argv[1]);

  if(sched.sched_priority > 99) {
    printf("%s: priority must be between 0 - 99!\n", argv[0]);
    exit(EINVAL);
  }
  
  if(sched_setscheduler(0, SCHED_FIFO, &sched)) {
    perror(argv[0]);
    exit(errno);
  }


  printf("%s: executing %s in realtime priority  %d...\n", argv[0], argv[2], sched.sched_priority);

  execvp(argv[2], &argv[2]);

  return 0;
}
