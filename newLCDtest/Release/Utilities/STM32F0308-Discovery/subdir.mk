################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/STM32F0308-Discovery/stm32f0308_discovery.c 

OBJS += \
./Utilities/STM32F0308-Discovery/stm32f0308_discovery.o 

C_DEPS += \
./Utilities/STM32F0308-Discovery/stm32f0308_discovery.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/STM32F0308-Discovery/%.o: ../Utilities/STM32F0308-Discovery/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DSTM32F051x8 -DUSE_HAL_DRIVER -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/HAL_Driver/Inc/Legacy" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/Middlewares/ST/STM32_TouchSensing_Library/inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/Utilities/STM32F0308-Discovery" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/CMSIS/device" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/CMSIS/core" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/HAL_Driver/Inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


