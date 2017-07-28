cmake_minimum_required (VERSION 2.8) 
 
set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/${CMAKE_MODULE_PATH}") 
find_package(raspicam REQUIRED)
find_package(OpenCV)
 
add_executable (raspicam_test raspicam_test.cpp)  
target_link_libraries (raspicam_test ${raspicam_CV_LIBS})
