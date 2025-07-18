################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/CLI/CLI.cpp \
../Src/CLI/Manager.cpp 

OBJS += \
./Src/CLI/CLI.o \
./Src/CLI/Manager.o 

CPP_DEPS += \
./Src/CLI/CLI.d \
./Src/CLI/Manager.d 


# Each subdirectory must supply rules for building sources it contributes
Src/CLI/%.o Src/CLI/%.su Src/CLI/%.cyclo: ../Src/CLI/%.cpp Src/CLI/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-CLI

clean-Src-2f-CLI:
	-$(RM) ./Src/CLI/CLI.cyclo ./Src/CLI/CLI.d ./Src/CLI/CLI.o ./Src/CLI/CLI.su ./Src/CLI/Manager.cyclo ./Src/CLI/Manager.d ./Src/CLI/Manager.o ./Src/CLI/Manager.su

.PHONY: clean-Src-2f-CLI

