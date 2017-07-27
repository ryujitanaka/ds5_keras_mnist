# TrustZone Example Makefile
#
# Copyright (C) ARM Limited, 2011. All rights reserved.
#
# This makefile is intended for use with GNU make
# This example is intended to be built with ARM Compiler 5

TARGET=TrustZone_with_NEON.axf

CC=armcc
AS=armasm
LD=armlink
AR=armar
FE=fromelf

# Select build rules based on Windows or Unix
ifdef WINDIR
DONE=@if exist $(1) echo Build completed.
RM=if exist $(1) del /q $(1)
SHELL=$(WINDIR)\system32\cmd.exe
else
ifdef windir
DONE=@if exist $(1) echo Build completed.
RM=if exist $(1) del /q $(1)
SHELL=$(windir)\system32\cmd.exe
else
DONE=@if [ -f $(1) ]; then echo Build completed.; fi
RM=rm -f $(1)
endif
endif


all: $(TARGET)
	$(call DONE,$(TARGET))

rebuild: clean all

clean:
	$(call RM,*.o)
	$(call RM,$(TARGET))


$(TARGET): startup_normal.s main_normal.c scatter_normal.scat startup_secure.s main_secure.c cnn.c cnn.h bp147_tzpc.c bp147_tzpc.h monitor.s scatter_secure.scat
# Assemble common routines
	$(AS) -g --cpu=Cortex-A9 v7.s -o v7.o
# Compile normal world code
	$(AS)    -g --cpu=Cortex-A9 startup_normal.s -o startup_normal.o
	$(CC) -c -g --cpu=Cortex-A9 main_normal.c -o main_normal.o -O3 -Otime --vectorize
# Link normal world code and create binary
	$(LD) main_normal.o startup_normal.o v7.o --scatter=scatter_normal.scat --entry=normalStart -o normal.axf
	$(FE) --bin -o normal.bin normal.axf
# Compile secure world code
	$(AS)    -g --cpu=Cortex-A9 startup_secure.s -o startup_secure.o
	$(CC) -c -g --cpu=Cortex-A9 main_secure.c -o main_secure.o -O3 -Otime --vectorize
	$(CC) -c -g --cpu=Cortex-A9 cnn.c -o cnn.o -O3 -Otime --vectorize
	$(CC) -c -g --cpu=Cortex-A9 bp147_tzpc.c -o bp147_tzpc.o -O1
	$(AS)    -g --cpu=Cortex-A9 monitor.s -o monitor.o
# Link final executable (secure + normal)
	$(LD) main_secure.o startup_secure.o cnn.o v7.o monitor.o bp147_tzpc.o --scatter=scatter_secure.scat --entry=secureStart --keep="startup_secure.o(NORMAL_IMAGE)" -o $(TARGET)
