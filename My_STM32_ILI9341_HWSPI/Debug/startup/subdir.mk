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
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"D:/~Documents/School/ECE 477/ECE477_Team9/My_STM32_ILI9341_HWSPI/HAL_Driver/Inc/Legacy" -I"D:/~Documents/School/ECE 477/ECE477_Team9/My_STM32_ILI9341_HWSPI/Utilities/STM32F0308-Discovery" -I"D:/~Documents/School/ECE 477/ECE477_Team9/My_STM32_ILI9341_HWSPI/inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/My_STM32_ILI9341_HWSPI/CMSIS/device" -I"D:/~Documents/School/ECE 477/ECE477_Team9/My_STM32_ILI9341_HWSPI/CMSIS/core" -I"D:/~Documents/School/ECE 477/ECE477_Team9/My_STM32_ILI9341_HWSPI/HAL_Driver/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


