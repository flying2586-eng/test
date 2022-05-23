//将光流点聚类
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
static void drawmeans(const Mat& flow, Mat& cflowmap)
{
	int sampleCount=cflowmap.cols*cflowmap.rows;
	int dims=cflowmap.channels();
	int clusterCount=2;
	Mat points(sampleCount,dims,CV_32F,Scalar(10));
	Mat labels;
	Mat centers(clusterCount,1,points.type());

	int index=0;
	for(int row=0;row<cflowmap.rows;row++)
	{
		for(int col=0;col<cflowmap.cols;col++)
		{
			index=row*cflowmap.cols+col;
			Vec3b rgb=cflowmap.at<Vec3b>(row,col);
			points.at<float>(index,0)=static_cast<int>(rgb[0]);
			points.at<float>(index,1)=static_cast<int>(rgb[1]);
			points.at<float>(index,2)=static_cast<int>(rgb[2]);
		}
	}
	kmeans(points,clusterCount,labels,TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1),2,KMEANS_PP_CENTERS,centers);
	for(int i=0;i<centers.rows;i++)
	{
		int x=centers.at<float>(i,0);
		int y=centers.at<float>(i,1);
		rectangle(flow,Point(x-50,y-50),Point(x+50,y+50),Scalar(0,255,0),2,8);
	}
	imshow("a",flow);

}
void drawflow(Mat& flow)
{
	Mat a(flow.rows,flow.cols,CV_8UC3);
	a=Scalar::all(255);
	// imshow("zeros",a);
	for (int y = 0; y <flow.rows; y++ )
		for (int x = 0; x < flow.cols; x++ )
		{
			
			const Point2f& fxy = flow.at<Point2f>(y, x);
			if(sqrt(fxy.x*fxy.x+fxy.y*fxy.y)>8)
			{
				a.at<Vec3b>(y,x)=Vec3b(0,0,255);
			}
			// imshow("b",a);
			// cout<<fxy;
		}
		imshow("b",a);

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
		
		adaptiveThreshold(gray,gray,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,3);
		GaussianBlur(gray,gray,Size(7,7),0,0);
		namedWindow("a",0);
		imshow("a",gray);
		if (!prevgray.empty())
		{
			calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.1, OPTFLOW_FARNEBACK_GAUSSIAN);
			//cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
			uflow.copyTo(flow);
			// cvtColor(preframe,preframe,CV_GRAY2RGB);

			// drawmeans(preframe,flow);
			drawflow(flow);
			drawOptFlowMap(flow, preframe, 16, 1.5, Scalar(0,0,255));
		
			imshow("flow", preframe);

			
            /* stringstream str;
            str<<"E:\\photo\\03\\08\\"<<i<<".png";
            cout<<str.str()<<endl;
            imwrite(str.str(),preframe);
            i++; */
		}
		if (waitKey(800) >= 0)
			break;
		std::swap(prevgray, gray);
		std::swap(preframe, frame);
		// std::swap(preframe, gray);
	}
	return 0;



}