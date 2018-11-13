cmake_minimum_required(VERSION 3.8)

project(fmilib++)

set(PACKAGE_BUGREPORT "yuhang.neu@gmail.com")
set(PACKAGE_NAME "fmilib++")

set(PACKAGE_VERSION_MAJOR "0")
set(PACKAGE_VERSION_MINOR "1")
set(PACKAGE_VERSION_PATCH "0")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
	message(STATUS "Setting build type to ${CMAKE_BUILD_TYPE}")
endif()

MESSAGE(STATUS "OS detected: ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "CXX Compiler detected: ${CMAKE_CXX_COMPILER_ID}")
MESSAGE(STATUS "Install dir: ${CMAKE_INSTALL_PREFIX}")

option(ENABLE_DOC "Generates the documentation target" OFF)
option(ENABLE_COVERAGE "Generates the coverage build" OFF)
option(ENABLE_TESTING "Turns on testing" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include_directories(include)

# Enable code coverage with -DENABLE_COVERAGE=1
if(ENABLE_COVERAGE)
    set(CMAKE_BUILD_TYPE "Coverage")
    include(CodeCoverage)
    setup_target_for_coverage(${PROJECT_NAME}_coverage ${PROJECT_NAME}_test coverage)
endif()

# Enable documentation with -DENABLE_DOC=1
if(ENABLE_DOC)
    add_subdirectory(doc)
endif()

####################################################################
# FMILibrary
# Get FMILibrary from git
####################################################################
message(STATUS "Building FMILibrary from source.")
set(FMILIBRARY_CPP_PLATFORM "unknown" CACHE STRING "FMI platform defines the subdirectory within FMU where binary is located")
if(FMILIBRARY_CPP_PLATFORM MATCHES "unknown")
	if(CMAKE_HOST_WIN32) #Set to true when the host system is Windows and on cygwin.
		set(FMI_PLATFORM win)
	elseif(CMAKE_HOST_APPLE) #Set to true when the host system is Apple OSX. 
		set(FMI_PLATFORM darwin)
	elseif(CMAKE_HOST_UNIX) #Set to true when the host system is UNIX or UNIX like (i.e. APPLE and CYGWIN). 
		set(FMI_PLATFORM linux)
	else(CMAKE_HOST_WIN32)
		message(SEND_ERROR "Target platform is not supported by the tests")
		set(FMI_PLATFORM not_supported)
	endif()
	
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(FMI_PLATFORM "${FMI_PLATFORM}32")
	else()
		set(FMI_PLATFORM "${FMI_PLATFORM}64")
	endif()
	
	set(FMILIBRARY_CPP_PLATFORM ${FMI_PLATFORM} CACHE STRING "FMI platform defines the subdirectory within FMU where binary is located" FORCE)
else()
	set(FMI_PLATFORM ${FMILIBRARY_CPP_PLATFORM})
endif()

include(ExternalProject)
set(FMIL_SETTINGS 
	"-DFMILibrary_SOURCE_DIR:PATH=${CMAKE_SOURCE_DIR}/3rdparty/FMILibrary-2.0.3"
	"-DFMILibrary_BINARY_DIR:PATH=${CMAKE_BINARY_DIR}/FMILibrary-2.0.3"
	"-DFMILIB_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/FMILibrary-2.0.3/Install"
	"-DFMILIB_FMI_PLATFORM:STRING=${FMILIBRARY_CPP_PLATFORM}"
	"-DFMILIB_DEFAULT_BUILD_TYPE_RELEASE:BOOLEAN=ON"
	"-DFMILIB_BUILD_STATIC_LIB:BOOLEAN=ON"
	"-DFMILIB_BUILD_SHARED_LIB:BOOLEAN=OFF"
	"-DFMILIB_INSTALL_SUBLIBS:BOOLEAN=OFF"
	"-DFMILIB_GENERATE_DOXYGEN_DOC:BOOLEAN=OFF"
	"-DFMILIB_BUILD_TESTS:BOOLEAN=OFF"
	"-DFMILIB_GENERATE_BUILD_STAMP:BOOLEAN=OFF"
	)

ExternalProject_Add(
	FMILibrary
	GIT_REPOSITORY "https://github.com/svn2github/FMILibrary.git"
	GIT_TAG "master"
	SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/FMILibrary-2.0.3"
	BINARY_DIR "${CMAKE_BINARY_DIR}/FMILibrary-2.0.3"
	INSTALL_DIR "${CMAKE_BINARY_DIR}/FMILibrary-2.0.3/Install"
	CMAKE_CACHE_ARGS ${FMIL_SETTINGS}
)

set(FMILibrary_LIBRARY_DIRS ${CMAKE_BINARY_DIR}/FMILibrary-2.0.3/Install/lib)
set(FMILibrary_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/FMILibrary-2.0.3/Install/include)
set(fmilib_name "${FMILibrary_LIBRARY_DIRS}/${CMAKE_STATIC_LIBRARY_PREFIX}fmilib${CMAKE_STATIC_LIBRARY_SUFFIX}")

add_library(fmilib STATIC IMPORTED)
set_target_properties(
	fmilib PROPERTIES 
		IMPORTED_LOCATION "${fmilib_name}"
)
add_dependencies(fmilib FMILibrary)

add_library(fmilib++ INTERFACE)

target_link_libraries(fmilib++ INTERFACE fmilib)
target_include_directories(
	fmilib++
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${FMILibrary_INCLUDE_DIRS}>
		$<INSTALL_INTERFACE:include>
	)

#target_compile_features(fmilib++ INTERFACE cxx_std_17)
if(ENABLE_TESTING OR ENABLE_COVERAGE)
    enable_testing()
    add_subdirectory(tests)
endif()
