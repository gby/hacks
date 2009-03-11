
/* a server in the unix domain.  The pathname of 
   the socket address is passed as an argument */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <signal.h>
#define _GNU_SOURCE
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "console.h"

/* UNIX file system acceptable version of my name (no spaces, dashes, etc.) */
#define MYNAME "demo_server"

/* Path to my socket */
#define SOCKETPATH SOCKETROOT "/" MYNAME  

/* Forward declerations */
void error (char *);
void dtor (void);
void term_handler (int sig);
void pipe_handler (int sig);
void *worker_thread_func (void *dummy);
void *console_thread_func (void *dummy);

/* This stream handle will be our log file stresm */
FILE * logfile = NULL;

/* This boolean flag is used to signal we wish to terminate */
int term_pending = 0;

/* This mutex object is used to synchronise the console and the worker thread */
pthread_mutex_t msync;


int main (int argc, char *argv[])
{
  /* This thread does the "real work". In real life there could be several. */
  pthread_t worker_thread;
  /* This thread deals with debug console */
  pthread_t console_thread;
  /* We need custom attributes for the worker thread */
  pthread_attr_t worker_thread_attr;


  /* Tell them what's going to happen */
  printf("%s going into background... \n", MYNAME);

  /* Fork into the background  and become a daemon*/
  daemon(0,0);

  /* Open the log file */
  logfile = fopen("/tmp/log.txt", "w");
  	if(!logfile) {
  		exit(1);
  	}
  
  /* Register a destructor function to be called at exit */
  atexit (dtor);
  /* register a handler for some exceptions */
  signal (SIGTERM, term_handler);
  signal (SIGINT, term_handler);
  /* We don't want to crap out if someone press CTRL_C during
     the time the client is connected to us, so mask out 
     SIGPIPE */
  signal (SIGPIPE, SIG_IGN);

  /* Init our mutex */
  pthread_mutex_init(&msync, NULL);
  
  /* Create the console thread. Use default attributes */
  pthread_create (&console_thread, NULL, console_thread_func, NULL);

  /* We want the worker thread to be in detached state, so make it so */
  pthread_attr_init (&worker_thread_attr);
  pthread_attr_setdetachstate (&worker_thread_attr, PTHREAD_CREATE_DETACHED);
  pthread_create (&worker_thread, &worker_thread_attr, worker_thread_func,
		  NULL);
  pthread_attr_destroy (&worker_thread_attr);

  /* When console thread finishes it's time to say goodbye */
  pthread_join (console_thread, NULL);

  return 0;
}


/* Main console thread function */

void *
console_thread_func (void *dummy)
{

  /* socket stuff */
  int sockfd, newsockfd, servlen, n;
  unsigned int clilen;
  struct sockaddr_un cli_addr, serv_addr;
  /* buffer to get messages into */
  char buf[MSGLEN];
  /* Buffer to send messages. We could re-use buf, 
     but it's ugly */
  char msg[MSGLEN];
  /* Raise this flag to quit */
  int quit = 0;
  /* Raise this flag to go back to waiting for connection
     after client exit */
  int reconn=0;
  
  srand(getpid());

   bzero(msg, sizeof(buf));

  /* Get a socket and bind to it */
  if ((sockfd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
    error ("creating socket");

  bzero ((char *) &serv_addr, sizeof (serv_addr));
  serv_addr.sun_family = AF_UNIX;
  strcpy (serv_addr.sun_path, SOCKETPATH);
  servlen = strlen (serv_addr.sun_path) + sizeof (serv_addr.sun_family);

  if (bind (sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
    error ("binding socket");

reconnect:

  /* set reconn flag to zero in case we got here after client
     disconnected */
  reconn=0;

  /* Wait for connection */
  listen (sockfd, 5);

  /* OK, we got a connection request, establish a socket with client.
     If we want we can support multiple clients by keeping an array 
     or list of "newsockfd"'s, but this is just a simple demo */

  clilen = sizeof (cli_addr);
  newsockfd = accept (sockfd, (struct sockaddr *) &cli_addr, &clilen);

  if (newsockfd < 0)
    error ("accepting");

  /* Our client only sends one message each time, but
     we support many messages on the same connection */
  do
    {

      /* read from the socket */
      n = read (newsockfd, buf, MSGLEN);

      if (n <=0) {
	/* Some error occured. If we got interrupted by signal,
	   Just try again. Otherwise, assume client disconnected on us 
	   and just raise the flag to go back to waiting for another 
	   connection */
	if(EINTR != errno) reconn=1;
	break;
      }

      switch (*buf)
	{

	/* Parse the message from client and deal with it */

	case 'q':
	  snprintf (msg, sizeof (msg), "Server quits.");
	  fprintf (logfile, "Server terminated on user request\n");
	  quit = 1;
	  break;
	  
	case 'c':
	  pthread_mutex_lock(&msync);
	  snprintf (msg, sizeof (msg), "Re-starting life.");
	  fprintf (logfile, "Re-starting life.\n");
	  start_life();
	  pthread_mutex_unlock(&msync);
	  break;
	  
	case 'p':
	  pthread_mutex_lock(&msync);
	  print_display(msg, sizeof(msg));
	  pthread_mutex_unlock(&msync);
	  break;


	default:
	  snprintf (msg, sizeof (msg), "Unknown command");
	  break;
	}

	/* Same logic as read above */
      if((write(newsockfd, msg, MSGLEN) <= 0) && errno != EINTR) reconn=1;

    } while (!quit && !reconn); /* if we were asked to quit or to reconnect do it, otherwise just loop */

   /* Either the socket is dead or we want to quit. Release it */
  close(newsockfd);

  /* If we need to re connect go back */
  if(reconn) goto reconnect;

  /* Otherwise, the thread work is finished */
  return NULL;

}

/* On error send message to log and quit (dtor will be called) */
void
error (char *msg)
{
  char buf[256];
  fprintf (logfile, "%s: %s", msg, strerror_r (errno, buf, sizeof (buf)));
  exit (1);
}

/* Do this when exiting */
void
dtor (void)
{

  /* Close log connection. */
  fclose(logfile);
  
  /* Destroy our mutex */
  pthread_mutex_destroy(&msync);
  /* Remove our socket */
  unlink (SOCKETPATH);
  return;
}

/* This is an signal handler that is called when certain signals
   are sent to us. We just set a flag. Doing anything else is
   dangerous because of the signal async. nature. 

   Specifically, calling fprintf here is a mistake that can
   cause very obscure and hard to find bugs 
   */
   
void
term_handler (int sig)
{
	term_pending = 1;

	return;

}

/* This is the demo thread  that does the actuall work*/

void *
worker_thread_func (void *dummy)
{

  start_life();

  while (!term_pending) {
	  pthread_mutex_lock(&msync);
	  bitmap_next();
	  pthread_mutex_unlock(&msync);
	  sleep(1);
  }

  fprintf(logfile, "Signal caught. Time to die.\n");

  exit(0);

  /* NOT REACHED */
  return NULL;
}
