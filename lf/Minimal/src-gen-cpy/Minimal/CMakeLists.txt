cmake_minimum_required(VERSION 3.13)

## Need this for cross-compiling
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

## Enable assembly (.S file)
project(Minimal LANGUAGES C ASM)

## Add custom gcc flags
# SET(GCC_COVERAGE_COMPILE_FLAGS "-I../../../../../flexpret/programs/lib/include/ -g -static -O0 -march=rv32i -mabi=ilp32 -nostartfiles")
SET(GCC_COVERAGE_COMPILE_FLAGS "-I../../../../../flexpret/programs/lib/include/ -g -static -O0 -march=rv32i -mabi=ilp32 -nostartfiles --specs=nosys.specs")
SET(CMAKE_C_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")

## Add custom linker flag
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ../../../flexpret.ld")

## Check if the script if updated.
message(STATUS "hello==============================================")

# Require C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEFAULT_BUILD_TYPE Release)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE} CACHE STRING "Choose the type of build." FORCE)
endif()
set(CoreLib core)
set(PlatformLib platform)

include(${CoreLib}/platform/Platform.cmake)
include_directories(${CoreLib})
include_directories(${CoreLib}/platform)
include_directories(${CoreLib}/federated)
set(LF_MAIN_TARGET Minimal)

# Declare a new executable target and list all its sources
## Add tinyalloc and start.S here.
add_executable(${LF_MAIN_TARGET} ../../start.S Minimal.c ${CoreLib}/platform/${LF_PLATFORM_FILE} ctarget.c core/mixed_radix.c ../../tinyalloc.c)

install(TARGETS ${LF_MAIN_TARGET}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

