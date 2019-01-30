#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	VideoCapture cap(0); // open the video camera 0

	if (!cap.isOpened()) return -1;

	//get the width of the video frame
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	//get the height of the video frame
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
	//Initialize the VideoWriter object
	VideoWriter videoWriter("myvideo.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, true);

	//If VideoWriter is not initialized successfully, exit the program
	if (!videoWriter.isOpened())
	{
		cout << "ERROR: Failed to write the video" << endl;
		return -1;
	}

	while (1) {
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video
		if (!bSuccess) {
			cout << "ERROR: Cannot read a frame from video file" << endl;
			break;
		}
		//write the frame into the file
		videoWriter.write(frame);
	}
	return 0;
}

