#ifndef _CONSOLE_H_
#define  _CONSOLE_H_

/* Where our sockets will be created */
#define SOCKETROOT "/tmp"

/* The Game of Line play field size */
#define HMAX 20
#define VMAX 80

/* Message buffer max length. We don't have to make it fixed,
   but it's easier this way */
#define MSGLEN ((HMAX * VMAX) + 1024)

void start_life(void);
void bitmap_next(void);
void print_display(char * msg, int size);



#endif /* _CONSOLE_H_ */
