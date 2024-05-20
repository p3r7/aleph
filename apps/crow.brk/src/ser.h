#ifndef _SER_H_
#define _SER_H_
#include "types.h"

void ser_print_char(char);
void ser_print(const char*);
void ser_println(const char*);
void ser_println_err(const char*);
void ser_println_hex(unsigned long);
void ser_println_ulong(unsigned long);
void ser_prompt(void);

void recv_char (char c);

char serialInputLineBuffer[1024];
char serialOutputLineBuffer[1024];
int serialInputLineBufferPos;
void handle_Serial (s32 data);
#endif // header guard
