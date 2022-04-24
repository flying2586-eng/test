#include<opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv;

static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
	double, const Scalar& color)
{
	for (int y = 0; y < cflowmap.rows; y += step)
		for (int x = 0; x < cflowmap.cols; x += step)
		{
			const Point2f& fxy = flow.at<Point2f>(y, x);
			line(cflowmap, Point(x, y), Point(cvRound(x + fxy.x), cvRound(y + fxy.y)),
				color);
			circle(cflowmap, Point(x, y), 2, color, -1);
		}
}

int main()
{
    Mat img1=imread("E:\\photo\\03\\45.jpg");
    Mat img2=imread("E:\\photo\\03\\46.jpg");
    cvtColor(img1,img1,COLOR_BGR2GRAY);
    cvtColor(img2,img2,COLOR_BGR2GRAY);

    Mat flow(img1.size(),CV_32FC2);

    calcOpticalFlowFarneback(img1,img2,flow,0.5,3,15,3,5,1.1,OPTFLOW_FARNEBACK_GAUSSIAN);
    drawOptFlowMap(flow,img2,4,1.5,Scalar(0,255,0));
    imshow("",img2);
    waitKey(0);
    return 0;



}