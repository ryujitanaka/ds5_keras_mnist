################################################################################
# 自動生成ファイルです。 編集しないでください!
################################################################################

-include ../makefile.init

RM := clean

# ビルドに関係するソースがすべて、ここに定義されています
-include sources.mk
-include ARM/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# これらのツール呼び出しからの入力および出力をビルド変数へ追加します

# すべてのターゲット
all: NEON-RZ_A1H_MNIST.axf

# ツール呼び出し
NEON-RZ_A1H_MNIST.axf: $(OBJS) ../scatter.scat $(USER_OBJS) $(LIBS)
	@echo 'ビルド・ターゲット: $@'
	@echo '呼び出し中: ARM Linker 5'
	armlink --cpu=Cortex-A9 --entry=__Vectors --scatter="../scatter.scat" --callgraph --map --info=sizes --info=totals --info=unused --info veneers --library_type=microlib --strict --summary_stderr --info summarysizes --xref --symbols --list NEON.map -o "NEON-RZ_A1H_MNIST.axf" $(OBJS) $(USER_OBJS) "C:/Users/ryutan01/Documents/DS-5 Workshop NEON/CMSIS_RTOS_RTX/RTOS/RTX/LIB/ARM/RTX_CA9_L.lib"
	@echo 'ビルド完了ターゲット: $@'
	@echo ' '

# その他のターゲット
clean:
	-$(RM) $(C_UPPER_DEPS) $(S_DEPS) $(C_DEPS) $(CC_DEPS) $(SX_DEPS) $(ASM_UPPER_DEPS) $(C++_DEPS) $(EXECUTABLES) $(CXX_DEPS) $(OBJS) $(SX_UPPER_DEPS) $(ASM_DEPS) $(S_UPPER_DEPS) $(CPP_DEPS) NEON-RZ_A1H_MNIST.axf 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
