cmake_minimum_required (VERSION 3.5)
project (wrSamples)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Some samples are not supported on the Jetson platform
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
	set(JETSON TRUE CACHE BOOL "" FORCE)
else()
	set(JETSON FALSE CACHE BOOL "" FORCE)
endif()

add_subdirectory(c)
add_subdirectory(cpp)
