cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME "MINs")
project(${PROJECT_NAME})

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Find JSONcpp package
find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp)

# Add the source files to the executable
add_executable(
    ${PROJECT_NAME}
    Source/Main.cpp
    # Components
    Source/Components/Graph.cpp
    Source/Components/Network.cpp
    Source/Components/Architecture.cpp
    # heuristics
    Source/Heuristics/SimulatedAnnealing.cpp
    # utils
    Source/Utils/Random.cpp
)

# Add OpenMP compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_CXX_FLAGS})

# Link against OpenMP library
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_CXX_LIBRARIES})

# Link against JSONcpp library
target_link_libraries(${PROJECT_NAME} PRIVATE ${JSONCPP_LIBRARIES})