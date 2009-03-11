/* a simple console client for use from the shell  */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "console.h"

void error (char *);

int
main (int argc, char *argv[])
{
  /* Socket stuff and buffers */
  int sockfd, servlen, n;
  struct sockaddr_un serv_addr;
  char buffer[MSGLEN];
  char socketpath[256];

  /* You need to give the name of a daemon and the message to send */

  if (argc != 3)
    {
      printf ("Usage: %s [server name] [message]\n", argv[0]);
      exit (0);
  }

   bzero(buffer, sizeof(buffer));

  /* Build our socket */
  snprintf(socketpath, sizeof(socketpath), "%s/%s", SOCKETROOT, argv[1]);

 /* connect to socket */
  bzero ((char *) &serv_addr, sizeof (serv_addr));
  serv_addr.sun_family = AF_UNIX;
  strcpy (serv_addr.sun_path, socketpath);
  servlen = strlen (serv_addr.sun_path) + sizeof (serv_addr.sun_family);

  if ((sockfd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
    error ("Creating socket");

  if (connect (sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
    error ("Connecting");

   /* Send what we have to say and print reply to terminal */
   write (sockfd, argv[2], strlen(argv[2]));
   n = read (sockfd, buffer, MSGLEN);
   printf ("%s#\n%s\n", argv[1], buffer);

  return 0;
}

void
error (char *msg)
{
  perror (msg);
  exit (0);
}
