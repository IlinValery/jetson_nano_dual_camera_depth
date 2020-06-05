cmake_minimum_required(VERSION 3.10)
project(jetson_nano_dual_camera)

set(CMAKE_CXX_STANDARD 14)


add_executable(jetson_nano_dual_camera main.cpp camera_manager.cpp camera_manager.h depth_detector.cpp depth_detector.h)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(jetson_nano_dual_camera ${OpenCV_LIBS} pthread)