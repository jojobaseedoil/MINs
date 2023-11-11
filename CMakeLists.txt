cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME "MINs")
project(${PROJECT_NAME})

add_executable(
	${PROJECT_NAME}
	Source/Main.cpp
	Source/Network.cpp
)
