cmake_minimum_required(VERSION 3.0)


project(raven)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)


# macro to clear source files that were added
macro(clear_added_sources)
	set (ADDED_SRCS)
endmacro()

# define macro to add source files to RAVEN_SOURCE
# must be called in ALL CMakeLists.txt AFTER CMakeLists.txt
macro(add_sources)
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


# define platform macros
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-DEXT_PLATFORM_WIN)
	set(PLATFORM_WIN TRUE)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions(-DEXT_PLATFORM_OSX)
	set(PLATFORM_OSX TRUE)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions(-DEXT_PLATFORM_LINUX)
	set(PLATFORM_LINUX TRUE)

else()
	message(FATAL_ERROR "Unsupported operating system, CMake will exit.")

endif()


# use OpenGL as graphics framework
add_definitions(-DEXT_PLATFORM_OPENGL)


# set build type
if(BUILD_DEBUG_MODE)
	set(CMAKE_BUILD_TYPE DEBUG)
	add_definitions(-DEXT_DEBUG_MODE)

	# set verbosity to 1 (highest)
	add_definitions(-DEXT_LOG_VERBOSE=1)
else()
	set(CMAKE_BUILD_TYPE RELEASE)
endif()


# set C++ compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}")


# add all external packages

INCLUDE(FindPkgConfig)

# find Boost
find_package(Boost)

if(NOT Boost_FOUND) 
	message(FATAL_ERROR "Boost not found, CMake will exit.")
endif()

set(RAVEN_LIB_INCLUDE ${RAVEN_LIB_INCLUDE} ${Boost_INCLUDE_DIRS})
set(RAVEN_LIB_LIBRARIES ${RAVEN_LIB_LIBRARIES} ${Boost_LIBRARIES})

# find ZLIB
find_package(ZLIB)

if(NOT ZLIB_FOUND)
	message(FATAL_ERROR "zlib not found, CMake will exit.")
endif()

set(RAVEN_LIB_INCLUDE ${RAVEN_LIB_INCLUDE} ${ZLIB_INCLUDE_DIRS})
set(RAVEN_LIB_LIBRARIES ${RAVEN_LIB_LIBRARIES} ${ZLIB_LIBRARIES})

# find LibPNG
find_package(PNG)

if(NOT PNG_FOUND)
	message(FATAL_ERROR "LibPNG not found, CMake will exit.")
endif()

set(RAVEN_LIB_INCLUDE ${RAVEN_LIB_INCLUDE} ${PNG_INCLUDE_DIRS})
set(RAVEN_LIB_LIBRARIES ${RAVEN_LIB_LIBRARIES} ${PNG_LIBRARIES})

# find SDL2
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

if(NOT SDL2_FOUND)
	message(FATAL_ERROR "SDL2 not found, CMake will exit.")
endif()

set(RAVEN_LIB_INCLUDE ${RAVEN_LIB_INCLUDE} ${SDL2_INCLUDE_DIRS})
set(RAVEN_LIB_LIBRARIES ${RAVEN_LIB_LIBRARIES} "-L${SDL2_LIBRARY_DIRS} -lSDL2")

# find OpenGL
find_package(OpenGL)

if (NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found, CMake will exit.")
endif()

set(RAVEN_LIB_INCLUDE ${RAVEN_LIB_INCLUDE} ${OPENGL_INCLUDE_DIRS})
set(RAVEN_LIB_LIBRARIES ${RAVEN_LIB_LIBRARIES} ${OPENGL_LIBRARIES})

# find googletest
find_package(GTEST)

if(NOT GTEST_FOUND)
	message(FATAL_ERROR "googletest not found, CMake will exit.")
endif()

set(RAVEN_TEST_INCLUDE ${RAVEN_TEST_INCLUDE} ${GTEST_INCLUDE_DIRS})
set(RAVEN_TEST_LIBRARIES ${RAVEN_TEST_LIBRARIES} ${GTEST_LIBRARIES})

# find googlemock
find_package(GMOCK)

if(NOT GMOCK_FOUND)
	message(FATAL_ERROR "googlemock not found, CMake will exit.")
endif()

set(RAVEN_TEST_INCLUDE ${RAVEN_TEST_INCLUDE} ${GMOCK_INCLUDE_DIRS})
set(RAVEN_TEST_LIBRARIES ${RAVEN_TEST_LIBRARIES} ${GMOCK_LIBRARIES})



# add external include directories
include_directories(${RAVEN_LIB_INCLUDE})


# define include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/source/engine/base
	${CMAKE_CURRENT_SOURCE_DIR}/source/engine/platform
	${CMAKE_CURRENT_SOURCE_DIR}/source/engine
	${CMAKE_CURRENT_SOURCE_DIR}/source/raven
	${CMAKE_CURRENT_SOURCE_DIR}/source
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

	# link external libraries
	target_link_libraries(raven ${RAVEN_LIB_LIBRARIES})

endif()


# output executable to build folder for ALL tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/tests)

# build unit tests if enabled
if(BUILD_UNIT_TESTS)

	# build type defaults to debug
	set(CMAKE_BUILD_TYPE DEBUG)

	include_directories(tests/unit)
	include_directories(${RAVEN_TEST_INCLUDE})

	# clear any source files from previous add_source() calls
	clear_added_sources()

	add_subdirectory(tests/unit)

	set(RAVEN_UNIT_SOURCE ${RAVEN_SOURCE} ${ADDED_SRCS})

	add_executable(unit_test tests/unit/main.cpp ${RAVEN_UNIT_SOURCE})

	# link external libraries
	target_link_libraries(unit_test ${RAVEN_LIB_LIBRARIES})
	target_link_libraries(unit_test ${RAVEN_TEST_LIBRARIES})

endif()


# build regression tests if enabled
if(BUILD_REGR_TESTS)

	# build type defaults to debug
	set(CMAKE_BUILD_TYPE DEBUG)

	include_directories(tests/regression)
	include_directories(${RAVEN_TEST_INCLUDE})

	# clear any source files from previous add_source() calls
	clear_added_sources()

	add_subdirectory(tests/regression)
	
	set(RAVEN_REGR_SOURCE ${RAVEN_SOURCE} ${ADDED_SRCS})

	add_executable(regr_test tests/regression/main.cpp ${RAVEN_REGR_SOURCE})

	# link external libraries
	target_link_libraries(regr_test ${RAVEN_LIB_LIBRARIES})
	target_link_libraries(regr_test ${RAVEN_TEST_LIBRARIES})

endif()


# build integration tests if enabled
if(BUILD_INTR_TESTS)

	# build type defaults to debug
	set(CMAKE_BUILD_TYPE DEBUG)

	include_directories(tests/integration)
	include_directories(${RAVEN_TEST_INCLUDE})

	# clear any source files from previous add_source() calls
	clear_added_sources()

	add_subdirectory(tests/integration)

	set(RAVEN_INTR_SOURCE ${RAVEN_SOURCE} ${ADDED_SRCS})

	add_executable(intr_test tests/integration/main.cpp ${RAVEN_INTR_SOURCE})

	# link external libraries
	target_link_libraries(intr_test ${RAVEN_LIB_LIBRARIES})
	target_link_libraries(intr_test ${RAVEN_TEST_LIBRARIES})

endif()