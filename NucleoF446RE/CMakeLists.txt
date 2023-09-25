set(NAME STM32F4)

set(SOURCES
  Application/Application.cpp
  Application/Application.h
  Application/Drivers/GpioDriver.cpp
  Application/Drivers/GpioDriver.h
  Application/Drivers/UartDriver.cpp
  Application/Drivers/UartDriver.h
  Application/Drivers/InterruptTimer.cpp
  Application/Drivers/InterruptTimer.h
  Application/Initialisation.cpp
  Application/Initialisation.h
)

set(INCLUDES
  Application
  Application/Drivers
)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS "Core/*.c")
find_directories_recursive(CORE_INCLUDES "Core/*.h")

file(GLOB_RECURSE DRIVERS_SOURCES CONFIGURE_DEPENDS "Drivers/*.c")
find_directories_recursive(DRIVERS_INCLUDES "Drivers/*.h")

add_library(${NAME} STATIC ${SOURCES} ${CORE_SOURCES} ${DRIVERS_SOURCES})

target_compile_options(${NAME} PRIVATE ${COMPILE_OPTIONS})
target_compile_definitions(${NAME} PRIVATE ${COMPILE_DEFINITIONS})
target_include_directories(${NAME} PUBLIC ${INCLUDES} ${CORE_INCLUDES} ${DRIVERS_INCLUDES})
target_link_libraries(${NAME}
  Command
  IDrivers
  Motor
  Terminal
  UserButton
)
