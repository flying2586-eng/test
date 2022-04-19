//光流法
//先使用腐蚀膨胀消除电线
//先确定ROI区域
#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap("E:\\photo\\test_bird_Trim.mp4");
	//Mat image = imread("E:\\photo\\04_result\\17.png");
    Mat image;
    while(cap.read(image))
    {
	namedWindow("原图", 0);
	imshow("原图", image);
	Mat imageROI;
	imageROI = image(Rect(500, 0, 1000, 500));
	namedWindow("1", 0);
	imshow("1", imageROI);

	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat imageROI_1;
	

	  Mat imageROI_2;
	 dilate(imageROI,imageROI_1,kernel_dilate);
	 namedWindow("膨胀",0);
	 imshow("膨胀",imageROI_1); 

	 erode(imageROI_1, imageROI_2, kernel_erode);
	 namedWindow("腐蚀", 0);
	 imshow("腐蚀", imageROI_2);
//二值化
    Mat out;
    cvtColor(imageROI_2,imageROI_2,CV_BGR2GRAY);
    threshold(imageROI_2,out,150,255,CV_THRESH_BINARY);
    imshow("",out);
    waitKey(1000);
    }
	
	return 0;
}