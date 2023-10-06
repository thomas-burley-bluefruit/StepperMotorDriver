set(NAME Tests.Mocks)

set(SOURCES
  Command/mock.CommandData.h
  Command/mock.CommandHandler.h
  Command/mock.CommandInterpreter.h
  Command/mock.CommandRegistry.h
  Drivers/mock.DualChannelMotorDriver.h
  Drivers/mock.GpioDriver.h
  Drivers/mock.InterruptTimer10Khz.h
  Drivers/mock.InterruptTimer1Khz.h
  Drivers/mock.UartDriver.h
  MockCalls.h
  Motor/mock.Stepper.h
  Motor/mock.StepperDriver.h
  Terminal/mock.Terminal.h
  Terminal/mock.TerminalOut.h
  UserInput/mock.ButtonPressCallback.h
  UserInput/mock.UserButton.h
)

set(INCLUDES .
  Command
  Drivers
  Motor
  Terminal
  UserInput)

add_library(${NAME} INTERFACE ${SOURCES})
target_include_directories(${NAME} INTERFACE ${INCLUDES})
target_link_libraries(${NAME} INTERFACE
  IDrivers)
