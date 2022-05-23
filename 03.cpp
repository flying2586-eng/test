#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

Mat frame, grayImg;  // 当前帧
Mat prev_frame, prev_grayImg;  //前一帧
vector<Point2f>features;  //Shi-Tomasi 焦点检测-特征数据
vector<Point2f>fpts[2];  //保证当前帧和前一帧的特征点位置
vector<Point2f>iniPoints;  //初始化特征数据
vector<uchar>status;  //特征点跟踪成功标志位
vector<float>errors;  //跟踪时候区域误差和

//Shi-Tomasi 焦点检测
void detectFeatures(Mat &inFrame, Mat &inGray)
{
	double maxCorners = 500;
	double qualitylevel = 0.01;
	double minDistance = 10;
	double blockSize = 3;
	double k = 0.04;

	// 算法很快，满足实时性要求
	goodFeaturesToTrack(inGray, features, maxCorners,
		qualitylevel, minDistance, Mat(), blockSize, false, k);
	cout << "detect features:" << features.size() << endl;
}

//绘制特征点
void drawFeature(Mat &inFrame)
{
	for (size_t i = 0; i < fpts[0].size(); i++)
	{
		circle(inFrame, fpts[0][i], 2, Scalar(0, 0, 255), 2);
	}
}
//在跟踪到的且移动了的特征点（光流）的开始跟踪的位置到当前跟踪到的位置之间绘制线段
void drawTrackLines()
{
	for (int i = 0; i < fpts[1].size(); i++)
	{
		line(frame, iniPoints[i], fpts[1][i], Scalar(0, 255, 0), 2, 8, 0);  //绘制线段
		cout << "两个特征点之间的距离是：" << abs(fpts[1][i].y - iniPoints[i].y) << endl;
		circle(frame, fpts[1][i], 2, Scalar(0, 0, 255), 2, 8, 0);
	}
}

// 稀疏光流法跟踪 KTL
void KLTrackFeature()
{
	calcOpticalFlowPyrLK(prev_grayImg, grayImg, fpts[0], fpts[1], status, errors);
	int k = 0;  //保证跟踪到的特征点数，最后将特征点的尺寸重新设置为 k
	for (int i = 0; i < fpts[1].size(); i++)
	{
		double dist = abs(fpts[0][i].x - fpts[1][i].x) + abs(fpts[0][i].y - fpts[1][i].y);
		if (dist > 2 && status[i])  //跟踪到的特征点，且距离移动了 2 以上的
		{
			//将跟踪到的移动了的特征点在 vector 中连续起来，剔除掉损失的和静止不动的特征点（这些跟踪点在前面帧中）
			iniPoints[k] = iniPoints[i];
			fpts[1][k++] = fpts[1][i];  //同上，只是这些跟踪点在当前帧中
		}
	}
	//保存特征点并绘制跟踪轨迹
	iniPoints.resize(k);
	fpts[1].resize(k);
	drawTrackLines();

	swap(fpts[1], fpts[0]);  //交换，将此帧跟踪到特征点作为下一帧的待跟踪点
}
void test()
{

	VideoCapture capture("E:\\photo\\20200319_Trim.avi");
	//VideoCapture capture(0);  //打开摄像头
	if (!capture.isOpened())
	{
		cout << "could not load video file...\n" << endl;
	}

	namedWindow("Result", CV_WINDOW_AUTOSIZE);

	while (capture.read(frame))
	{
		frame=frame(Rect(243,0,1252,610));//ROI区域


		resize(frame, frame, Size(540, 480), 0, 0, INTER_CUBIC);
		cvtColor(frame, grayImg, COLOR_BGR2GRAY);

		//跟踪40个特征点，如果跟踪的时候损失了一些特征点，重新检测，追加
		if (fpts[0].size() < 40)
		{
			detectFeatures(frame, grayImg);
			fpts[0].insert(fpts[0].end(), features.begin(), features.end());  //追加带跟踪的特征点
			iniPoints.insert(iniPoints.end(), features.begin(), features.end());
		}
		else
		{
			cout << "tracjing...\n" << endl;
		}

		if (prev_grayImg.empty())
		{
			grayImg.copyTo(prev_grayImg);
		}

		KLTrackFeature();  //稀疏光流跟踪 KLT
		drawFeature(frame);  //绘制特征点
		//更新前一帧的数据
		grayImg.copyTo(prev_grayImg);
		frame.copyTo(prev_frame);
		imshow("Result", frame);

		char c = waitKey(50);
		if (c == 27)
		{
			break;
		}
	}
	capture.release();
}
int main()
{
	test();
	waitKey(0);
	return 0;
}