cmake_minimum_required (VERSION 3.5)
project (cpp CXX)

find_package(wrnchAI REQUIRED)
find_package(OpenCV
    OPTIONAL_COMPONENTS imgcodecs videoio
    COMPONENTS core highgui imgproc
)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(wrHello_sample_cpp src/wrHello_sample.cpp)
target_link_libraries(wrHello_sample_cpp ${OpenCV_LIBS} wrAPI)

add_executable(wrHands_sample_cpp src/wrHands_sample.cpp)
target_link_libraries(wrHands_sample_cpp ${OpenCV_LIBS} wrAPI)

add_executable(wrPose2d_sample_cpp src/wrPose2d_sample.cpp)
target_link_libraries(wrPose2d_sample_cpp ${OpenCV_LIBS} wrAPI)

add_executable(wrPose3d_sample_cpp src/wrPose3d_sample.cpp)
target_link_libraries(wrPose3d_sample_cpp ${OpenCV_LIBS} wrAPI)

add_executable(wrPose3dPostIK_sample_cpp src/wrPose3dPostIK_sample.cpp)
target_link_libraries(wrPose3dPostIK_sample_cpp ${OpenCV_LIBS} wrAPI)

add_executable(wrHead_sample_cpp src/wrHead_sample.cpp)
target_link_libraries(wrHead_sample_cpp ${OpenCV_LIBS} wrAPI)

add_executable(wrGesture_sample_cpp src/wrGesture_sample.cpp)
target_link_libraries(wrGesture_sample_cpp ${OpenCV_LIBS} wrAPI)

add_executable(wrJoint_definitions_cpp src/wrJointDefinitions_sample.cpp)
target_link_libraries(wrJoint_definitions_cpp wrAPI)

add_executable(wrSerialization_sample src/wrSerialization_sample.cpp)
target_link_libraries(wrSerialization_sample wrAPI)

#Activity/Gesture detection is not supported on the Jetson platform
if(NOT JETSON)
    add_executable(wrActivities_sample_cpp src/wrActivities_sample)
    target_link_libraries(wrActivities_sample_cpp ${OpenCV_LIBS} wrAPI)
endif()


if(WIN32)
    file(GLOB REQUIRED_DLLS ${wrnchAI_DIR}/../../bin/*.dll)

    add_custom_command(TARGET wrHello_sample_cpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${REQUIRED_DLLS}
        "$<TARGET_FILE_DIR:wrHello_sample_cpp>"
    )
endif()

