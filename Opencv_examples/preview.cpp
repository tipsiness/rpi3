#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	VideoCapture cap(0); // open the video camera 0
	if (!cap.isOpened()) return -1;

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 

	cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	cv::namedWindow("Preview", cv::WINDOW_AUTOSIZE);

	while (1) {
		Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video
		if (!bSuccess) {
			cout << "ERROR: Cannot read a frame from video file" << endl;
			break;
		}

		imshow("Preview", frame);

		if (cv::waitKey(1) == 27) {
			break;
		}
	}

	return 0;
}

