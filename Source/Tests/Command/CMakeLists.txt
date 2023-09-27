set(NAME Tests.Command)

set(SOURCES
  CommandInterpreterTestObject.h
  CommandTestUtilities.h
  tests.CommandInterpreter.cpp
  tests.CommandInterpreter.Component.cpp
  tests.CommandInterpreter.Parameters.cpp
  tests.CommandRegistry.cpp
)

set(INCLUDES .)

add_executable(${NAME} ${SOURCES})
target_include_directories(${NAME} PRIVATE ${INCLUDES})
target_link_libraries(${NAME} PRIVATE
  gmock_main
  Command
  Tests.Mocks
)
