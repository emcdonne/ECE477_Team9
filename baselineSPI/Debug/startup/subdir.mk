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
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"D:/workspace/baselineSPI/HAL_Driver/Inc/Legacy" -I"D:/workspace/baselineSPI/Utilities/STM32F0308-Discovery" -I"D:/workspace/baselineSPI/inc" -I"D:/workspace/baselineSPI/CMSIS/device" -I"D:/workspace/baselineSPI/CMSIS/core" -I"D:/workspace/baselineSPI/HAL_Driver/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


