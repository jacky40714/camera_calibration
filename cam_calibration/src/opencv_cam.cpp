#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <limits.h>
#include <libgen.h>
#include <signal.h>
#include <math.h>
#include <time.h>

using namespace cv;
using namespace std;

int iLowH = 0;
int iHighH = 0;

int iLowS = 0; 
int iHighS = 0;

int iLowV = 0;
int iHighV = 0;

int cam_brightness = 10;
int cam_contrast = 50;
int cam_saturation = 100;

int cam_hue = 50;
int cam_exposure = 50;
int cam_auto = false;
int cam_focus = 50;


Mat frame;
Mat image;
Mat finder;
Mat imgOriginal;
Mat imgHSV;
// 	CvCapture *capture;
VideoCapture capture(0);


int Pixels = 0;
double Proportion = 0;

class Point2D
{
	public:
		double x;
		double y;
};

Point2D center_pos;
Point temp;


void scanframe();
void trackbar();

int main(){

	//capture = cvCaptureFromCAM(0);
	cvNamedWindow("Webcam",0);
	//cvResizeWindow("Webcam",640,480 );
	
	trackbar();
	capture.read(frame);
	imshow("Webcam",frame);

	while(1){
		double cam_B = (double)cam_brightness/100;
	    double cam_C = (double)cam_contrast/100;
	    double cam_S = (double)cam_saturation/100;

	    double cam_A = (double)cam_auto/100;
	    double cam_F = (double)cam_focus;
	    double cam_E = (double)cam_exposure;
	    double cam_H = (double)cam_hue/100;
	    

	    capture.set(CV_CAP_PROP_BRIGHTNESS, cam_B);
	    capture.set(CV_CAP_PROP_CONTRAST, cam_C);
	    capture.set(CV_CAP_PROP_SATURATION, cam_S);

	    capture.set(CV_CAP_PROP_HUE, cam_H);
	    //capture.set(CV_CAP_PROP_EXPOSURE, cam_E);
	    //capture.set(CV_CAP_PROP_AUTO_EXPOSURE, cam_A);
	    //capture.set(CV_CAP_PROP_FOCUS, cam_F);
	
	    scanframe();
	}
	//cvReleaseCapture(&capture);
	//cvDestroyWindow("Webcam");
    return 0;



}


void scanframe(){
	Proportion = 0;
    Pixels = 0;
	//frame = cvQueryFrame(capture);//catch capture
	capture.read(frame);
    flip(frame,frame,1);
    //BrightnessAndContrastAuto(frame,frame,1);
	imgOriginal = frame;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), finder);
    erode(finder, finder, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  	dilate( finder, finder, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  	int r=0, g=0, b=0;
  	double sum_x=0,sum_y=0,count=0;
  	
	//////////catch ROI , conunt pixel , get center//////////
	for(int y =0;y<=finder.rows;y+=1)
	{
		for(int x =0;x<=finder.cols;x+=1)
		{
			r=0, g=0, b=0;
			if(finder.at<uchar>(y,x)==255 )
			{
				sum_y +=y;
				sum_x +=x;
				count +=1;
				Pixels+=1;
                r = 255;
                g = 0;
                b = 0;
			}
			if(r > 0 || g > 0 || b > 0){
				imgOriginal.at<Vec3b>(y, x)[0] = b;//B
				imgOriginal.at<Vec3b>(y, x)[1] = g;//G
				imgOriginal.at<Vec3b>(y, x)[2] = r;//R
			}
				r=0, g=0, b=0;
			
		}
	}

		if(count > 0){
			center_pos.x =(sum_x/count);
			center_pos.y =(sum_y/count);
		}
		else
		{
			center_pos.x = -1;
			center_pos.y = -1;
		}

		
		temp.x = center_pos.x;
		temp.y = center_pos.y;

		circle(frame,temp,20,Scalar(0,255,0),CV_FILLED,8,0);

		Proportion=(double)Pixels/(finder.cols*finder.rows);
		printf("Proportion %.2lf ",Proportion);
		printf("center_pos %.2lf , %.2lf  \n",center_pos.x,center_pos.y);
		//printf("Proportion %.2lf ",Proportion);
		//resize( finder, finder, Size(finder.cols / 2, finder.rows / 2) );
		//imshow("Image", finder);

       	imshow("Webcam",frame);
       	int key = cvWaitKey(20);
}

void trackbar(){
	namedWindow("color_Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	moveWindow( "color_Control", 1, 620 );
  		//Create trackbars in "Control" window
 	createTrackbar("LowH", "color_Control", &iLowH, 179); //Hue (0 - 179)
 	createTrackbar("HighH", "color_Control", &iHighH, 179);

  	createTrackbar("LowS", "color_Control", &iLowS, 255); //Saturation (0 - 255)
 	createTrackbar("HighS", "color_Control", &iHighS, 255);

 	createTrackbar("LowV", "color_Control", &iLowV, 255);//Value (0 - 255)
 	createTrackbar("HighV", "color_Control", &iHighV, 255);

 	namedWindow("cam control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	moveWindow( "cam control", 600, 20 );
  	//Create trackbars in "Control" window
 	createTrackbar("brightness", "cam control", &cam_brightness, 100);
 	createTrackbar("contrast", "cam control", &cam_contrast, 100);
  	createTrackbar("saturation", "cam control", &cam_saturation, 100);

  	createTrackbar("hue", "cam control", &cam_hue, 100);
 	createTrackbar("exposure", "cam control", &cam_exposure, 100);
  	createTrackbar("focus", "cam control", &cam_focus, 100);
  	createTrackbar("auto", "cam control", &cam_auto, 1);
  	
}