################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/frizz++.cpp \
../src/lexer.cpp \
../src/md_preprocessor.cpp 

OBJS += \
./src/frizz++.o \
./src/lexer.o \
./src/md_preprocessor.o 

CPP_DEPS += \
./src/frizz++.d \
./src/lexer.d \
./src/md_preprocessor.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++17 -I/home/dmmettlach/eclipse-workspace/frizz++/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


