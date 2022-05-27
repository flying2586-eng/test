
#include <iostream>  
#include<vector>
#include<algorithm>
#include <opencv2\opencv.hpp>  
#include <opencv2\highgui\highgui.hpp>  
using namespace std;
using namespace cv;
 
 
//轮廓按照面积大小升序排序
bool ascendSort(vector<Point> a, vector<Point> b) {
	return a.size() < b.size();
 
}
 
//轮廓按照面积大小降序排序
bool descendSort(vector<Point> a, vector<Point> b) {
	return a.size() > b.size();
}
int main() {
	Mat srcImage = imread("E:\\photo\\03\\08\\001\\5.png");
	Mat thresholdImage;
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	threshold(grayImage, thresholdImage, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	//Mat resultImage;
	//thresholdImage.copyTo(resultImage);
	vector< vector< Point> > contours;  //用于保存所有轮廓信息
	vector< vector< Point> > contours2; //用于保存面积不足100的轮廓
	vector<Point> tempV;				//暂存的轮廓
 
	findContours(thresholdImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	//cv::Mat labels;
	//int N = connectedComponents(resultImage, labels, 8, CV_16U);
	//findContours(labels, contours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
 
	//轮廓按照面积大小进行升序排序
	sort(contours.begin(), contours.end(), ascendSort);//升序排序
	vector<vector<Point> >::iterator itc = contours.begin();
	int i = 0;
	while (itc != contours.end())
	{
		//获得轮廓的矩形边界
		Rect rect = boundingRect(*itc);
		int x = rect.x;
		int y = rect.y;
		int w = rect.width;
		int h = rect.height;
		//绘制轮廓的矩形边界
		cv::rectangle(srcImage, rect, { 0, 0, 255 }, 1);
		//保存图片
		// char str[10];
		// sprintf(str, "%d.jpg", i++);
		// cv::imshow("srcImage", srcImage);
		// imwrite(str, srcImage);
		// waitKey(1000);
	
		if (itc->size() < 100)
		{
			//把轮廓面积不足100的区域，放到容器contours2中，
			tempV.push_back(Point(x, y));
			tempV.push_back(Point(x, y+h));
			tempV.push_back(Point(x+w, y+h));
			tempV.push_back(Point(x+w, y));
			contours2.push_back(tempV);
			/*也可以直接用：contours2.push_back(*itc);代替上面的5条语句*/
			//contours2.push_back(*itc);
 
			//删除轮廓面积不足100的区域，即用黑色填充轮廓面积不足100的区域：
			cv::drawContours(srcImage, contours2, -1, Scalar(0,0,0), CV_FILLED);
		}
		//保存图片
		// sprintf(str, "%d.jpg", i++);
		cv::imshow("srcImage", srcImage);
		// imwrite(str, srcImage);
		cv::waitKey(100);
		tempV.clear();
		++itc;
	}
    cv::waitKey(0);
	return 0;
}