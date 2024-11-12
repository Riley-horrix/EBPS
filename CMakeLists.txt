# Setup Project
cmake_minimum_required(VERSION "3.28")
project(ebps LANGUAGES CXX VERSION 0.1)

message("Starting build...")

# Set standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Set variables
set(LIBRARY ebps_lib)
set(TEST_EXE test)

# Include headers
include_directories(./include)

# Include cmake files
include(./CMakeSources.cmake)

# Build source library
add_library(${LIBRARY} SHARED ${SRC_FILES})

# Add Testing Libraries

find_package(Catch2)
if(TARGET Catch2::Catch2)
	message("Found catch2")
else()
  message("Installing Catch2")
  Include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0 # or a later release
  )
  FetchContent_MakeAvailable(Catch2)
endif()

add_executable(${TEST_EXE} ${TEST_SRC_FILES})
target_link_libraries(${TEST_EXE} PRIVATE 
  Catch2::Catch2WithMain
  ${LIBRARY})
