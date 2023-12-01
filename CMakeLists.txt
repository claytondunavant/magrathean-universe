# CMakeLists.txt

# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(magrathean-universe)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Source files
file(GLOB SOURCES "src/*.cpp")

# include directories
include_directories("include")

if (WIN32)
	# set path to GLEW
	set(GLEW_ROOT_PATH "C:/Users/shiss/Downloads/glew-2.1.0-win32/glew-2.1.0")

	# Include GLEW
	include_directories(${GLEW_ROOT_PATH}/include)
	# Specify the GLEW library directory
	link_directories(${GLEW_ROOT_PATH}/lib/Release/x64)

	# Additional library directory
	link_directories(${CMAKE_SOURCE_DIR}/lib/x64)

	# Add the executable
	add_executable(${PROJECT_NAME} ${SOURCES})

	# Specify include directories for the target
	target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)


	# External libraries
	set(LIBS
		opengl32
		freeglut
		glew32
	)

	# Link external libraries
	target_link_libraries(${PROJECT_NAME} ${LIBS})

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${GLEW_ROOT_PATH}/bin/Release/x64/glew32.dll"
                       $<TARGET_FILE_DIR:${PROJECT_NAME}>)

	

elseif (UNIX)
	# External libraries
	set(LIBS
		GL
		glut
		GLEW
	)

	# Add the executable
	add_executable(${PROJECT_NAME} ${SOURCES})

	# Link external libraries
	target_link_libraries(${PROJECT_NAME} ${LIBS})
endif()


