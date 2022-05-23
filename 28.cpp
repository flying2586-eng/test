//光流保存
#include<opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/xfeatures2d/nonfree.hpp>
#include<fstream>
#include<strstream>
#include<math.h>
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
// void writeflow(Mat& flow)
// {
//     for(int row=0;row<flow.rows;row++)
//     {
//         for(int col=0;col<flow.cols;col++)
//         {
//             string str=""
//         }
//     }
// }

int main()
{
    //VideoCapture cap(0);
	VideoCapture cap;
	//cap.open("E:\\photo\\03\\test.avi");
    cap.open("E:\\photo\\02.avi");
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
		frame=frame(Rect(243,0,1252,310));
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		// blur(gray,gray,Size(7,7));
		adaptiveThreshold(gray,gray,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,3);
		GaussianBlur(gray,gray,Size(7,7),0,0);
		namedWindow("a",0);
		imshow("a",gray);
		if (!prevgray.empty())
		{
			calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.1, OPTFLOW_FARNEBACK_GAUSSIAN);
			//cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
			uflow.copyTo(flow);
			// imshow("a",flow);
			// cvtColor(preframe,preframe,CV_GRAY2RGB);
			drawOptFlowMap(flow, preframe, 16, 1.5, Scalar(0,0,255));
		
			imshow("flow", preframe);
            
            static int iiii =0;

            string NEWfilename = "E:\\photo\\03\\08\\05\\"+to_string(iiii)+".csv";
			string NEWfilename_2 = "E:\\photo\\03\\08\\06\\"+to_string(iiii)+".csv";
			string NEWfilename_3 = "E:\\photo\\03\\08\\07\\"+to_string(iiii)+".csv";
            // ofstream file(NEWfilename,ios::out);
			// ofstream file_2(NEWfilename_2,ios::out);
            ofstream file_3(NEWfilename_3,ios::out);
            for(int row=0;row<flow.rows;row++)
            {
                for(int col=0;col<flow.cols;col++)
                {
                    // const Point2f& fxy=flow.at<Point2f>(row,col);
                    // if(fxy.x!=0&&fxy.y!=0)
                    //     file<<"("<<row<<","<<col<<")"<<fxy<<" ";
                    // string bb="";

                    // file <<flow.at<Point2f>(row,col).x<<"."<<flow.at<Point2f>(row,col).y<<',';
					const Point2f& fxy=flow.at<Point2f>(row,col);
					// float a,b;
					// {
					// 	if(fabs(fxy.x)<0.01)
					// 	a=0;
					// 	else
					// 	a=fxy.x;
					// 	if(fabs(fxy.y)<0.01)
					// 	b=0;
					// 	else
					// 	b=fxy.y;
					// }
					// file<<"x"<<','<<a<<','<<"y"<<','<<b<<',';

					// file<<fxy<<',';

					// float longmodel=sqrt(fxy.x*fxy.x+fxy.y*fxy.y);
					// if(longmodel<0.01)
					// 	longmodel=0;
					// file_2<<longmodel<<',';
					
					float a,b;
					{
					if(fabs(fxy.x)<0.01)
						a=0;
					else
						a=fxy.x;
					if(fabs(fxy.y)<0.01)
						b=0;
					else
						b=0-fxy.y;
					}
					float angleflow=atan2(b,a);
					float angleflow_2=angleflow*180/3.14159;
					file_3<<angleflow_2<<',';







                }
                // file<<endl;
				// file_2<<endl;
				file_3<<endl;
            }

            file_3.close();
			// file_2.close();
            // file.close();
            iiii++;
		
            /* stringstream str;
            str<<"E:\\photo\\03\\08\\"<<i<<".png";
            cout<<str.str()<<endl;
            imwrite(str.str(),preframe);
            i++; */
		}
		if (waitKey(10) >= 0)
			break;
		std::swap(prevgray, gray);
		std::swap(preframe, frame);
		// std::swap(preframe, gray);
	}
	return 0;



}