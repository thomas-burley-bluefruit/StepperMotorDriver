set(NAME Motor)
enable_language(C CXX)

set(SOURCES
  FullStepSequence.h
  IStepper.h
  L298n.cpp
  L298n.h
  Stepper.cpp
  Stepper.h)

set(INCLUDES .)

add_library(${NAME} STATIC ${SOURCES})

target_compile_options(${NAME} PRIVATE ${COMPILE_OPTIONS})
target_compile_definitions(${NAME} PRIVATE ${COMPILE_DEFINITIONS})
target_include_directories(${NAME} PUBLIC ${INCLUDES})
target_link_libraries(${NAME} PUBLIC
  IDrivers
  UserButton)