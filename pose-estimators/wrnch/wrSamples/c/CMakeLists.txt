cmake_minimum_required (VERSION 3.5)
project (c C)
set(CMAKE_C_STANDARD 99)

find_package(wrnchAI REQUIRED)

find_package(OpenCV 
    OPTIONAL_COMPONENTS imgcodecs
    COMPONENTS core highgui
)
if(OpenCV_VERSION VERSION_LESS "2.4")
    message(FATAL_ERROR "ERROR: ")
endif()

add_executable(pose2d_sample_c pose2d_sample.c wrOpenCvHelpers.cpp)
target_link_libraries(pose2d_sample_c ${OpenCV_LIBS} wrAPI)

# Activity/Gesture detection is not supported on the Jetson platform
if(NOT JETSON)
    add_executable(activities_sample_c activities_sample.c wrOpenCvHelpers.cpp)
    target_link_libraries(activities_sample_c ${OpenCV_LIBS} wrAPI)
endif()

if(WIN32)
    file(GLOB REQUIRED_DLLS ${wrnchAI_DIR}/../../bin/*.dll)

    add_custom_command(TARGET pose2d_sample_c POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${REQUIRED_DLLS}
        "$<TARGET_FILE_DIR:pose2d_sample_c>"
    )
endif()
