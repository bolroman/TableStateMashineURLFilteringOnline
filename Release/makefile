################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include States/subdir.mk
-include StateMachine/subdir.mk
-include Machines/Release/States/subdir.mk
-include Machines/Release/StateMachine/subdir.mk
-include Machines/Release/DumpReader/BoundedBuffer/subdir.mk
-include Machines/Release/DumpReader/subdir.mk
-include Machines/Release/Dedup/subdir.mk
-include Machines/Release/Common/HashMap/subdir.mk
-include Machines/Release/Common/BitArray/subdir.mk
-include Machines/Release/Common/subdir.mk
-include Machines/Release/AhoCorasick/subdir.mk
-include Machines/Release/subdir.mk
-include DumpReader/BoundedBuffer/subdir.mk
-include DumpReader/subdir.mk
-include Dedup/subdir.mk
-include Common/HashMap/subdir.mk
-include Common/BitArray/subdir.mk
-include Common/subdir.mk
-include AhoCorasick/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: TableStateMashineURLFilteringOnline

# Tool invocations
TableStateMashineURLFilteringOnline: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/usr/local/lib -o "TableStateMashineURLFilteringOnline" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) TableStateMashineURLFilteringOnline
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
