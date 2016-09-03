################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ControlLoop.cpp \
../actuatorhandler.cpp \
../buzzer.cpp \
../comminterfaces.cpp \
../healthwatchdog.cpp \
../murmur.cpp \
../sensorIMU.cpp \
../sensorhousekeeping.cpp \
../spihelper.cpp \
../storagecontroller.cpp \
../telecommand.cpp \
../telemetry.cpp \
../topics.cpp 

OBJS += \
./ControlLoop.o \
./actuatorhandler.o \
./buzzer.o \
./comminterfaces.o \
./healthwatchdog.o \
./murmur.o \
./sensorIMU.o \
./sensorhousekeeping.o \
./spihelper.o \
./storagecontroller.o \
./telecommand.o \
./telemetry.o \
./topics.o 

CPP_DEPS += \
./ControlLoop.d \
./actuatorhandler.d \
./buzzer.d \
./comminterfaces.d \
./healthwatchdog.d \
./murmur.d \
./sensorIMU.d \
./sensorhousekeeping.d \
./spihelper.d \
./storagecontroller.d \
./telecommand.d \
./telemetry.d \
./topics.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DUSE_STM32_DISCOVERY -DSTM32F40_41xxx -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\CMSIS\Include" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\hal" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal-generic" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\independent\gateway" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\independent" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\api" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\sdCard" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\api\hal" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\rodos\default_usr_configs" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\support_libs" -I"C:\Users\Raptoaaah\Desktop\Eclipse Luna SR1 121\Workspace\support_libs\flash\spiFlash_AT45DBxxx" -fabi-version=0 -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


