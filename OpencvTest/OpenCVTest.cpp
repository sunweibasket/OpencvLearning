#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <imgproc/imgproc.hpp>
#include <imgproc/imgproc_c.h>

#include <stdio.h>


using namespace cv;
string imageFileName = "pic.png";
string videoFileName = "Benz.mp4";
string outVideoFileName = "BenzCopy.avi";


void openImageMat()
{
	Mat img = imread("pic.png");

	cvNamedWindow("openImage");
	imshow("openImage", img);
	cvNamedWindow("openImageSmooth");
	IplImage* image = cvLoadImage(imageFileName.c_str());
	cvShowImage("openImageSmooth", image);

	/*-------------smooth--------------------*/
	IplImage* dstImg = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U,3);
	cvSmooth(image, dstImg, CV_GAUSSIAN, 3, 3);  //需要image类型
	cvShowImage("openImageSmooth", dstImg);

	/*-------------cvPyrDown--------------------*/
	assert(image->width/2 !=0 && image->height/2 != 0);
	IplImage* out = cvCreateImage(cvSize(image->width / 2, image->height / 2), image->depth, image->nChannels);
	cvPyrDown(image, out);
	cvShowImage("openImageSmooth", out); 


	while (1)
	{
		if (27 == waitKey())
			break;
	}

	cvReleaseImage(&image);
	cvReleaseImage(&dstImg);
	cvReleaseImage(&out);

}


void callBackMat(int parm, void *cap){
	VideoCapture *temp = (VideoCapture*)cap;
	temp->set(CV_CAP_PROP_POS_FRAMES, parm);
}

void callBack(int parm, void *cap){
	CvCapture *temp = (CvCapture*)cap;
}
int openVideoMat()
{
	VideoCapture cap("Benz.mp4"); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	Mat frame;
	namedWindow("video", 1);
	createTrackbar("Trackbar", "video", 0, cap.get(CV_CAP_PROP_FRAME_COUNT), callBackMat, &cap);
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

int openImage(const char* filename)
{
	IplImage* img = cvLoadImage(filename);
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example1", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyAllWindows();
	return 0;
}

void openVedio(const char* filename)
{
	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(filename);  //版本更新后用 VideoCapture 和Mat 较少用IplImage
	IplImage* frame;
	createTrackbar("Trackbar", "Example2", 0, cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT), callBack, &capture);

	while (1)
	{
		frame = cvQueryFrame(capture);
		if (!frame)
			break;
		cvShowImage("Example2",frame);
		char c = cvWaitKey(33); //sleep 33ms ，并且返回按键输入ASCII 27对应ESC
		if (c > 0)
			printf("user input is %d ！",c);

		if (c == 27)
			break;

	}
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();
}

void writeVideo(const char* filename, const char* outFilename)
{
	CvCapture* capture = cvCreateFileCapture(filename);
	if (!capture)
		return;
	IplImage *bgr_frame = cvQueryFrame(capture);
	double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	CvSize size = cvSize((int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH),
						 (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT));
	CvVideoWriter *writer = cvCreateVideoWriter(outFilename,                            //支持AVI格式
		CV_FOURCC('M', 'J', 'P', 'G'), //CV_FOURCC('P', 'I', 'M', '1'),
		fps,
		size);
	IplImage* logpolar_frame = cvCreateImage(size, IPL_DEPTH_8U, 3);
	while ((bgr_frame=cvQueryFrame(capture))!= NULL)
	{
		cvLogPolar(bgr_frame, logpolar_frame, cvPoint2D32f(bgr_frame->width/2, bgr_frame->height / 2), 40, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
		cvWriteFrame(writer, logpolar_frame);
	}
	cvReleaseVideoWriter(&writer);
	cvReleaseImage(&logpolar_frame);
	cvReleaseCapture(&capture);
}
int main()
{
	//openImageMat();
	//openVideoMat();
	//openImage(imageFileName.c_str());
	//openVedio(videoFileName.c_str());
	//openImageMat();
	writeVideo(videoFileName.c_str(), outVideoFileName.c_str());

}
