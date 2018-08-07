#include <cstdio>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

class CameraCalibrator{
private:
    vector<string> m_filenames;
    Size m_borderSize;
    vector<vector<Point2f> > m_srcPoints;
    vector<vector<Point3f> > m_dstPoints;
    Mat map1,map2;
    int i=0; 
public:
    void writeMatToFile(cv::Mat& m, const char* filename);
    void setFilename();
    void setBorderSize(const Size &borderSize);
    void addChessboardPoints();
    void addPoints(const vector<Point2f> &imageCorners, const vector<Point3f> &objectCorners);
    void calibrate(const Mat &src, Mat &dst);
    void load( cv::Mat& matData,string fileName);
};