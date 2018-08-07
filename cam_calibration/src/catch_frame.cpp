#include <opencv2/opencv.hpp>
#include <iostream>
 
using namespace std;
using namespace cv;
 
int main()
{
	VideoCapture capture(0);
	int delay = 1000 / 30;
	string a[] = {"chessboard01.jpg","chessboard02.jpg","chessboard03.jpg","chessboard04.jpg","chessboard05.jpg","chessboard06.jpg",
		"chessboard07.jpg","chessboard08.jpg","chessboard09.jpg","chessboard10.jpg","chessboard11.jpg","chessboard12.jpg","chessboard13.jpg","chessboard14.jpg"};
	int i = 0;
	while (1)
	{
		Mat frame;
		capture >> frame;
		imshow("show",frame);
		//imwrite("camera1.jpg", frame);
		int key;
		key = waitKey(delay);
		if ( 27 == (char)key)
		{
			imwrite(a[i],frame);
			printf("num = %d \n",i);
			i++;
		}
		
	}

}