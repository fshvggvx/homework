cmake_minimum_required(VERSION 3.16)
project(opencv_demo)
set(OpenCV_DIR D:/opencv_4.8.0/opencv/build/x64/vc16/lib)
find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc imgcodecs)

add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE ${OpenCV_LIBS})