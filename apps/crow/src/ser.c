/* #include <stdio.h> */
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

        usart_putchar(DEV_USART, '\r');
        usart_putchar(DEV_USART, '\n');
        print_dbg(">> ");

        // print_dbg("\r\nevaluating lua: ");
        // print_dbg((const char*)serialInputLineBuffer);

        if(Lua_eval(L, (const char*)serialInputLineBuffer, serialInputLineBufferPos-2, "=repl")) {
            print_dbg("\r\n!eval");
        }

        print_dbg("\r\n>> ");

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
