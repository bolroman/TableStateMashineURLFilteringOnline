################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CommonFunctions.c \
../StateMachineTest.c \
../TableStateMachineTest.c \
../acsmx.c \
../main.c \
../main_old.c 

OBJS += \
./CommonFunctions.o \
./StateMachineTest.o \
./TableStateMachineTest.o \
./acsmx.o \
./main.o \
./main_old.o 

C_DEPS += \
./CommonFunctions.d \
./StateMachineTest.d \
./TableStateMachineTest.d \
./acsmx.d \
./main.d \
./main_old.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


