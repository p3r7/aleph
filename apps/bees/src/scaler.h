/* scaler.h
   bees

   parameter-scaling module.

   a scaler converts io_t to full-scale 4-byte parameter representation (native DSP value.) 
   sometimes this can be done by calculation, more often i think by tuned tabled lookup.

   each scaler type must maintain a lookup table size.
   will try and keep these flexible enough that tuning can be performed offline on table data,
   and hopefully from the aleph UI at some point.

   in addition to a DSP use value, 
   scaler must provide human-readable value as well (in 16.16 i guess)
   so, two tables per initialized parameter type.
   
*/

#ifndef _ALEPH_AVR32_BEES_SCALER_H_
#define _ALEPH_AVR32_BEES_SCALER_H_


#include "op_math.h"
#include "param_common.h"

// scaler class
typedef struct _scaler scaler;

// initialize scaler for given param
extern void scaler_init(scaler* sc, const ParamDesc* desc);

// get DSP value 
extern s32 scaler_get_value(scaler* sc, io_t in);
// get readable value
/*
 this datatype can change! 
 usually it is signed 16.16,
 but integer params may need a bigger range of values.
 caller should check the associated paramDesc and cast if needed
*/
extern s32 scaler_get_rep(scaler* sc, io_t in);

#endif
