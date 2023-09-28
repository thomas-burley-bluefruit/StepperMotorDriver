set(NAME Tests.Motor)

set(SOURCES
  tests.L298n.cpp
  tests.Stepper.cpp
  tests.UserControl.cpp)

set(INCLUDES .)

add_executable(${NAME} ${SOURCES})
target_include_directories(${NAME} PRIVATE ${INCLUDES})
target_link_libraries(${NAME} PUBLIC
  Motor
  Tests.Mocks
  gmock_main)
