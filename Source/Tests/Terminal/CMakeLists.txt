set(NAME Tests.Terminal)

set(SOURCES
  tests.TerminalIn.cpp
  tests.TerminalOut.cpp
)

set(INCLUDES .)

add_executable(${NAME} ${SOURCES})
target_include_directories(${NAME} PRIVATE ${INCLUDES})
target_link_libraries(${NAME} PRIVATE
  gmock_main
  Command
  Terminal
  Tests.Mocks
)

add_test(${NAME} ${NAME})
