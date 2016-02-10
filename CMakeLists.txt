cmake_minimum_required(VERSION 3.0)


project(raven)


# define project options
option(BUILD_TESTS OFF)
option(BUILD_DEBUG_MODE ON)


# set build type
if(OPTION_DEBUG_MODE)
	set(CMAKE_BUILD_TYPE DEBUG)
else()
	set(CMAKE_BUILD_TYPE RELEASE)
endif()


# define platform macros
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-DPLATFORM_WIN)
	set(PLATFORM_WIN TRUE)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions(-DPLATFORM_OSX)
	set(PLATFORM_OSX TRUE)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions(-DPLATFORM_LINUX)
	set(PLATFROM_LINUX TRUE)

else()
	message(FATAL_ERROR "Unsupported operating system, CMake will exit.")

endif()


# define include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/source
	${CMAKE_CURRENT_SOURCE_DIR}/source/engine
	#{CMAKE_CURRENT_SOURCE_DIR}/source/raven
)

# TODO: adds all main system source directories
# set(RAVEN_SOURCE ...)

if(NOT BUILD_TESTS)
	# build the game if BUILD_TESTS is OFF

	message(${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp)

	# builds the game
	add_executable(raven ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp) # PLEASE CHANGE THIS to include RAVEN_SOURCE

else()
	# build the tests if BUILD_TESTS is ON

	# build type defaults to debug
	set(CMAKE_BUILD_TYPE DEBUG)

	# TODO: builds the tests

endif()