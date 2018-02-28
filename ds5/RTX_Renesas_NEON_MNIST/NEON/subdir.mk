################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NEON.c \
../RTX_Conf_CM.c \
../RZ_A1H_GENMAI_Init.c \
../barman.c \
../cnn.c \
../gic.c \
../mmu_Renesas_RZ_A1.c \
../pl310.c \
../system_Renesas_RZ_A1.c 

C_DEPS += \
./NEON.d \
./RTX_Conf_CM.d \
./RZ_A1H_GENMAI_Init.d \
./barman.d \
./cnn.d \
./gic.d \
./mmu_Renesas_RZ_A1.d \
./pl310.d \
./system_Renesas_RZ_A1.d 

OBJS += \
./NEON.o \
./RTX_Conf_CM.o \
./RZ_A1H_GENMAI_Init.o \
./barman.o \
./cnn.o \
./gic.o \
./mmu_Renesas_RZ_A1.o \
./pl310.o \
./system_Renesas_RZ_A1.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc --cpu=Cortex-A9 --thumb --apcs=/interwork -D__MICROLIB -D__FPU_PRESENT -I"C:\Users\ryutan01\Documents\DS-5 v0528 Workspace\barman-CMSIS_RTOS_RTX\RTOS\RTX\SRC" -I"C:\Users\ryutan01\Documents\DS-5 v0528 Workspace\barman-CMSIS_RTOS_RTX\RTOS\RTX\INC" -I"C:\Users\ryutan01\Documents\DS-5 v0528 Workspace\barman-CMSIS_RTOS_RTX\Include" -I"C:\Users\ryutan01\Documents\DS-5 v0528 Workspace\barman-CMSIS_RTOS_RTX\RTOS\RTX\Boards\Renesas\RZ_A1H_GENMAI" -I"C:\Users\ryutan01\Documents\DS-5 v0528 Workspace\barman-CMSIS_RTOS_RTX\RTOS\RTX\Boards\Renesas\RZ_A1H_GENMAI\INC" -I"C:/Users/ryutan01/Documents/DS-5 v0528 Workspace/RTX_Renesas_NEON_MNIST" --gnu -O2 -Otime --vectorize -g --diag_warning=optimizations --md --depend_format=unix_escaped --no_depend_system_headers -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


