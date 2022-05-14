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
			line(cflowmap, Point(x, y), Point(cvRound(x + fxy.x), cvRound(y + fxy.y)),color);
			circle(cflowmap, Point(x, y), 2, color, -1);
		}
}

int main()
{
    //VideoCapture cap(0);
	VideoCapture cap;
	//cap.open("E:\\photo\\03\\test.avi");
    cap.open("E:\\photo\\20200319_Trim.avi");
	if (!cap.isOpened())
		return -1;
 
	Mat flow, cflow, frame, preframe;
	//Mat gray, prevgray, uflow;
	//从UMat修改为Mat
	Mat gray, prevgray, uflow;
	namedWindow("flow", 0);
    int i=0;
 
	for (;;)
	{
		//cap >> frame;
		bool ret = cap.read(frame);
		frame=frame(Rect(243,0,1252,610));
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		// blur(gray,gray,Size(7,7));
		// adaptiveThreshold(gray,gray,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,3);
		namedWindow("a",0);
		imshow("a",gray);
		if (!prevgray.empty())
		{
			calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.1, OPTFLOW_FARNEBACK_GAUSSIAN);
			//cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
			uflow.copyTo(flow);
			// cvtColor(preframe,preframe,CV_GRAY2RGB);
			drawOptFlowMap(flow, preframe, 16, 1.5, Scalar(0,0,255));
		
			imshow("flow", preframe);
		
            /* stringstream str;
            str<<"E:\\photo\\03\\08\\"<<i<<".png";
            cout<<str.str()<<endl;
            imwrite(str.str(),preframe);
            i++; */
		}
		if (waitKey(500) >= 0)
			break;
		std::swap(prevgray, gray);
		std::swap(preframe, frame);
		// std::swap(preframe, gray);
	}
	return 0;



}