#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#define BUFF_SIZE	1024
#define SERVER_PORT	9999

#define MAX_FRAME	4
#define REQ_FRAME_CNT	4

#define HDMI_DISPLAY	0
#define RX_DATA_SIZE	8192
#define TX_DATA_SIZE	8192

using namespace cv;
using namespace std;

unsigned char *frame_data;

int server_fd, client_fd;

int main(int, char**)
{
	int len;
	int i;
	int recv_len;
	int recv_size;
	int total_len = 0;
	socklen_t addrlen;
	struct sockaddr_in server_addr, client_addr;

	char recv_buf[BUFF_SIZE];
	char send_buf[BUFF_SIZE];

	char *frameData;
	char *tmp;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.XXX");
	server_addr.sin_port = htons(SERVER_PORT);

	connect(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	recv_size = 640 * 480 * 3;
	frameData = (char*)malloc(sizeof(char)*recv_size);
	tmp = (char*)malloc(sizeof(char)*RX_DATA_SIZE);

	long total_tx_len = 0;
	int tx_len = 0;

	VideoCapture cap(0); // open the default camera

	cap.set(CV_CAP_PROP_FPS, 60);

	while (1) {
		Mat frame;
		bool ret = cap.read(frame);
		// copy first frame to memory buffer
		cout << "Frame size: " << frame.size() << endl;
		cout << "Byte size: " << frame.total() * frame.elemSize() << endl;

		long data_size = frame.total() * frame.elemSize();

		memcpy((unsigned char*)frameData, (unsigned char*)frame.data, frame.rows*frame.cols*3);

		//printf("Sending data %ld bytes.\n", data_size);
		total_tx_len = 0;
		tx_len = 0;

		printf("Sending data %ld bytes.\n", frame.total() * frame.elemSize());
		
		total_tx_len = write(server_fd, (char*)frameData, data_size);

		printf("Data Transmission Completed: %ld\n", total_tx_len);
	}

	printf("send complated\n");
	free((void*)frame_data);

	close(server_fd);

	return 0;
}


