cmake_minimum_required(VERSION 3.8)
project(RaspberryRecorder)

include_directories(include)

#Set module path
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

set(CMAKE_CXX_STANDARD 17)

if (OPENCV4)
    set(OpenCV_CORE_INCLUDE_DIR /usr/local/include/opencv4/opencv2)
    set(OpenCV_HIGHGUI_INCLUDE_DIR /usr/local/include/opencv4/opencv2/highgui)
    set(OpenCV_ROOT_DIR /usr/local/include/opencv4/opencv2)
    set(OpenCV_INCLUDE_DIR /usr/local/include/opencv4)
endif()

FIND_PACKAGE(OpenCV)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})

FIND_PACKAGE(LibUVC)
INCLUDE_DIRECTORIES(${uvc_INCLUDE_DIRS})

#FIND_PACKAGE(FRNETLIB)
#INCLUDE_DIRECTORIES(${FRNETLIB_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp include/Shell.h src/Shell.cpp src/ClipRecorder.cpp include/ClipRecorder.h)
add_executable(RaspberryRecorder ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(RaspberryRecorder ${OpenCV_LIBRARIES} opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs opencv_video opencv_videoio opencv_objdetect)
TARGET_LINK_LIBRARIES(RaspberryRecorder ${uvc_LIBRARIES} uvc -lpthread)
#TARGET_LINK_LIBRARIES(RaspberryRecorder ${FRNETLIB_LIBRARIES})