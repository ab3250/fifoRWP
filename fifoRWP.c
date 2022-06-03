#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "chibi/eval.h"
#include <sys/select.h>
#include <sys/poll.h>

  char buf[500] = { '\0' };

static char* scheme_read_ws(char * fifoIn) {
  int bytes = 0;
  fd_set set;
  int fd = 0;
  if ((fd = open (fifoIn, O_RDWR | O_NONBLOCK)) < 0)
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
}

static void scheme_write_ws(char * fifoOut,char* str){
   int sendfd;
   sendfd = open(fifoOut, O_WRONLY);
   write(sendfd, str, strlen(str));    
   close(sendfd);  
}

int fifoPoll (char * fifo)
{
	struct pollfd fds[2];
	int ret;
  int fd = open(fifo, O_RDWR | O_NONBLOCK); //

  if(fd == 1){ 
    perror("file not found");
    return -1;
  }
	/* watch stdin for input */
	fds[0].fd = fd;
	fds[0].events = POLLIN;

  ret = poll(fds, 2, NULL);

	if (ret == -1) {
		perror ("poll");
		return -1;
	}
  
  if (fds[0].revents & POLLIN) {
    return 0;
  } else {
    return 1;
  }
}


sexp sexp_fifoPoll_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  res = sexp_make_integer(ctx, fifoPoll(sexp_string_data(arg0)));
  return res;
}

sexp sexp_scheme_write_ws_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0, sexp arg1) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  if (! sexp_stringp(arg1))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg1);
  res = ((scheme_write_ws(sexp_string_data(arg0), sexp_string_data(arg1))), SEXP_VOID);
  return res;
}

sexp sexp_scheme_read_ws_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  res = sexp_c_string(ctx, scheme_read_ws(sexp_string_data(arg0)), -1);
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
  op = sexp_define_foreign(ctx, env, "scheme_write_ws", 2, sexp_scheme_write_ws_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_STRING);
    sexp_opcode_arg2_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "scheme_read_ws", 1, sexp_scheme_read_ws_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_STRING);
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "gwinit", 0, sexp_gwinit_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
  }
  sexp_gc_release3(ctx);
  return SEXP_VOID;
}



//gcc -lchibi-scheme -Wall -Wextra -O2 -I /usr/local/include -I ./ -std=c11 -fPIC -pthread -shared main.c -o gwsocket.so
