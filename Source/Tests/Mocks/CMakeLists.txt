set(NAME Tests.Mocks)

set(SOURCES
  mock.ButtonPressCallback.h
  mock.CommandHandler.h
  mock.CommandRegistry.h
  mock.CommandInterpreter.h
  mock.DualChannelMotorDriver.h
  mock.GpioDriver.h
  mock.InterruptTimer1Khz.h
  mock.InterruptTimer10Khz.h
  mock.Terminal.h
  mock.TerminalOut.h
  mock.UartDriver.h
  MockCalls.h
)

set(INCLUDES .)

add_library(${NAME} INTERFACE ${SOURCES})
target_include_directories(${NAME} INTERFACE ${INCLUDES})
target_link_libraries(${NAME} INTERFACE
  IDrivers)
