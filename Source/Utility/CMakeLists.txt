set(NAME Utility)

set(SOURCES
  DataQueue.cpp
  DataQueue.h
)

set(INCLUDES .)

add_library(${NAME} STATIC ${SOURCES})

target_include_directories(${NAME} PUBLIC ${INCLUDES})
target_link_libraries(${NAME} PUBLIC
)
