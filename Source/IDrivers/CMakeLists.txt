set(NAME IDrivers)

set(SOURCES
  GpioPin.h
  IButtonPressCallback.h
  IExternalInterruptReceiver.h
  IGpioDriver.h
  IInterruptTimer.h
  IDualChannelMotorDriver.h
)

set(INCLUDES .)

add_library(${NAME} INTERFACE ${SOURCES})
target_include_directories(${NAME} INTERFACE ${INCLUDES})
