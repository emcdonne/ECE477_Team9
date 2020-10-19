################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ILI9341_Driver.c \
../src/display.c \
../src/main.c \
../src/stm32f0xx_it.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

OBJS += \
./src/ILI9341_Driver.o \
./src/display.o \
./src/main.o \
./src/stm32f0xx_it.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

C_DEPS += \
./src/ILI9341_Driver.d \
./src/display.d \
./src/main.d \
./src/stm32f0xx_it.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051x8 -DUSE_HAL_DRIVER -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/HAL_Driver/Inc/Legacy" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/Middlewares/ST/STM32_TouchSensing_Library/inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/Utilities/STM32F0308-Discovery" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/CMSIS/device" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/CMSIS/core" -I"D:/~Documents/School/ECE 477/ECE477_Team9/newLCDtest/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


