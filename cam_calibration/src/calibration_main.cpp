#include <cam_calibration/calibration.h>

int main(){ 
    CameraCalibrator myCameraCalibrator; 
    printf("1\n");
    myCameraCalibrator.setFilename(); 
    printf("2\n");
    myCameraCalibrator.setBorderSize(Size(7,5));
    printf("3\n"); 
    myCameraCalibrator.addChessboardPoints();
    printf("4\n");

    Mat src = imread("src/cam_calibration/data/chessboard09.jpg",0);
    Mat dst,map1,map2;
    myCameraCalibrator.calibrate(src, dst);

    imshow("Original Image", src);
    imshow("Undistorted Image", dst);
    waitKey();
    return 0;
}
