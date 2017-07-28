#include "opencv2/opencv.hpp"

using namespace cv;

int main() {
  VideoCapture cap(0);  //open the default camera
  if (!cap.isOpened()) { //check if we succeed
    printf("videocapture device open failed\n");
    return -1;
  }

  Mat frame;                  
  cap >> frame;         //get a new frame from camera
  imwrite("image.jpg", frame);
  
  //the camera is deinitialized automatically in 
  //VideoCapture destructor
  return 0;
}

