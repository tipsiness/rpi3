#!/bin/bash

echo "======> Update repositories"
sudo apt update 
sudo apt upgrade

echo "======> Install dependencies"
sudo apt-get purge wolfram-engine -y
sudo apt-get install build-essential git cmake pkg-config libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libgtk2.0-dev libatlas-base-dev gfortran python2.7-dev python3-dev -y

echo "======> Download opencv.zip"
wget https://github.com/Itseez/opencv/archive/3.1.0.zip -O opencv.zip

echo "======> Unzip opencv.zip"
unzip opencv.zip

echo "======> Download opencv_contrib.zip"
wget https://github.com/Itseez/opencv_contrib/archive/3.1.0.zip -O opencv_contrib.zip

echo "======> Download opencv_contrib.zip"
unzip opencv_contrib.zip

cd opencv-3.1.0
mkdir build
cd build

# if occurs 'pre-compiled header' additional include option '-D ENABLE_PRECOMPILED_HEADERS=OFF'.
echo "======> Configuration opencv"
cmake -D CMAKE_BUILD_TYPE=RELEASE \
-D CMAKE_INSTALL_PREFIX=/usr \
-D BUILD_NEW_PYTHON_SUPPORT=ON \
-D WITH_V4L=ON \
-D INSTALL_C_EXAMPLES=OFF \
-D INSTALL_PYTHON_EXAMPLES=OFF \
-D BUILD_EXAMPLES=OFF \
-D WITH_QT=OFF \
-D WITH_OPENGL=ON \
-D WITH_IPP=OFF \
-DOPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.1.0/modules ../

echo "======> Compile opencv"
make -j4

echo "======> Install opencv"
sudo make install
sudo ldconfig
