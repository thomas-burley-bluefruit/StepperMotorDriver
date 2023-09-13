set(NAME Tests.Drivers)

set(SOURCES
  tests.UserButton.cpp)

set(INCLUDES .)

add_executable(${NAME} ${SOURCES})
target_include_directories(${NAME} PRIVATE ${INCLUDES})
target_link_libraries(${NAME} PUBLIC
  UserButton
  Tests.Mocks
  gmock_main)

include(GoogleTest)
gtest_discover_tests(${NAME})
