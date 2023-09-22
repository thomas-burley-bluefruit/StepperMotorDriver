set(NAME Terminal)

set(SOURCES
  ITerminalReceiveCharacterCallback.h
  TerminalIn.cpp
  TerminalIn.h
  TerminalOut.cpp
  TerminalOut.h
)

set(INCLUDES . Commands)

add_library(${NAME} STATIC ${SOURCES})

target_include_directories(${NAME} PUBLIC ${INCLUDES})
target_link_libraries(${NAME} PUBLIC
  Command
  IDrivers
)
