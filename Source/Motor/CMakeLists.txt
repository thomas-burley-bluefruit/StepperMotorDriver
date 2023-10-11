set(NAME Motor)
enable_language(C CXX)

set(SOURCES
  FullStepSequence.h
  IStepper.h
  IStepperDriver.h
  L298n.cpp
  L298n.h
  SpeedRamping.cpp
  SpeedRamping.h
  Stepper.cpp
  Stepper.h
  StepperDriver.cpp
  StepperDriver.h
  StepperMove.cpp
  StepperMove.h
  StepperRun.cpp
  StepperRun.h
  StepperUtility.cpp
  StepperUtility.h
  UserControl.cpp
  UserControl.h
)

set(INCLUDES .)

add_library(${NAME} STATIC ${SOURCES})

target_compile_options(${NAME} PRIVATE ${COMPILE_OPTIONS})
target_compile_definitions(${NAME} PRIVATE ${COMPILE_DEFINITIONS})
target_include_directories(${NAME} PUBLIC ${INCLUDES})
target_link_libraries(${NAME} PUBLIC
  IDrivers
  UserButton
  Command)