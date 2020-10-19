################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f051x8.s 

OBJS += \
./startup/startup_stm32f051x8.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/HAL_Driver/Inc/Legacy" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/Middlewares/ST/STM32_TouchSensing_Library/inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/Utilities/STM32F0308-Discovery" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/CMSIS/device" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/CMSIS/core" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/HAL_Driver/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


