################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include Utilities/STM32F0308-Discovery/subdir.mk
-include HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: My_STM32_ILI9341_HWSPI.elf

# Tool invocations
My_STM32_ILI9341_HWSPI.elf: $(OBJS) $(USER_OBJS) D:\~Documents\School\ECE\ 477\ECE477_Team9\My_STM32_ILI9341_HWSPI/LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -T"D:/~Documents/School/ECE 477/ECE477_Team9/My_STM32_ILI9341_HWSPI/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "My_STM32_ILI9341_HWSPI.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "My_STM32_ILI9341_HWSPI.elf" "My_STM32_ILI9341_HWSPI.bin"
	arm-none-eabi-size "My_STM32_ILI9341_HWSPI.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
