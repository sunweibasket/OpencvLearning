#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <imgproc/imgproc.hpp>
#include <imgproc/imgproc_c.h>

#include <stdio.h>


using namespace cv;



void openImage()
{
	Mat img = imread("pic.png");
	Mat dstImg;

	cvNamedWindow("openImage");
	imshow("openImage", img);
	cvNamedWindow("openImageSmooth");
	//cvSmooth(&img, &dstImg);  //需要image类型
	//imshow("smoothImg", dstImg);

	waitKey(6000);
}


void callBack(int parm, void *cap){
	VideoCapture *temp = (VideoCapture*)cap;
	temp->set(CV_CAP_PROP_POS_FRAMES, parm);
}
int openVideo()
{
	VideoCapture cap("Benz.mp4"); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	Mat frame;
	namedWindow("video", 1);
	createTrackbar("Trackbar", "video", 0, cap.get(CV_CAP_PROP_FRAME_COUNT), callBack, &cap);
	for (;;)
	{
		if(!(cap.read(frame))) //cap >> frame
			break; // get a new frame from camera
		setTrackbarPos("Trackbar", "video", cap.get(CV_CAP_PROP_POS_FRAMES));
		imshow("video", frame);
		if (waitKey(cap.get(CV_CAP_PROP_FPS)) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

int main()
{
	openImage();
	//openVideo();
}
