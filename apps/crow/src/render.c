/*
  render.c

  aleph-mix

  definitions for rendering graphics.

*/

//--- std headers
#include <string.h>

//--- asf headers
#include "print_funcs.h"

// common
#include "fix.h"
#include "simple_string.h"
#include "types.h"

//--- libavr32 headers
// font-rendering functions
#include "font.h"
// region class
#include "region.h"
// screen refresh function
#include "screen.h"

//--- aleph-specific headers
// application framework
#include "app.h"
// extra drawing routines
#include "region_extra.h"

//--- app-specific headers
#include "ctl.h"
#include "render.h"


//---- extern vars
region* lineRegion = NULL;

char lineBuf[LINEBUF_LEN];
static char *pline = lineBuf;
static char * const pLineEnd = lineBuf + LINEBUF_LEN - 1;

//-------------------------------------------------
//----- -static variables

/*
  the screen-drawing routines in avr32_lib provide the "region" object
  a simple 16-bit pixel buffer with width, height, x/y offset, and dirty flag.
  there are also methods for basic fill and text rendering into regions.
*/

static region lineRegion_pr = 	{ .w=128, .h=8, .x = 0, .y = 0  };

//-------------------------
//---- static funcs

//... nothing here.

//-------------------------------------------------
//----- external functions

// initialze renderer
void render_init(void) {
    // malloc
    region_alloc((region*)(&lineRegion_pr));

    lineRegion = &lineRegion_pr;
}

// fill with initial graphics
void render_startup(void) {
    // fill the graphics buffer (with black)
    region_fill(lineRegion, 0x0);
    // physically render the region data to the screen
    region_draw(lineRegion);
}

// update dirty regions
// (this will be called from an application timer)
void render_update(void) {
    app_pause(); // avr32 mutex thingy? disabled in bees

    // region_draw(lineRegion);

    app_resume(); // avr32 mutex thingy? disabled in bees
}

// append to line buffer
inline void appendln(const char* str) {
    while((*str != 0) && (pline <= pLineEnd)) {
        *pline = *str;
        ++pline;
        ++str;
    }
}

// clear line buffer
inline void clearln(void) {
    pline = lineBuf;
    memset(pline, ' ', LINEBUF_LEN);
}

inline void endln(void) {
    *(pline) = '\0';
}
