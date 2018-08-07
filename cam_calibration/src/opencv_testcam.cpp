#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cam_calibration/calibration.h>
#include <sensor_msgs/CameraInfo.h>

CameraCalibrator myCameraCalibrator; 
Mat map1,map2;

int i = 0;
//static const std::string OPENCV_WINDOW = "Image window";
 
class ImageConverter
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_;
	image_transport::Publisher image_pub_;
	 ros::Subscriber sub1;
	 ros::Publisher pub;
	public:
	ImageConverter()
	: it_(nh_)
	{
	// Subscrive to input video feed and publish output video feed
	image_sub_ = it_.subscribe("/usb_cam/image_raw", 1,&ImageConverter::imageCb, this);
	image_pub_ = it_.advertise("/image_converter/output_video", 1);
	sub1 = nh_.subscribe("/usb_cam/camera_info", 1, &ImageConverter::imageInfo,this);
	pub = nh_.advertise<sensor_msgs::CameraInfo>("/image_converter/camera_info", 1);
	//cv::namedWindow(OPENCV_WINDOW);
	}
	 
	~ImageConverter()
	{
	//cv::destroyWindow(OPENCV_WINDOW);
	}
	 
	void imageInfo(const sensor_msgs::CameraInfo::ConstPtr& msg)
	{
		pub.publish(msg);
	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg)
	{
		i++;

	cv_bridge::CvImagePtr cv_ptr;
	try
	{
	cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception& e)
	{
	ROS_ERROR("cv_bridge exception: %s", e.what());
	return;
	}

	//myCameraCalibrator.calibrate(cv_ptr->image, cv_ptr->image);

	remap(cv_ptr->image, cv_ptr->image, map1, map2, INTER_LINEAR); 
	// Draw an example circle on the video stream
	if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
	//cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));
	 
	// Update GUI Window
	//cv::imshow(OPENCV_WINDOW, cv_ptr->image);
	cv::waitKey(3);
	 
	// Output modified video stream
	image_pub_.publish(cv_ptr->toImageMsg());
	}
};
 
int main(int argc, char** argv)
{
	ros::init(argc, argv, "image_converter");
	std::string map_data; 
	ros::NodeHandle n;
	n.param("map_path", map_data, std::string("src/cam_calibration"));
	ros::param::get("~map_path",map_data);
	
	myCameraCalibrator.load(map1,map_data+"/data/map1.txt");
	myCameraCalibrator.load(map2,map_data+"/data/map2.txt");

	ImageConverter ic;
	ros::spin();
	
	return 0;
}
