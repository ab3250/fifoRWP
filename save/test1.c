// C program to implement one side of FIFO
// This side writes first, then reads
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int PIPE_BUF = 80;
char buf[80] = { 0 };
static void read_message (int fd, fd_set set) {
  int bytes = 0;
  

  FD_ZERO (&set);
  FD_SET (fd, &set);

  if ((select (fd + 1, &set, NULL, NULL, NULL)) < 1)
    exit (1);
  if (!FD_ISSET (fd, &set))
    return;

  if (read (fd, buf, PIPE_BUF) > 0)
    printf ("%s\n", buf);
}

int main() {
    int infd;
    int outfd = 0;
    char *infifo = "./in";
    
    const char *msg = "Message to broadcast";

    fd_set set;
    char *outfifo = "./out";
 

  if ((outfd = open (outfifo, O_RDWR | O_NONBLOCK)) < 0)
    exit (1);
  while (1) 
    read_message(outfd, set);
    //printf("%s", set);

    infd = open(infifo, O_WRONLY);
    write(infd, buf, strlen(msg));    
    close(infd);



    return 0;
}
