# CMakeLists.txt

# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(magrathean-universe)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Source files
set(SOURCES
	    src/main.cpp
    )

# External libraries
set(LIBS
	GL
	glut
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link external libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
