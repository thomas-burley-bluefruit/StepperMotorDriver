set(NAME IDrivers)

set(SOURCES
  GpioPin.h
  IButtonPressCallback.h
  ITimerInterruptReceiver.h
  IGpioDriver.h
  IInterruptTimer1Khz.h
  IInterruptTimer10Khz.h
  IDualChannelMotorDriver.h
  IUartDriver.h
)

set(INCLUDES .)

add_library(${NAME} INTERFACE ${SOURCES})
target_include_directories(${NAME} INTERFACE ${INCLUDES})
