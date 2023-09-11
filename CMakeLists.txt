cmake_minimum_required(VERSION 3.27.1)

project(StepperMotorDriver)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(FIRMWARE)
  include(CMake/firmware.cmake)
endif()

if(TESTING)
  include(CMake/testing.cmake)
endif()
