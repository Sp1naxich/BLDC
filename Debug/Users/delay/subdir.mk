################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Users/delay/delay.c 

OBJS += \
./Users/delay/delay.o 

C_DEPS += \
./Users/delay/delay.d 


# Each subdirectory must supply rules for building sources it contributes
Users/delay/%.o Users/delay/%.su Users/delay/%.cyclo: ../Users/delay/%.c Users/delay/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I"C:/SysGCC/arm-eabi/lib/gcc/arm-none-eabi/14.2.1/include" -I"C:/SysGCC/arm-eabi/lib/gcc/arm-none-eabi/14.2.1/include-fixed" -I../Core/Inc -I"C:/SysGCC/arm-eabi/arm-none-eabi/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"D:/Working/Project/00_MCU/Project/BLDC/Users" -I"D:/Working/Project/00_MCU/Project/BLDC/Bus/Uart" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/delay" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/MotorControl" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/RS485" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/sys" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/vofa" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/BLDC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Users-2f-delay

clean-Users-2f-delay:
	-$(RM) ./Users/delay/delay.cyclo ./Users/delay/delay.d ./Users/delay/delay.o ./Users/delay/delay.su

.PHONY: clean-Users-2f-delay

