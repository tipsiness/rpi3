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

using namespace cv;
using namespace std;

#define MAX_FRAME	4
#define REQ_FRAME_CNT	4

//#define HDMI_DISPLAY	0
#define TX_DATA_SIZE	8192
#define RX_DATA_SIZE	8192

int sleep_divisor = 1;

// declare Mat Array for 4-frame
//unsigned char *frame_data;
//unsigned char *total_frames;

int capture(int cnt);

int server_fd, client_fd;
int getIpAddress(int, char*);

int main(int, char *argv[])
{
	int len;
	int i;
	socklen_t addrlen;
	struct sockaddr_in server_addr, client_addr;

	char recv_buf[BUFF_SIZE];
	char send_buf[BUFF_SIZE];

	pid_t pid;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = htonl(inet_addr("192.168.100."));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//server_addr.sin_port = htons(atoi(argv[1]));
	server_addr.sin_port = htons(SERVER_PORT);

	// Show IP Address
	char *strIpAddr = (char*)malloc(sizeof(40));
	memset(strIpAddr, 0x0, 40);
	/*
	strIpAddr = inet_ntoa(server_addr.sin_addr);
	printf("IP Address: %s\n", strIpAddr);
	*/
	int ret = getIpAddress(1, strIpAddr);
	printf("IP Address: %s\n", strIpAddr);

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("bind error\n");
		exit(-1);
	}

	if (listen(server_fd, 5) < 0) {
		printf("listen error\n");
		exit(-1);
	}

	while (1) {
		addrlen = sizeof(client_addr);

		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);

		if ((pid = fork()) < 0) {
			printf("fork error\n");
			exit(-1);
		} else if (pid == 0) { // child process
			printf("New client connected [%d]\n", client_fd);

			cv::namedWindow("Display", CV_WINDOW_AUTOSIZE);

			char *frameData;
			char *tmp;

			int total_len = 0;
			int recv_len = 0;
			int recv_size;

			Mat image = Mat(Size(640, 480), CV_8UC3);

			recv_size = 640 * 480 * 3;
			frameData = (char*)malloc(sizeof(char)*recv_size);
			tmp = (char*)malloc(sizeof(char)*RX_DATA_SIZE);

			while (1) {	
				while (total_len < recv_size) {
					recv_len = read(client_fd, (char*)tmp, RX_DATA_SIZE);
					printf("recv_len: %d\n", recv_len);
					memcpy(&frameData[total_len], (char*)tmp, recv_len);
					total_len += recv_len;
					printf("total_len: %d\n", total_len);
					memset(tmp, 0x0, RX_DATA_SIZE);
				}

				printf("receive completed\n");
				memcpy(image.data, frameData, recv_size);
				imshow("Display", image);
				total_len = 0;

				if (cv::waitKey(1) > 0) {
					break;
				}
			}

			printf("Client %d is disconnected.\n", client_fd);
			close(client_fd);
			exit(0);
		} 
	}

	close(server_fd);

	return 0;
}

int getIpAddress(int iNetType, char *ip_addr) {
	struct ifreq ifr;
	int sock = 0;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (iNetType == 0) {
		strcpy(ifr.ifr_name, "eth0");
	} else {
		strcpy(ifr.ifr_name, "wlan0");
	}

	if (ioctl(sock, SIOCGIFADDR, &ifr) > 0) {
		strcpy(ip_addr, "0.0.0.0");
		return -1;
	}

	sprintf(ip_addr, "%s", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
	close(sock);

	return 0;
}

