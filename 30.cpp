//去除小区域面积
#include "opencv.hpp"
#include "iostream"
using namespace std;
using namespace cv;
 
int main()
{
	Mat srcImage, grayImage, dstImage,imgHSVMask;
	int size = 800;           //面积因子
	//srcImage = imread("Road2.png");
	/*imshow("原图", srcImage);
	cvtColor(srcImage, grayImage, COLOR_RGB2GRAY);
	threshold(grayImage, dstImage, 100, 255, THRESH_BINARY);
	imshow("二值图", dstImage);*/
 
	Mat img = imread("E:\\photo\\03\\08\\02\\3.png");
	cvtColor(img, grayImage, COLOR_RGB2GRAY);
	threshold(grayImage, imgHSVMask, 100, 255, THRESH_BINARY);
	imshow("原图", imgHSVMask);   
 
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));   //针对高亮部分
	erode(imgHSVMask, imgHSVMask, element);
	imshow("腐蚀", imgHSVMask);
 
	// 提取连通区域，并剔除小面积联通区域
	vector<vector<Point>> contours;           //二值图像轮廓的容器
	vector<Vec4i> hierarchy;                  //4个int向量，分别表示后、前、父、子的索引编号
	findContours(imgHSVMask, contours, hierarchy,RETR_LIST, CHAIN_APPROX_NONE);             //检测所有轮廓
	//contours.erase(remove_if(contours.begin(), contours.end(),[](const vector<Point>& c) {return contourArea(c) < 800; }), contours.end());  //vector.erase 删除元素
	// 显示图像并保存
	/*imgHSVMask.setTo(0);
	drawContours(imgHSVMask, contours, -1, Scalar(255), FILLED);
	imshow("处理图", imgHSVMask); */
 
	Mat ImageContours = Mat::zeros(imgHSVMask.size(), CV_8UC1);  //绘制
	Mat ImgContours= Mat::zeros(imgHSVMask.size(), CV_8UC1);
 
	vector<vector<Point>>::iterator k;                    //迭代器，访问容器数据
 
	for (k = contours.begin(); k != contours.end();)      //遍历容器,设置面积因子
	{
		if (contourArea(*k, false) < size)
		{//删除指定元素，返回指向删除元素下一个元素位置的迭代器
			k = contours.erase(k);
		}
		else
			++k;
	}
 
	//contours[i]代表第i个轮廓，contours[i].size()代表第i个轮廓上所有的像素点
	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = 0; j < contours[i].size(); j++)
		{
			//获取轮廓上点的坐标
			Point P = Point(contours[i][j].x, contours[i][j].y);
			ImgContours.at<uchar>(P) = 255;
		}
		drawContours(ImageContours, contours,i, Scalar(255), -1, 8);
	}
 
	imshow("轮廓", ImageContours);
	imshow("轮廓点集合", ImgContours);
	waitKey(0);
    return 0;
}