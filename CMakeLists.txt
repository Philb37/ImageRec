cmake_minimum_required(VERSION 3.9)
project(ImageRec)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
find_package( OpenCV REQUIRED )
find_package(PythonLibs 3.5.2 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
pkg_check_modules(GTKMM gtkmm-3.0)
find_package(Caffe REQUIRED)


if(ON)
    if(NOT OpenCV_FOUND)
        set(Caffe_OpenCV_CONFIG_PATH "/usr/local/share/OpenCV")
        if(Caffe_OpenCV_CONFIG_PATH)
            get_filename_component(Caffe_OpenCV_CONFIG_PATH ${Caffe_OpenCV_CONFIG_PATH} ABSOLUTE)

            if(EXISTS ${Caffe_OpenCV_CONFIG_PATH} AND NOT TARGET opencv_core)
                message(STATUS "Caffe: using OpenCV config from ${Caffe_OpenCV_CONFIG_PATH}")
                include(${Caffe_OpenCV_CONFIG_PATH}/OpenCVModules.cmake)
            endif()

        else()
            find_package(OpenCV REQUIRED)
        endif()
        unset(Caffe_OpenCV_CONFIG_PATH)
    endif()
endif()


get_filename_component(Caffe_CMAKE_DIR "/home/uqamportable/Documents/RobotLib/caffe/build/cmake" PATH)


#if(NOT TARGET caffe AND NOT caffe_BINARY_DIR)
include("//home/uqamportable/Documents/RobotLib/caffe/build/CaffeTargets.cmake")
#endif()

# List of IMPORTED libs created by CaffeTargets.cmake
#set(Caffe_LIBRARIES "/home/uqamportable/Documents/RobotLib/caffe/build/lib/libcaffe.so")



set(Caffe_DEFINITIONS "-DUSE_OPENCV;-DUSE_LMDB;-DUSE_LEVELDB")





include_directories(${Caffe_INCLUDE_DIRS})
link_directories(${Caffe_LIBRARY_DIRS})
list(APPEND Caffe_INCLUDE_DIRS "/usr/include")
list(APPEND Caffe_INCLUDE_DIRS "/usr/local/include")

list(APPEND Caffe_INCLUDE_DIRS "/usr/local/include/opencv")
list(APPEND Caffe_INCLUDE_DIRS "/usr/local/include/opencv2")



find_package(Threads REQUIRED)
find_package(freenect2 REQUIRED HINTS "ENV{HOME}/freenect2")

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("/usr/include/libusb-1.0/")
include_directories("./Header files")
include_directories("./Header files/CNN")
include_directories("./Source files/")
include_directories("./Source files/CNN")


INCLUDE_DIRECTORIES(
        ${freenect2_INCLUDE_DIR}
)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

add_executable(ImageRec VideoArea.cpp MainWindow.cpp main.cpp Kinect.cpp "Source files/ImageSegmentationManual.cpp" "Header files/ImageSegmentationManual.h" "Source files/Items.cpp" ROISelection.cpp ROISelection.h FileManager.cpp FileManager.h KinectTreatment.cpp KinectTreatment.h  GoogleCNN.cpp GoogleCNN.h WebCam.cpp WebCam.h RealSense.cpp  Classifier.cpp "Source files/CNN/CaffeCNN.cpp" Socket.cpp )
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${GTK3_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${FREENECT2_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${freenect2_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${Caffe_LIBRARIES} -lglog -lprotobuf -lboost_system)
target_link_libraries(${CMAKE_PROJECT_NAME} ${Protobuf_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${PYTHON_LIBRARIES})
