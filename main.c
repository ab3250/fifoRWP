#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "chibi/eval.h"
#include <spawn.h>

//send to page
//recv from page

char buf[500] = { '\0' };


static void read_from_page() {
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
}

static void gwinit(void){
  fflush(stdout);
 //posix_spawn(P_DETACH, "./gwsocket", "--addr=127.0.0.1 -p 8080 --pipein=./send --pipeout=./recv");
}

static void send_to_page(char* str){
   int sendfd;
   char *sendfifo = "./send";
   sendfd = open(sendfifo, O_WRONLY);
   write(sendfd, str, strlen(str));    
   close(sendfd);  
}

char* get_buf(void){
    return buf;
}

void set_buf(char* str){
    printf(str);
    strncpy(buf,str,500);
}

sexp sexp_set_buf_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  res = ((set_buf(sexp_string_data(arg0))), SEXP_VOID);
  return res;
}

sexp sexp_send_to_page_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  res = ((send_to_page(sexp_string_data(arg0))), SEXP_VOID);
  return res;
}

sexp sexp_get_buf_stub (sexp ctx, sexp self, sexp_sint_t n) {
  sexp res;
  res = sexp_c_string(ctx, get_buf(), -1);
  return res;
}

sexp sexp_read_from_page_stub (sexp ctx, sexp self, sexp_sint_t n) {
  sexp res;
  res = ((read_from_page()), SEXP_VOID);
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
  op = sexp_define_foreign(ctx, env, "set_buf", 1, sexp_set_buf_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "send_to_page", 1, sexp_send_to_page_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "get_buf", 0, sexp_get_buf_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "read_from_page", 0, sexp_read_from_page_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
  }
  op = sexp_define_foreign(ctx, env, "gwinit", 0, sexp_gwinit_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
  }
  sexp_gc_release3(ctx);
  return SEXP_VOID;
}

