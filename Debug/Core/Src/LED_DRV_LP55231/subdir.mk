################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LED_DRV_LP55231/led_drv_lp55231.c 

OBJS += \
./Core/Src/LED_DRV_LP55231/led_drv_lp55231.o 

C_DEPS += \
./Core/Src/LED_DRV_LP55231/led_drv_lp55231.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LED_DRV_LP55231/%.o: ../Core/Src/LED_DRV_LP55231/%.c Core/Src/LED_DRV_LP55231/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

