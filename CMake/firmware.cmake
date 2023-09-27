enable_language(C ASM)

set(EXECUTABLE ${PROJECT_NAME}.out)
set(SOURCES
  NucleoF446RE/Core/Inc/stm32f4xx_hal_conf.h
  NucleoF446RE/Core/Src/system_stm32f4xx.c
  NucleoF446RE/Core/Startup/startup_stm32f446retx.s)

add_executable(${EXECUTABLE} ${SOURCES})

set(COMPILE_OPTIONS
  -mcpu=cortex-m4
  -mthumb
  -mfpu=fpv4-sp-d16
  -mfloat-abi=hard
  -fdata-sections
  -ffunction-sections
  -Wall
  $<$<CONFIG:Debug>:-Og>
  -fno-exceptions
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
target_compile_options(${EXECUTABLE} PRIVATE ${COMPILE_OPTIONS})

set(COMPILE_DEFINITIONS
  -DUSE_HAL_DRIVER
  -DSTM32F446xx)
target_compile_definitions(${EXECUTABLE} PRIVATE ${COMPILE_DEFINITIONS})

set(LINK_OPTIONS
  -T${CMAKE_SOURCE_DIR}/NucleoF446RE/STM32F446RETX_FLASH.ld
  -mcpu=cortex-m4
  -mthumb
  -mfpu=fpv4-sp-d16
  -mfloat-abi=hard
  -specs=nosys.specs
  -lc
  -lm
  -lnosys
  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -Wl,--gc-sections
  -uInterruptTimer10Khz_PeriodElapsed
  -uInterruptTimer1Khz_PeriodElapsed
  -uHAL_TIM_PeriodElapsedCallback
)
target_link_options(${EXECUTABLE} PRIVATE ${LINK_OPTIONS})

set_target_properties(${EXECUTABLE} PROPERTIES ADDITIONAL_CLEAN_FILES
  "${PROJECT_NAME}.bin;${PROJECT_NAME}.hex;${PROJECT_NAME}.map")

target_link_libraries(${EXECUTABLE} PUBLIC
  STM32F4)

add_subdirectory(NucleoF446RE)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
  COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)