################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MY_ILI9341.c \
../Core/Src/TSC2046.c \
../Core/Src/flash.c \
../Core/Src/main.c \
../Core/Src/memory.c \
../Core/Src/stm32f0xx_hal_msp.c \
../Core/Src/stm32f0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f0xx.c 

OBJS += \
./Core/Src/MY_ILI9341.o \
./Core/Src/TSC2046.o \
./Core/Src/flash.o \
./Core/Src/main.o \
./Core/Src/memory.o \
./Core/Src/stm32f0xx_hal_msp.o \
./Core/Src/stm32f0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f0xx.o 

C_DEPS += \
./Core/Src/MY_ILI9341.d \
./Core/Src/TSC2046.d \
./Core/Src/flash.d \
./Core/Src/main.d \
./Core/Src/memory.d \
./Core/Src/stm32f0xx_hal_msp.d \
./Core/Src/stm32f0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DUSE_HAL_DRIVER -DSTM32F051x8 -I"D:/~Documents/School/ECE 477/ECE477_Team9/Youtube_ILI9341_Tutorial/Core/Inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/Youtube_ILI9341_Tutorial/Drivers/STM32F0xx_HAL_Driver/Inc" -I"D:/~Documents/School/ECE 477/ECE477_Team9/Youtube_ILI9341_Tutorial/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy" -I"D:/~Documents/School/ECE 477/ECE477_Team9/Youtube_ILI9341_Tutorial/Drivers/CMSIS/Device/ST/STM32F0xx/Include" -I"D:/~Documents/School/ECE 477/ECE477_Team9/Youtube_ILI9341_Tutorial/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


