cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ServerLib)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    add_definitions(-DLinuxOS="Linux")
elseif(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    add_definitions(-DWindowsOS="WindowsOS")
endif()

# Include header files
include_directories(http/headers)

# Find source files
file(GLOB SOURCES http/src/*.cpp)



# Add the executable
#add_executable(server
#    http/src/threadpool.cpp
#    http/src/http_tcpServer.cpp
#    http/src/http_server.cpp
#)



# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(WIN32)
    target_link_libraries(ServerLib ws2_32)
endif()
