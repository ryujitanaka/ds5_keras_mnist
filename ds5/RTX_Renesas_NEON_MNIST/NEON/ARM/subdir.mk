################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../ARM/startup_Renesas_RZ_A1.s 

S_DEPS += \
./ARM/startup_Renesas_RZ_A1.d 

OBJS += \
./ARM/startup_Renesas_RZ_A1.o 


# Each subdirectory must supply rules for building sources it contributes
ARM/startup_Renesas_RZ_A1.o: ../ARM/startup_Renesas_RZ_A1.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Assembler 5'
	armasm --cpu=Cortex-A9 --littleend --apcs=/interwork -g --diag_suppress=1786 --pd "__MICROLIB SETA 1" --pd "__FPU_PRESENT SETA 1" --md --depend_format=unix_escaped --depend="ARM/startup_Renesas_RZ_A1.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


