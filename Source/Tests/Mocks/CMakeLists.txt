set(NAME Tests.Mocks)

set(SOURCES
  mock.ButtonPressCallback.h
  mock.GpioDriver.h
  mock.InterruptTimer.h
  MockCalls.h
)

set(INCLUDES .)

add_library(${NAME} INTERFACE ${SOURCES})
target_include_directories(${NAME} INTERFACE ${INCLUDES})
target_link_libraries(${NAME} INTERFACE
  IDrivers)
