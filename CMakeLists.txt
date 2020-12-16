cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(RandomBooleanNetwork VERSION 0.1)

# configure header to pass version number
#configure_file(src/main.cpp src/main.cpp)

# add the executable
add_executable(RandomBooleanNetwork src/main.cpp src/network.cpp)

# add src directory to list of paths to search for include
target_include_directories(RandomBooleanNetwork PUBLIC
														"${PROJECT_BINARY_DIR}"/src
														)

# specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)