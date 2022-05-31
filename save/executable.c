#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//send to page
//recv from page


char buf[80] = { '\0' };

static void recv_page (int fd, fd_set set) {
int bytes = 0;
//char buf[PIPE_BUF] = { 0 };
FD_ZERO (&set);
FD_SET (fd, &set);
if ((select (fd + 1, &set, NULL, NULL, NULL)) < 1)
exit (1);
if (!FD_ISSET (fd, &set))
return;
if (read (fd, buf, 80) > 0)
;
//printf ("%s\n", buf);
}

static void w_recv_page(void){
    strncpy(buf, "", 80);    
    int recvfd = 0;
    fd_set set;   
    char *recvfifo = "./recv";
    if ((recvfd = open (recvfifo, O_RDWR | O_NONBLOCK)) < 0)
        exit (1);
    while (1) 
        recv_page(recvfd, set);         
    return;
}

static void send_page(void){
   int sendfd;
   char *sendfifo = "./send";
   sendfd = open(sendfifo, O_WRONLY);
   write(sendfd, buf, strlen(buf));    
   close(sendfd);
   strncpy(buf, "", 80);
}

int main() {    
   w_recv_page();
   send_page(); 
}