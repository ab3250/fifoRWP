#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "chibi/eval.h"
//#include <spawn.h>
#include <sys/select.h>
#include <sys/poll.h>

#define TIMEOUT 5

char buf[500] = { '\0' };


static char* scheme_read_ws() {
  int bytes = 0;
  fd_set set;
  char *recvfifo = "./recv";
  int fd = 0;
  if ((fd = open (recvfifo, O_RDWR | O_NONBLOCK)) < 0)
    exit (1);
  FD_ZERO (&set);
  FD_SET (fd, &set);
  if ((select (fd + 1, &set, NULL, NULL, NULL)) < 1)
    exit (1);
  if (!FD_ISSET (fd, &set))
    return;
  if (read (fd, buf, 500) > 0){}else{}
  return buf;
}

static void gwinit(void){
  fflush(stdout);
 //posix_spawn(P_DETACH, "./gwsocket", "--addr=127.0.0.1 -p 8080 --pipein=./send --pipeout=./recv");
}

static void scheme_write_ws(char* str){
   int sendfd;
   char *sendfifo = "./send";
   sendfd = open(sendfifo, O_WRONLY);
   write(sendfd, str, strlen(str));    
   close(sendfd);  
}

char * get_buf(void){
    return buf;
}

void set_buf(char* str){
    printf(str);
    strncpy(buf,str,500);
}

int fifoPoll (char * fifo)
{
	struct pollfd fds[2];
	int ret;
  int fd = open(fifo, O_RDWR | O_NONBLOCK); //
	/* watch stdin for input */
	fds[0].fd = fd;
	fds[0].events = POLLIN;

	/* watch stdout for ability to write */
	fds[1].fd = STDOUT_FILENO;
	fds[1].events = POLLOUT;

	ret = poll(fds, 2, TIMEOUT * 1000);

	if (ret == -1) {
		perror ("poll");
		return 1;
	}

	if (!ret) {
		printf ("%d seconds elapsed.\n", TIMEOUT);
		return 0;
	}

	//if (fds[0].revents & POLLIN)
	//	printf ("stdin is readable\n");

if (fds[0].revents & POLLIN){
  return 1;
  }else{
    return 0;
  }
	return ret;

}



// int fifoPoll (char * file)
// {
// 	struct pollfd fds[2];
// 	int ret;
// 	int fd;
// 	if (!(fd = open (file, O_RDWR | O_NONBLOCK))){
// 		perror("file not found!");
// 		return -1;
// 	}
	
// 	/* watch stdin for input */
// 	fds[0].fd = STDIN_FILENO;
// 	fds[0].events = POLLIN;

	
// 	ret = poll(fds, 2, TIMEOUT * 1000);

// 	if (ret == -1) {
// 		perror ("poll");
// 		return -1;
// 	}

// 	// if (!ret) {
// 	// 	printf ("%d seconds elapsed.\n", TIMEOUT);
// 	// 	return 1;
// 	// }

// 	if (fds[0].revents & POLLIN){
// 		return 1;
// 	}else{
// 		return 0;
// 	}

// }

// int chready(void)
// {
//     //fd_set readCheck;
//     //fd_set errCheck;
//     fd_set set;
//    // char buffer[64];
//     struct timeval timeout;
//     int rv;
// //

//  // printf("%i", 1);
// //
//     int fd = open("./recv", O_RDWR | O_NONBLOCK);

//     //FD_ZERO(&readCheck);
//    // FD_ZERO(&errCheck);

//     //FD_SET(fd, &readCheck);
//     //FD_SET(fd, &errCheck);

//     FD_ZERO(&set);
//     FD_SET(fd, &set);

//     timeout.tv_sec = 0;
//     timeout.tv_usec = 5000;
 

//      rv = select(fd+1, &set, NULL, NULL, NULL);  
// //   //  if (FD_ISSET(fd, &readCheck)) {
// //   //           memset(buffer, 0, sizeof(buffer));
// //   //           rv = read(fd, buffer, sizeof(buffer));
// //   //           if (rv < 0) {
// //   //               printf("Read failed\r\n");
// //   //               break;
// //   //           }
// //   //           buffer[rv] = '\0';
// //   // }

//   close(fd);
//   if (FD_ISSET(fd, &set)){
    
//   return 1;}else{
   
//     return 0;
//   }
    
// }


// int chready (void)
// {
// 	struct pollfd fds[2];
// 	int ret;
//   int fd = open("./recv", O_RDWR | O_NONBLOCK); //
// 	/* watch stdin for input */
// 	fds[0].fd = fd;
// 	fds[0].events = POLLIN;

// 	/* watch stdout for ability to write */
// 	fds[1].fd = STDOUT_FILENO;
// 	fds[1].events = POLLOUT;

// 	ret = poll(fds, 2, TIMEOUT * 1000);

// 	if (ret == -1) {
// 		perror ("poll");
// 		return 1;
// 	}

// 	if (!ret) {
// 		printf ("%d seconds elapsed.\n", TIMEOUT);
// 		return 0;
// 	}

// 	if (fds[0].revents & POLLIN)
// 		printf ("stdin is readable\n");

// //	if (fds[1].revents & POLLOUT)
// //		printf ("stdout is writable\n");
// if (fds[0].revents & POLLIN) {return 1;} else {return 0;}
// 	return ret;

// }




sexp sexp_fifoPoll_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  res = sexp_make_integer(ctx, fifoPoll(sexp_string_data(arg0)));
  return res;
}

sexp sexp_set_buf_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  res = ((set_buf(sexp_string_data(arg0))), SEXP_VOID);
  return res;
}

sexp sexp_scheme_write_ws_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  res = ((scheme_write_ws(sexp_string_data(arg0))), SEXP_VOID);
  return res;
}

sexp sexp_get_buf_stub (sexp ctx, sexp self, sexp_sint_t n) {
  sexp res;
  res = sexp_c_string(ctx, get_buf(), -1);
  return res;
}

sexp sexp_scheme_read_ws_stub (sexp ctx, sexp self, sexp_sint_t n) {
  sexp res;
  res = sexp_c_string(ctx, scheme_read_ws(), -1);
  return res;
}

sexp sexp_gwinit_stub (sexp ctx, sexp self, sexp_sint_t n) {
  sexp res;
  res = ((gwinit()), SEXP_VOID);
  return res;
}


sexp sexp_init_library (sexp ctx, sexp self, sexp_sint_t n, sexp env, const char* version, const sexp_abi_identifier_t abi) {
  sexp_gc_var3(name, tmp, op);
  if (!(sexp_version_compatible(ctx, version, sexp_version)
        && sexp_abi_compatible(ctx, abi, SEXP_ABI_IDENTIFIER)))
    return SEXP_ABI_ERROR;
  sexp_gc_preserve3(ctx, name, tmp, op);
  op = sexp_define_foreign(ctx, env, "fifoPoll", 1, sexp_fifoPoll_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "set_buf", 1, sexp_set_buf_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "scheme_write_ws", 1, sexp_scheme_write_ws_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "get_buf", 0, sexp_get_buf_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "scheme_read_ws", 0, sexp_scheme_read_ws_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "gwinit", 0, sexp_gwinit_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
  }
  sexp_gc_release3(ctx);
  return SEXP_VOID;
}




//gcc -lchibi-scheme -Wall -Wextra -O2 -I /usr/local/include -I ./ -std=c11 -fPIC -pthread -shared main.c -o gwsocket.so
