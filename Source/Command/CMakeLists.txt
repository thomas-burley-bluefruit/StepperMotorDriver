set(NAME Command)

set(SOURCES
  CommandData.cpp
  CommandData.h
  CommandInterpreter.cpp
  CommandInterpreter.h
  CommandRegistry.cpp
  CommandRegistry.h
  ICommandData.h
  ICommandHandler.h
  ICommandInterpreter.h
  ICommandRegistry.h
  Types/CommandKeys.h
  Types/ComponentName.cpp
  Types/ComponentName.h
)

set(INCLUDES . Components Types)

add_library(${NAME} STATIC ${SOURCES})

target_include_directories(${NAME} PUBLIC ${INCLUDES})
target_link_libraries(${NAME} PUBLIC
  Terminal
  Utility
)
