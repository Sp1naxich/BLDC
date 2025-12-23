################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Users/MotorControl/Back_EMF_Positon_Estimation.c \
../Users/MotorControl/Coordinate_transform.c \
../Users/MotorControl/FOC_Control.c \
../Users/MotorControl/Filter.c \
../Users/MotorControl/HFI.c \
../Users/MotorControl/Motor.c \
../Users/MotorControl/PI_Cale.c \
../Users/MotorControl/SVPWM.c \
../Users/MotorControl/Senserless_SMO.c 

OBJS += \
./Users/MotorControl/Back_EMF_Positon_Estimation.o \
./Users/MotorControl/Coordinate_transform.o \
./Users/MotorControl/FOC_Control.o \
./Users/MotorControl/Filter.o \
./Users/MotorControl/HFI.o \
./Users/MotorControl/Motor.o \
./Users/MotorControl/PI_Cale.o \
./Users/MotorControl/SVPWM.o \
./Users/MotorControl/Senserless_SMO.o 

C_DEPS += \
./Users/MotorControl/Back_EMF_Positon_Estimation.d \
./Users/MotorControl/Coordinate_transform.d \
./Users/MotorControl/FOC_Control.d \
./Users/MotorControl/Filter.d \
./Users/MotorControl/HFI.d \
./Users/MotorControl/Motor.d \
./Users/MotorControl/PI_Cale.d \
./Users/MotorControl/SVPWM.d \
./Users/MotorControl/Senserless_SMO.d 


# Each subdirectory must supply rules for building sources it contributes
Users/MotorControl/%.o Users/MotorControl/%.su Users/MotorControl/%.cyclo: ../Users/MotorControl/%.c Users/MotorControl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I"C:/SysGCC/arm-eabi/lib/gcc/arm-none-eabi/14.2.1/include" -I"C:/SysGCC/arm-eabi/lib/gcc/arm-none-eabi/14.2.1/include-fixed" -I../Core/Inc -I"C:/SysGCC/arm-eabi/arm-none-eabi/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"D:/Working/Project/00_MCU/Project/BLDC/Users" -I"D:/Working/Project/00_MCU/Project/BLDC/Bus/Uart" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/delay" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/MotorControl" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/RS485" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/sys" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/vofa" -I"D:/Working/Project/00_MCU/Project/BLDC/Users/BLDC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Users-2f-MotorControl

clean-Users-2f-MotorControl:
	-$(RM) ./Users/MotorControl/Back_EMF_Positon_Estimation.cyclo ./Users/MotorControl/Back_EMF_Positon_Estimation.d ./Users/MotorControl/Back_EMF_Positon_Estimation.o ./Users/MotorControl/Back_EMF_Positon_Estimation.su ./Users/MotorControl/Coordinate_transform.cyclo ./Users/MotorControl/Coordinate_transform.d ./Users/MotorControl/Coordinate_transform.o ./Users/MotorControl/Coordinate_transform.su ./Users/MotorControl/FOC_Control.cyclo ./Users/MotorControl/FOC_Control.d ./Users/MotorControl/FOC_Control.o ./Users/MotorControl/FOC_Control.su ./Users/MotorControl/Filter.cyclo ./Users/MotorControl/Filter.d ./Users/MotorControl/Filter.o ./Users/MotorControl/Filter.su ./Users/MotorControl/HFI.cyclo ./Users/MotorControl/HFI.d ./Users/MotorControl/HFI.o ./Users/MotorControl/HFI.su ./Users/MotorControl/Motor.cyclo ./Users/MotorControl/Motor.d ./Users/MotorControl/Motor.o ./Users/MotorControl/Motor.su ./Users/MotorControl/PI_Cale.cyclo ./Users/MotorControl/PI_Cale.d ./Users/MotorControl/PI_Cale.o ./Users/MotorControl/PI_Cale.su ./Users/MotorControl/SVPWM.cyclo ./Users/MotorControl/SVPWM.d ./Users/MotorControl/SVPWM.o ./Users/MotorControl/SVPWM.su ./Users/MotorControl/Senserless_SMO.cyclo ./Users/MotorControl/Senserless_SMO.d ./Users/MotorControl/Senserless_SMO.o ./Users/MotorControl/Senserless_SMO.su

.PHONY: clean-Users-2f-MotorControl

