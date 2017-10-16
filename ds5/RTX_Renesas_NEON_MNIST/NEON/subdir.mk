################################################################################
# 自動生成ファイルです。 編集しないでください!
################################################################################

# これらのツール呼び出しからの入力および出力をビルド変数へ追加します
C_SRCS += \
../NEON.c \
../RTX_Conf_CM.c \
../RZ_A1H_GENMAI_Init.c \
../cnn.c \
../gic.c \
../mmu_Renesas_RZ_A1.c \
../pl310.c \
../system_Renesas_RZ_A1.c 

C_DEPS += \
./NEON.d \
./RTX_Conf_CM.d \
./RZ_A1H_GENMAI_Init.d \
./cnn.d \
./gic.d \
./mmu_Renesas_RZ_A1.d \
./pl310.d \
./system_Renesas_RZ_A1.d 

OBJS += \
./NEON.o \
./RTX_Conf_CM.o \
./RZ_A1H_GENMAI_Init.o \
./cnn.o \
./gic.o \
./mmu_Renesas_RZ_A1.o \
./pl310.o \
./system_Renesas_RZ_A1.o 


# サブディレクトリーはすべて、それ自身がコントリビュートするソースをビルドするためのルールを提供しなければなりません
%.o: ../%.c
	@echo 'ビルドするファイル: $<'
	@echo '呼び出し中: ARM C Compiler 5'
	armcc --cpu=Cortex-A9 --thumb --apcs=/interwork -D__MICROLIB -D__FPU_PRESENT -I"C:\Users\ryutan01\Documents\DS-5 Workshop NEON\CMSIS_RTOS_RTX\RTOS\RTX\SRC" -I"C:\Users\ryutan01\Documents\DS-5 Workshop NEON\CMSIS_RTOS_RTX\RTOS\RTX\INC" -I"C:\Users\ryutan01\Documents\DS-5 Workshop NEON\CMSIS_RTOS_RTX\Include" -I"C:\Users\ryutan01\Documents\DS-5 Workshop NEON\CMSIS_RTOS_RTX\RTOS\RTX\Boards\Renesas\RZ_A1H_GENMAI" -I"C:\Users\ryutan01\Documents\DS-5 Workshop NEON\CMSIS_RTOS_RTX\RTOS\RTX\Boards\Renesas\RZ_A1H_GENMAI\INC" -I"C:/Users/ryutan01/Documents/DS-5 Workshop NEON/RTX_Renesas_NEON_MNIST" -O2 -Otime --vectorize -g --diag_warning=optimizations --md --depend_format=unix_escaped --no_depend_system_headers -c -o "$@" "$<"
	@echo 'ビルド完了: $<'
	@echo ' '


