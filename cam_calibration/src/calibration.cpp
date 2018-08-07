#include <cam_calibration/calibration.h>
#include <iostream>  
#include <fstream>  
#include <iterator>  
#include <vector>  

void CameraCalibrator::load( cv::Mat& matData,string fileName)  
{  
    int retVal = 0;  
     int matRows = 480;
      int matCols = 640;
      int matChns = 0;
    // 打开文件  
    ifstream inFile(fileName.c_str(), ios_base::in);  
    if(!inFile.is_open())  
    {  
        cout << "读取文件失败" << endl;  
        retVal = -1;  
        //return (retVal);  
    }  
  
    // 载入数据  
    istream_iterator<float> begin(inFile);    //按 float 格式取文件数据流的起始指针  
    istream_iterator<float> end;          //取文件流的终止位置  
    vector<float> inData(begin,end);      //将文件数据保存至 std::vector 中  
    cv::Mat tmpMat = cv::Mat(inData);       //将数据由 std::vector 转换为 cv::Mat  
  
    // 输出到命令行窗口  
    //copy(vec.begin(),vec.end(),ostream_iterator<double>(cout,"\t"));   
  
    // 检查设定的矩阵尺寸和通道数  
    size_t dataLength = inData.size();  
    //1.通道数  
    if (matChns == 0)  
    {  
        matChns = 1;  
    }  
    //2.行列数  
    if (matRows != 0 && matCols == 0)  
    {  
        matCols = dataLength / matChns / matRows;  
    }   
    else if (matCols != 0 && matRows == 0)  
    {  
        matRows = dataLength / matChns / matCols;  
    }  
    else if (matCols == 0 && matRows == 0)  
    {  
        matRows = dataLength / matChns;  
        matCols = 1;  
    }  
    //3.数据总长度  
    if (dataLength != (matRows * matCols * matChns))  
    {  
        cout << "读入的数据长度 不满足 设定的矩阵尺寸与通道数要求，将按默认方式输出矩阵！" << endl;  
        retVal = 1;  
        matChns = 1;  
        matRows = dataLength;  
    }   
  
    // 将文件数据保存至输出矩阵  
    matData = tmpMat.reshape(matChns, matRows).clone();  
      
    //return (retVal);  
}  

void CameraCalibrator::writeMatToFile(cv::Mat& m, const char* filename)
{
	std::ofstream fout(filename);
 
	if (!fout)
	{
		std::cout << "File Not Opened" << std::endl;  
		return;
	}
 
	for (int i = 0; i<m.rows; i++)
	{
		for (int j = 0; j<m.cols; j++)
		{
			fout << m.at<float>(i, j) << "\t";
		}
		fout << std::endl;
	}
 
	fout.close();
}


void CameraCalibrator::setFilename(){
    m_filenames.clear();
    m_filenames.push_back("src/cam_calibration/data/chessboard01.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard02.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard03.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard04.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard05.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard06.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard07.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard08.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard09.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard10.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard11.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard12.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard13.jpg"); 
    m_filenames.push_back("src/cam_calibration/data/chessboard14.jpg");
} 

void CameraCalibrator::setBorderSize(const Size &borderSize){ 
    m_borderSize = borderSize; 
} 

void CameraCalibrator::addChessboardPoints(){  
    vector<Point2f> srcCandidateCorners; 
    vector<Point3f> dstCandidateCorners; 
    for(int i=0; i<m_borderSize.height; i++){ 
        for(int j=0; j<m_borderSize.width; j++){ 
            dstCandidateCorners.push_back(Point3f(i, j, 0.0f)); 
        } 
    } 

    for(int i=0; i<m_filenames.size(); i++){ 
        Mat image=imread(m_filenames[i],CV_LOAD_IMAGE_GRAYSCALE); 
        findChessboardCorners(image, m_borderSize, srcCandidateCorners); 
        TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);
        cornerSubPix(image, srcCandidateCorners, Size(5,5), Size(-1,-1), param);  
        if(srcCandidateCorners.size() == m_borderSize.area()){ 
            addPoints(srcCandidateCorners, dstCandidateCorners); 
        } 
    } 
} 

void CameraCalibrator::addPoints(const vector<Point2f> &srcCorners, const vector<Point3f> &dstCorners){           
    m_srcPoints.push_back(srcCorners);  
    m_dstPoints.push_back(dstCorners); 
} 

void CameraCalibrator::calibrate(const Mat &src, Mat &dst){ 
    if(i<2){
    Size imageSize = src.size(); 
    Mat cameraMatrix, distCoeffs; 
    

    vector<Mat> rvecs, tvecs; 
    calibrateCamera(m_dstPoints, m_srcPoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);  
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), imageSize, CV_32F, map1, map2);
    writeMatToFile(map1,"src/cam_calibration/data/map1.txt");
    writeMatToFile(map2,"src/cam_calibration/data/map2.txt");
    }
    i++; 
    remap(src, dst, map1, map2, INTER_LINEAR); 
}


