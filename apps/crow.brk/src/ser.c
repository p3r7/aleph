#include <stdio.h>
/* #include <avr/io.h> */
#include "ser.h"
#include "serial.h"
#include "usart.h"
// #include "aleph_board.h"
#include "print_funcs.h"

#include "lualink.h"
#include "../../../lua/src/lua.h" // lua_State*

lua_State* L;

static u16 serial_read_pos = 0;


// ------------------------------------------------------------------------
// outptut

void ser_print_char(char c) {
    usart_putchar(DEV_USART, c);
}

// NB: same impl as `print_dbg` in aleph/utils/avr32_sim/src/print_funcs.c
void ser_print(const char* str) {
    printf("%s",str);
}

void ser_println(const char* str) {
    printf("%s\r\n",str);
}

void ser_println_err(const char* msg) {
    ser_print("error: ");
    ser_println(msg);
}

void ser_println_ulong(unsigned long n) {
    printf("%ul\r\n", (unsigned int)n);
}

void ser_println_hex(unsigned long n) {
    printf("%08x\r\n", (unsigned int)n);
}

void ser_prompt() {
    ser_print(">> ");
}


// ------------------------------------------------------------------------
// input

void recv_char (char c) {
    usart_putchar(DEV_USART, c);
    if (c == '\b') {
        serialInputLineBufferPos--;
    } else {
        serialInputLineBuffer[serialInputLineBufferPos] = c;
        serialInputLineBufferPos++;
    }
    if (c == '\n' || c == '\r') {
        /* stdout = fdevopen(test_write); */

        serialInputLineBuffer[serialInputLineBufferPos] = '\n';
        serialInputLineBufferPos++;
        serialInputLineBuffer[serialInputLineBufferPos] = '\0';
        serialInputLineBufferPos++;

        ser_println("");
        ser_prompt();

        if(Lua_eval(L, (const char*)serialInputLineBuffer, serialInputLineBufferPos-2, "=repl")) {
            ser_println("err - failed eval");
        }

        ser_println("");
        ser_prompt();

        serialInputLineBufferPos = 0;
    }
}

void handle_Serial (s32 data) {
    u16 c = (u16)data;
    while(serial_read_pos != c) {
        recv_char(serial_buffer[serial_read_pos]);
        serial_read_pos++;
        if(serial_read_pos == SERIAL_BUFFER_SIZE) {
            serial_read_pos = 0;
        }
    }
    /* serial_process(data); */
}
