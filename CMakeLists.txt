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


#add dependencies
add_subdirectory(http/dependencies/threadpool)




option(GEN_HSRV_EXE "Generate an execuatable for testing http server, by default is true" ON)


if(GEN_HSRV_EXE)
 
    # Add the executable
    add_executable(ServerLibTest
        http-server-driver.cpp
        http/src/http_tcpServer.cpp
        http/src/http_server.cpp
    )

    # Link the library from threadpool to the main executable
    target_link_libraries(ServerLibTest threadpool)
    if(WIN32)
        target_link_libraries(ServerLibTest ws2_32)
    endif()

endif()


#generate static library
add_library(${PROJECT_NAME}  ${SOURCES})
if(WIN32)
    target_link_libraries(${PROJECT_NAME}  ws2_32)
endif()
