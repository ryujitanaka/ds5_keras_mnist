# Copyright (C) Arm Ltd 2012-2017

# Select build rules based on Windows or Unix
ifdef WINDIR
DONE=@if exist $(1) echo Build completed.
RM_FILE= if exist $(1) del /q $(1)
RM_DIR = if exist $(1) rmdir /s /q $(1)
MK_DIR = mkdir
SHELL=$(WINDIR)\system32\cmd.exe
SEP=\\
else
ifdef windir
DONE=@if exist $(1) echo Build completed.
RM_FILE= if exist $(1) del /q $(1)
RM_DIR = if exist $(1) rmdir /s /q $(1)
MK_DIR = mkdir
SHELL=$(windir)\system32\cmd.exe
SEP=\\
else
DONE=@if [ -f $(1) ]; then echo Build completed.; fi
RM_FILE= /bin/rm -f $(1)
RM_DIR = /bin/rm -rf $(1)
MK_DIR = /bin/mkdir
SEP=/
endif
endif

TOOLCHAIN=ARM
CC=armcc
AS=armasm
AR=armar

VPATH = $(TOOLCHAIN)

.phony: all clean nearlyclean

############################################
ifeq ($(CPU),A9)
OBJ_DIR = OBJ_A9
LIB_NAME = RTX_CA9_L.lib
CFLAGS=-D__CMSIS_RTOS -D__CORTEX_Ax --cpu=Cortex-A9 -D__FPU_PRESENT \
       --apcs=interwork -g -O0 --thumb --li --split_sections -I. -I../INC -I../../../Include --md --diag_suppress=1786,3731 --depend_dir=$(OBJ_DIR)
AFLAGS=--apcs=interwork --cpu=Cortex-A9 --pd "__CMSIS_RTOS SETA 1" --pd "__FPU_PRESENT SETA 1" -g --depend $(OBJ_DIR)/$*.d

OBJECTS=$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(wildcard rt_*.c)) HAL_CA SVC_Table HAL_CA9))
else
############################################
ifeq ($(CPU),A5)
OBJ_DIR = OBJ_A5
LIB_NAME = RTX_CA5_L.lib
CFLAGS=-D__CMSIS_RTOS -D__CORTEX_Ax --cpu=Cortex-A5.neon -D__FPU_PRESENT \
       --apcs=interwork -g -O0 --thumb --li --split_sections -I. -I../INC -I../../../Include --md --diag_suppress=1786,3731 --depend_dir=$(OBJ_DIR)
AFLAGS=--apcs=interwork --cpu=Cortex-A5.neon --pd "__CMSIS_RTOS SETA 1" --pd "__FPU_PRESENT SETA 1" -g --depend $(OBJ_DIR)/$*.d

OBJECTS=$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(wildcard rt_*.c)) HAL_CA SVC_Table HAL_CA5))
else
############################################
ifeq ($(CPU),A7)
OBJ_DIR = OBJ_A7
LIB_NAME = RTX_CA7_L.lib
CFLAGS=-D__CMSIS_RTOS -D__CORTEX_Ax --cpu=Cortex-A7 -D__FPU_PRESENT \
       --apcs=interwork -g -O0 --thumb --li --split_sections -I. -I../INC -I../../../Include --md --diag_suppress=1786,3731 --depend_dir=$(OBJ_DIR)
AFLAGS=--apcs=interwork --cpu=Cortex-A7 --pd "__CMSIS_RTOS SETA 1" --pd "__FPU_PRESENT SETA 1" -g --depend $(OBJ_DIR)/$*.d

OBJECTS=$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(wildcard rt_*.c)) HAL_CA SVC_Table HAL_CA7))
else
############################################
ifeq ($(CPU),R4)
OBJ_DIR = OBJ_R4
LIB_NAME = RTX_CR4_L.lib
CFLAGS=-D__CMSIS_RTOS -D__CORTEX_Ax --cpu=Cortex-R4F -D__FPU_PRESENT \
       --apcs=interwork -g -O0 --thumb --li --split_sections -I. -I../INC -I../../../Include --md --diag_suppress=1786,3731 --depend_dir=$(OBJ_DIR)
AFLAGS=--apcs=interwork --cpu=Cortex-R4F --pd "__CMSIS_RTOS SETA 1" --pd "__FPU_PRESENT SETA 1" -g --depend $(OBJ_DIR)/$*.d

OBJECTS=$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(wildcard rt_*.c)) HAL_CA SVC_Table HAL_CR4))
else
############################################
$(error No CPU or unrecognised CPU specified.  For example, use "make CPU=A9" or "make CPU=A5" or "make CPU=A7" or "make CPU=R4")
endif
endif
endif
endif

DEP_FILES := $(OBJECTS:%=%.d)

all: ../LIB/ARM/$(LIB_NAME)

../LIB/ARM/$(LIB_NAME): $(OBJ_DIR) $(OBJECTS)
	$(AR) -rv --create $@ $(OBJECTS)

includes = $(wildcard *.h)

nearlyclean:
	$(call RM_FILE,$(OBJ_DIR)$(SEP)*.o)
	$(call RM_FILE,$(OBJ_DIR)$(SEP)*.d)
	$(call RM_DIR,$(OBJ_DIR))

clean: nearlyclean
	$(call RM_FILE,..$(SEP)LIB$(SEP)ARM$(SEP)$(LIB_NAME))

$(OBJ_DIR):
	$(MK_DIR) $@

$(OBJ_DIR)/%.o : %.c $(includes)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o : %.s $(includes)
	$(AS) $(AFLAGS) -o $@ $<

-include $(DEP_FILES)
