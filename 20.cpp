//Meanshift
#include <iostream>
#include <vector>
#include <core/core.hpp>
#include <imgproc/imgproc.hpp>
#include <highgui/highgui.hpp>

#include<opencv2/xfeatures2d/nonfree.hpp>
#include <features2d/features2d.hpp>


#include <opencv2/video/tracking.hpp>
 
using namespace std;
using namespace cv;
 
int meanShiftTest(Mat probImage, Rect& window, int maxCount, double epsilon);
 
int main()
{
	/* 第一部分：将输入图像转为灰度图后计算反投影 */
	Mat image = imread("baboon1.jpg", 0);
	Mat image_2 = imread("baboon3.jpg", 0);
	Mat imageROI = image(Rect(133, 44, 25, 70));
	Mat image_show;
	image.copyTo(image_show);
	rectangle(image_show, Rect(133, 44, 25, 70), Scalar(0));
	imshow("image_show", image_show);
 
	int histSize[1];
	float hranges[2];
	const float* ranges[1];
	int channels[1];
	histSize[0] = 256;
	hranges[0] = 0.f;
	hranges[1] = 255.f;
	ranges[0] = hranges;
	channels[0] = 0;
 
	MatND hist;
	calcHist(&imageROI, 1, channels, Mat(), hist, 1, histSize, ranges);
	normalize(hist, hist, 1.0);
 
	Mat result;	
	calcBackProject(&image_2, 1, channels, hist, result, ranges, 255.0);
	imshow("result", result);
 
	/* 第二部分:转换为HSV后利用颜色信息计算反投影 */
	image = imread("baboon1.jpg");
	image_2 = imread("baboon3.jpg");
	imageROI = image(Rect(133, 44, 25, 70));
	image.copyTo(image_show);
	rectangle(image_show, Rect(133, 44, 25, 70), Scalar(0, 0, 0));
	imshow("image_show_hsv", image_show);
 
	Mat image_hsv;
	cvtColor(imageROI, image_hsv, CV_BGR2HSV);
	Mat mask;
	int minSaturation = 65;
	vector<Mat> v;
	split(image_hsv, v);
	threshold(v[1], mask, minSaturation, 255, THRESH_BINARY);
	histSize[0] = 256;
	hranges[0] = 0.f;
	hranges[1] = 180.f;
	ranges[0] = hranges;
	channels[0] = 0;
	MatND hist_hsv;
	calcHist(&image_hsv, 1, channels, mask, hist_hsv, 1, histSize, ranges);
	normalize(hist_hsv, hist_hsv, 1.0);
 
	Mat image_2_hsv;
	cvtColor(image_2, image_2_hsv, CV_BGR2HSV);
	vector<Mat> v_2;
	split(image_2_hsv, v_2);
	threshold(v_2[1], v_2[1], minSaturation, 255, THRESH_BINARY);
 
	Mat result_hsv;
	calcBackProject(&image_2_hsv, 1, channels, hist_hsv, result_hsv, ranges, 255.0);
	bitwise_and(result_hsv, v_2[1], result_hsv);
	imshow("result_hsv", result_hsv);
 
	/* 第三部分：调用opencv中的 meanShift计算位置 */
	Rect rect(133, 44, 25, 70);
	TermCriteria criteria(TermCriteria::MAX_ITER, 10, 0.01);
	long long t = getTickCount();
	meanShift(result_hsv, rect, criteria);
	cout<<"time: "<<(getTickCount() - t)/getTickFrequency()<<endl;
	rectangle(image_2, rect, Scalar(255, 255, 255));
 
	/* 测试自己写的meanshift */
	Rect rect2(133, 44, 25, 70);
	t = getTickCount();
	meanShiftTest(result_hsv, rect2, 30, 0.01);
	cout<<"time: "<<(getTickCount() - t)/getTickFrequency()<<endl;
	//rectangle(image_2, rect2, Scalar(255, 255, 255));
	rectangle(image_2, Rect(133, 44, 25, 70), Scalar(0, 0, 0));
	imshow("image_show_hsv_result", image_2);
 
	cv::waitKey();
	return 0;
 
}
 
 
int meanShiftTest(Mat probImage, Rect& window, int maxCount, double epsilon)
{
	if(probImage.type() != CV_8UC1)
	{
		return -1;
	}
	/* 应该要做window是否合适的判断，譬如参数是否合适，是否位于probImage内部等
	 * 这里偷懒不写了
	 */
	Mat imageROI = probImage(window);
	int x_o = imageROI.cols / 2;
	int y_o = imageROI.rows / 2;
 
	for(int i = 0; i < maxCount; i++)// 迭代次数
	{
		float x_dst, y_dst;
		x_dst = y_dst = 0.f;
		float weight_sum = 0;
		int sum = 0;
		/* 计算meanshift向量（重心相对于中心的偏移） */
		for(int m = 0; m < imageROI.rows; m++)
		{
			for(int n = 0; n< imageROI.cols; n++)
			{
				if(imageROI.at<unsigned char>(m,n))
				{
					sum++;
					int weight = imageROI.at<unsigned char>(m,n);
					weight_sum += weight;
					x_dst += (n - x_o)*weight;
					y_dst += (m - y_o)*weight;
				}
			}
		}
		if(sum > 0)
		{
			x_dst /= (sum*weight_sum);
			y_dst /= (sum*weight_sum);
			window.x += x_dst;
			window.y += y_dst;
			if(fabs(x_dst) + fabs(y_dst) < epsilon)// 位移变化阈值
				break;
		}
		imageROI = probImage(window);
	}
	return 0;
}