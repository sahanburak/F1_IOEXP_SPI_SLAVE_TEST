################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/SH1107/sh1107.c \
../Core/Src/SH1107/sh1107_fonts.c \
../Core/Src/SH1107/sh1107_test.c 

OBJS += \
./Core/Src/SH1107/sh1107.o \
./Core/Src/SH1107/sh1107_fonts.o \
./Core/Src/SH1107/sh1107_test.o 

C_DEPS += \
./Core/Src/SH1107/sh1107.d \
./Core/Src/SH1107/sh1107_fonts.d \
./Core/Src/SH1107/sh1107_test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/SH1107/%.o: ../Core/Src/SH1107/%.c Core/Src/SH1107/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

