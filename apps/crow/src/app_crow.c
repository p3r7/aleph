/*
  app_crow.c

  aleph/app/crow

  required app-specific implementation of avr32_lib/src/app.h
*/

// asf
#include "delay.h"
#include "gpio.h"
#include "print_funcs.h"
#include "sd_mmc_spi.h"

// avr32_lib
#include "app.h"
#include "bfin.h"
#include "encoders.h"

//--- app-specific
#include "app_timers.h"
#include "ctl.h"
#include "handler.h"
#include "render.h"

// #include "files.h"

#include "lualink.h"


// ------------------------------------------------------------------------
// lifecycle

// this is called during hardware initialization.
// allocate memory here.
void app_init(void) {
    // print_dbg("\r\n files_init...");
    // files_init();

    print_dbg("\r\n render_init...");
    render_init();
}

// this is called from the event queue to start the app
// return >0 if there is an error doing firstrun init
u8 app_launch(eLaunchState state) {

  print_dbg("\r\n launching app with state: ");
  print_dbg_ulong(state);

  print_dbg("\r\n initialising crow...");

  Lua_Init();

  //// startup using default DSP name
  // files_load_dsp_name("waves.ldr");

  bfin_wait_ready();

  //    print_dbg("\r\n enable DSP audio...");
  bfin_enable();


  // extra few ms...
  delay_ms(100);
  //==========================

  // enable timers
  init_app_timers();

  // render initial screen
  render_startup();
  render_update();

  // set hardcoded default values
  ctl_init();

  if(state == eLaunchStateFirstRun) {
      // this was the first run since firwmare was flashed.
      // do any necessary flash initialization here.
  } else {
      if(state == eLaunchStateClean) {
          // use this condition to launch in a "default" or "clean" mode,
          // with known settings
      } else {
          // go ahead and load stored state from flash...
      }
  }


  // set app event handlers
  assign_event_handlers();

  // tell the main loop that we launched successfully.
  // if this was the first run,
  // main() should now write the firstrun pattern to flash and reboot.
  return 1;
}
