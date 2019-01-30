#!/bin/bash

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
