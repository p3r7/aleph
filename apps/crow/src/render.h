#ifndef _ALEPH_APP_MIX_RENDER_H_
#define _ALEPH_APP_MIX_RENDER_H_

#define LINEBUF_LEN 32

// avr32
#include "region.h"
#include "font.h"

extern char lineBuf[LINEBUF_LEN];

extern region* lineRegion;

// init
extern void render_init(void);

// startup state
extern void render_startup(void);

// render a channel
extern void render_chan(u8 ch);

// update
extern void render_update(void);

// append to line buffer
extern void appendln( const char* str);

// clear line buffer, and reset position
extern void clearln( void);
// end line buffer (write a zero to next position)
extern void endln( void);

#endif // h guard
