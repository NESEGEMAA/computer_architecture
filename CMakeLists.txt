cmake_minimum_required(VERSION 3.10)

# Project name
project(computer_architecture C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find all source files in the src directory
file(GLOB SOURCES "src/*.c")

# Create executable from all source files
add_executable(${PROJECT_NAME} ${SOURCES})