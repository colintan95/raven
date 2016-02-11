cmake_minimum_required(VERSION 3.0)


project(raven)

# macro to clear source files that were added
macro(clear_added_sources)
	set (ADDED_SRCS)
endmacro()

# define macro to add source files to RAVEN_SOURCE
# must be called in ALL CMakeLists.txt AFTER CMakeLists.txt
macro (add_sources)
    file (RELATIVE_PATH _path "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_path)
            list (APPEND ADDED_SRCS "${_path}/${_src}")
        else()
            list (APPEND ADDED_SRCS "${_src}")
        endif()
    endforeach()
    if (_path)
        # propagate SRCS to parent directory
       set (ADDED_SRCS ${ADDED_SRCS} PARENT_SCOPE)
    endif()
endmacro()


# define project options
option(BUILD_GAME "build game" ON)
option(BUILD_UNIT_TESTS "build unit tests" ON)
option(BUILD_REGR_TESTS "build regression tests" OFF)
option(BUILD_INTR_TESTS "build integration tests" OFF)
option(BUILD_DEBUG_MODE "enable debug mode" ON)


# set build type
if(BUILD_DEBUG_MODE)
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
	${CMAKE_CURRENT_SOURCE_DIR}/source/raven
)

# add source subdirectory
add_subdirectory(source)

# add source files from add_sources() macro
set(RAVEN_SOURCE ${ADDED_SRCS})


# build the game if enabled
if(BUILD_GAME)

	# output executable to build folder
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/release)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/release)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/debug)

	add_executable(raven source/main.cpp ${RAVEN_SOURCE})

endif()


# output executable to build folder for ALL tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/tests)

# build unit tests if enabled
if(BUILD_UNIT_TESTS)

	# build type defaults to debug
	set(CMAKE_BUILD_TYPE DEBUG)

	include_directories(tests/unit)

	# clear any source files from previous add_source() calls
	clear_added_sources()

	add_subdirectory(tests/unit)

	set(RAVEN_UNIT_SOURCE ${RAVEN_SOURCE} ${ADDED_SRCS})

	add_executable(unit_test tests/unit/main.cpp ${RAVEN_UNIT_SOURCE})

endif()


# build regression tests if enabled
if(BUILD_REGR_TESTS)

	# build type defaults to debug
	set(CMAKE_BUILD_TYPE DEBUG)

	include_directories(tests/regression)

	# clear any source files from previous add_source() calls
	clear_added_sources()

	add_subdirectory(tests/regression)
	
	set(RAVEN_REGR_SOURCE ${RAVEN_SOURCE} ${ADDED_SRCS})

	add_executable(regr_test tests/regression/main.cpp ${RAVEN_REGR_SOURCE})

endif()


# build integration tests if enabled
if(BUILD_INTR_TESTS)

	# build type defaults to debug
	set(CMAKE_BUILD_TYPE DEBUG)

	include_directories(tests/integration)

	# clear any source files from previous add_source() calls
	clear_added_sources()

	add_subdirectory(tests/integration)

	set(RAVEN_INTR_SOURCE ${RAVEN_SOURCE} ${ADDED_SRCS})

	add_executable(intr_test tests/integration/main.cpp ${RAVEN_INTR_SOURCE})

endif()