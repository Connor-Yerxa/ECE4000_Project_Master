################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GPS.c \
../Core/Src/MAX_Commands.c \
../Core/Src/Menus.c \
../Core/Src/SD_Commands.c \
../Core/Src/displayText.c \
../Core/Src/font12.c \
../Core/Src/font16.c \
../Core/Src/font20.c \
../Core/Src/font24.c \
../Core/Src/font8.c \
../Core/Src/main.c \
../Core/Src/max31856_stm32.c \
../Core/Src/max31865_stm32.c \
../Core/Src/read_buttons.c \
../Core/Src/runCalibration.c \
../Core/Src/runTest.c \
../Core/Src/sd_benchmark.c \
../Core/Src/sd_diskio_spi.c \
../Core/Src/sd_functions.c \
../Core/Src/sd_spi.c \
../Core/Src/selectNumber.c \
../Core/Src/showGraph.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/z_displ_ILI9XXX.c \
../Core/Src/z_displ_ILI9XXX_test.c 

OBJS += \
./Core/Src/GPS.o \
./Core/Src/MAX_Commands.o \
./Core/Src/Menus.o \
./Core/Src/SD_Commands.o \
./Core/Src/displayText.o \
./Core/Src/font12.o \
./Core/Src/font16.o \
./Core/Src/font20.o \
./Core/Src/font24.o \
./Core/Src/font8.o \
./Core/Src/main.o \
./Core/Src/max31856_stm32.o \
./Core/Src/max31865_stm32.o \
./Core/Src/read_buttons.o \
./Core/Src/runCalibration.o \
./Core/Src/runTest.o \
./Core/Src/sd_benchmark.o \
./Core/Src/sd_diskio_spi.o \
./Core/Src/sd_functions.o \
./Core/Src/sd_spi.o \
./Core/Src/selectNumber.o \
./Core/Src/showGraph.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/z_displ_ILI9XXX.o \
./Core/Src/z_displ_ILI9XXX_test.o 

C_DEPS += \
./Core/Src/GPS.d \
./Core/Src/MAX_Commands.d \
./Core/Src/Menus.d \
./Core/Src/SD_Commands.d \
./Core/Src/displayText.d \
./Core/Src/font12.d \
./Core/Src/font16.d \
./Core/Src/font20.d \
./Core/Src/font24.d \
./Core/Src/font8.d \
./Core/Src/main.d \
./Core/Src/max31856_stm32.d \
./Core/Src/max31865_stm32.d \
./Core/Src/read_buttons.d \
./Core/Src/runCalibration.d \
./Core/Src/runTest.d \
./Core/Src/sd_benchmark.d \
./Core/Src/sd_diskio_spi.d \
./Core/Src/sd_functions.d \
./Core/Src/sd_spi.d \
./Core/Src/selectNumber.d \
./Core/Src/showGraph.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/z_displ_ILI9XXX.d \
./Core/Src/z_displ_ILI9XXX_test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/GPS.cyclo ./Core/Src/GPS.d ./Core/Src/GPS.o ./Core/Src/GPS.su ./Core/Src/MAX_Commands.cyclo ./Core/Src/MAX_Commands.d ./Core/Src/MAX_Commands.o ./Core/Src/MAX_Commands.su ./Core/Src/Menus.cyclo ./Core/Src/Menus.d ./Core/Src/Menus.o ./Core/Src/Menus.su ./Core/Src/SD_Commands.cyclo ./Core/Src/SD_Commands.d ./Core/Src/SD_Commands.o ./Core/Src/SD_Commands.su ./Core/Src/displayText.cyclo ./Core/Src/displayText.d ./Core/Src/displayText.o ./Core/Src/displayText.su ./Core/Src/font12.cyclo ./Core/Src/font12.d ./Core/Src/font12.o ./Core/Src/font12.su ./Core/Src/font16.cyclo ./Core/Src/font16.d ./Core/Src/font16.o ./Core/Src/font16.su ./Core/Src/font20.cyclo ./Core/Src/font20.d ./Core/Src/font20.o ./Core/Src/font20.su ./Core/Src/font24.cyclo ./Core/Src/font24.d ./Core/Src/font24.o ./Core/Src/font24.su ./Core/Src/font8.cyclo ./Core/Src/font8.d ./Core/Src/font8.o ./Core/Src/font8.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/max31856_stm32.cyclo ./Core/Src/max31856_stm32.d ./Core/Src/max31856_stm32.o ./Core/Src/max31856_stm32.su ./Core/Src/max31865_stm32.cyclo ./Core/Src/max31865_stm32.d ./Core/Src/max31865_stm32.o ./Core/Src/max31865_stm32.su ./Core/Src/read_buttons.cyclo ./Core/Src/read_buttons.d ./Core/Src/read_buttons.o ./Core/Src/read_buttons.su ./Core/Src/runCalibration.cyclo ./Core/Src/runCalibration.d ./Core/Src/runCalibration.o ./Core/Src/runCalibration.su ./Core/Src/runTest.cyclo ./Core/Src/runTest.d ./Core/Src/runTest.o ./Core/Src/runTest.su ./Core/Src/sd_benchmark.cyclo ./Core/Src/sd_benchmark.d ./Core/Src/sd_benchmark.o ./Core/Src/sd_benchmark.su ./Core/Src/sd_diskio_spi.cyclo ./Core/Src/sd_diskio_spi.d ./Core/Src/sd_diskio_spi.o ./Core/Src/sd_diskio_spi.su ./Core/Src/sd_functions.cyclo ./Core/Src/sd_functions.d ./Core/Src/sd_functions.o ./Core/Src/sd_functions.su ./Core/Src/sd_spi.cyclo ./Core/Src/sd_spi.d ./Core/Src/sd_spi.o ./Core/Src/sd_spi.su ./Core/Src/selectNumber.cyclo ./Core/Src/selectNumber.d ./Core/Src/selectNumber.o ./Core/Src/selectNumber.su ./Core/Src/showGraph.cyclo ./Core/Src/showGraph.d ./Core/Src/showGraph.o ./Core/Src/showGraph.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/z_displ_ILI9XXX.cyclo ./Core/Src/z_displ_ILI9XXX.d ./Core/Src/z_displ_ILI9XXX.o ./Core/Src/z_displ_ILI9XXX.su ./Core/Src/z_displ_ILI9XXX_test.cyclo ./Core/Src/z_displ_ILI9XXX_test.d ./Core/Src/z_displ_ILI9XXX_test.o ./Core/Src/z_displ_ILI9XXX_test.su

.PHONY: clean-Core-2f-Src

