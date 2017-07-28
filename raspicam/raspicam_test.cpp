#include <raspicam/raspicam_cv.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
	raspicam::RaspiCam_Cv Camera; // Camera Object
	cv::Mat frame;

	// Set camera params
	Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3); // For color

	// Open camera
	cout << "Opening camera..." << endl;
	if (!Camera.open()) {
		cerr << "Error opening camera!" << endl; 
		return -1; 
	}
	cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE); 

	for (;;) { 
		Camera.grab(); 
		Camera.retrieve(frame); 
		cv::imshow("Display Window", frame); 

		if (cv::waitKey(20) == 27)
		    break;
 	}

	cout << "Stopping camera.." << endl;
	Camera.release();
	return 0;
}

