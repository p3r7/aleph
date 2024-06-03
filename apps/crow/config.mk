# --- config.mk
#
# --- customized makefile for aleph-avr32 application.
# --- this is included via the ASF utility makefile.

# app name
APP = crow

# baudrate! can override in make invocation
BAUD = 115200

# avr32 configuration
include ../aleph_avr32_config.mk
# avr32 sources
include ../aleph_avr32_src.mk

# CSRCS += $(APP_DIR)/../../avr32/src/fat_io_lib/fat_filelib_comp.h
CSRCS += $(APP_DIR)/src/fat_filelib_comp.h

LUAS=$(APP_DIR)/../../lua/src
LUACORE_SRC=	lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c \
		lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c \
		ltm.c lundump.c lvm.c lzio.c
LUALIB_SRC=	lauxlib.c lbaselib.c lbitlib.c lcorolib.c liolib.c \
		lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c linit.c

CSRCS += $(addprefix $(LUAS)/,$(LUACORE_SRC) $(LUALIB_SRC) )

# app sources
CSRCS += \
	$(APP_DIR)/src/app_crow.c \
	$(APP_DIR)/src/app_timers.c \
	$(APP_DIR)/src/ctl.c \
	$(APP_DIR)/src/handler.c \
	$(APP_DIR)/src/render.c \
	$(APP_DIR)/src/ser.c \
	$(APP_DIR)/src/lualink.c

# List of assembler source files.
ASSRCS +=

# List of include paths.
INC_PATH += \
	$(APP_DIR)	           \
	$(APP_DIR)/src	\
	$(LUAS)	\
	$(APP_DIR)/../../../../avr32-gnu-toolchain/include

# $(APP_DIR)/../../../avr32-toolchain-linux/include
