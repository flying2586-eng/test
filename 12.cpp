#include<opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture capture;
    capture.open("E:\\photo\\Videos\\run\\daria_run.avi");
    if(!capture.isOpened())
    {
        return 0;
    }
    Mat pre_gray,now_gray;
    capture.read(pre_gray);
    cvtColor(pre_gray,pre_gray,COLOR_BGR2GRAY);
    Mat flow=Mat::zeros(pre_gray.size(),CV_32FC2);

    Mat frame, move_x, move_y;
	move_x = Mat::zeros(flow.size(), CV_32F);
	move_y = Mat::zeros(flow.size(), CV_32F);
	while (capture.read(frame))
	{
		cvtColor(frame, now_gray, COLOR_BGR2GRAY);
		calcOpticalFlowFarneback(pre_gray, now_gray, flow, 0.5, 3, 9, 1, 7, 1.5, OPTFLOW_FARNEBACK_GAUSSIAN);
		for (int row = 0; row < flow.rows; row++)
		{
			for (int col = 0; col < flow.cols; col++)
			{
				//遍历光流矩阵，将x和y方向上的移动值分别存放为独立的矩阵
				Point2f move_x_y = flow.at<Point2f>(row, col);
				move_x.at<float>(row, col) = move_x_y.x;
				move_y.at<float>(row, col) = move_x_y.y;
			}
		}

		Mat mag, ang;
		cartToPolar(move_x, move_y, mag, ang);		//将移动矢量从笛卡尔坐标转换极坐标，输出像素点移动的幅度和相位（弧度）
		Mat H, S, V;
		//以相位值来确定色调H，像素点的不同移动方向形成不同颜色
		H = ang * 180 / CV_PI / 2;			//将弧度转为角度，且在OpenCV中H取值范围是0~180°
		convertScaleAbs(H, H);
		//饱和度S设置最大255，色彩最浓，有利于观察
		S = Mat::zeros(move_x.size(), CV_8UC1);
		S = Scalar(255);
		//亮度V由像素点的移动幅度决定，幅度越大则亮度越大
		normalize(mag, V, 0, 255, NORM_MINMAX);
		convertScaleAbs(V, V);

		vector<Mat> HSV(3);
		HSV[0] = H;
		HSV[1] = S;
		HSV[2] = V;
		Mat result;
		merge(HSV, result);
		//转换到RGB色彩空间进行显示
		cvtColor(result, result, COLOR_HSV2BGR);
		imshow("result", result);

		char ch = cv::waitKey(20);
		if (ch == 27)
		{
			break;
		}
		//将当前帧图像，置为上一帧图像
		swap(pre_gray, now_gray);
    }

    capture.release();


    
}