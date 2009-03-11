#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fcntl.h>
#include <sys/mman.h>

static char * progname;

#define PAGE_SIZE (4096)

void usage(void) {

	printf("Usage: %s [filename]\n", progname);
	return;
}


int main(int argc, char * argv[]) {

	const char * filename;
	int fd, ret;

	char *buffer;

	progname = argv[0];

	if (argc != 2) {
		usage();
		exit(0);
	}

	filename = argv[1];

	ret = posix_memalign(&buffer, 512, PAGE_SIZE);

	if(ret) {
                printf("%s: %s", progname, strerror(ret));
                exit(-5);
        }

	printf("%s: Got aligned buffer %p\n", progname, buffer);
	
	fd = open(filename, O_RDWR|O_CREAT|O_DIRECT, S_IRWXU);

	if(-1 == fd) {
		perror(progname);
		exit(-1);
	}

	strcpy(buffer, "testing testing 1 2 3!");
	
	ret = write(fd, buffer, PAGE_SIZE);
                                                                                
               if(-1 == ret) {
                perror(progname);
                exit(-2);
        }
                                                                                
        printf("%s: Written: %s\n", progname, buffer);

	lseek(fd, SEEK_SET, 0);

        ret = read(fd, buffer, PAGE_SIZE);
                                                                                                                             
               if(-1 == ret) {
                perror(progname);
                exit(-2);
        }
                                                                                                                             
        printf("%s: Got %s\n", progname, buffer);



	return 0;
}
	

	
