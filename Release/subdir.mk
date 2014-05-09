################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fileop.c \
../handledata.c \
../laplace.c \
../quicksort.c \
../random.c \
../stego.c 

OBJS += \
./fileop.o \
./handledata.o \
./laplace.o \
./quicksort.o \
./random.o \
./stego.o 

C_DEPS += \
./fileop.d \
./handledata.d \
./laplace.d \
./quicksort.d \
./random.d \
./stego.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/opencv -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


