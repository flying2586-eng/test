//与19相同
#include<opencv2/opencv.hpp>
#include<opencv2/tracking.hpp>
using namespace cv;
 
int main()
{
	VideoCapture capture;
	Mat frame;
	//保存目标轨迹  
	std::vector<Point> pt;
	//capture.open(0);
	frame = capture.open("E:\\photo\\Videos\\run\\lyova_run.avi");
	if (!capture.isOpened())
	{
		printf("can not open camera \n");
		return -1;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	namedWindow("output", WINDOW_AUTOSIZE);
	capture.read(frame);
	if (frame.empty())
		return -1;
	Rect2d first = selectROI("output", frame);
	Rect selectionROI;
	selectionROI.width = first.width;
	selectionROI.height = first.height;
	selectionROI.x = first.x;
	selectionROI.y = first.y;
	printf("x= %d, y=%d, width=%d, height=%d", selectionROI.x, selectionROI.y, selectionROI.width, selectionROI.height);
 
	Mat mask, hist, backproject;
	int bins = 120;
	Mat drawImg = Mat::zeros(300, 300, CV_8UC3);
 
	while (capture.read(frame))
	{
		Mat hsvimage;
		cvtColor(frame, hsvimage, CV_BGR2HSV);
		inRange(hsvimage, Scalar(25, 43, 46), Scalar(35, 256, 256), mask);
		Mat hue = Mat(hsvimage.size(), hsvimage.depth());
		int channels[] = { 0, 0 };
		mixChannels(&hsvimage, 1, &hue, 1, channels, 1);
 
		//ROI直方图计算
		Mat roi(hue, first);
		Mat maskroi(mask, first);
		float hrange[] = { 0, 180 };
		const float* hranges = hrange;
		//直方图
		calcHist(&roi, 1, 0, maskroi, hist, 1, &bins, &hranges);
		normalize(hist, hist, 0, 255, NORM_MINMAX);
 
 
		int binw = drawImg.cols / bins;
		Mat colorIndex = Mat(1, bins, CV_8UC3);
		for (int i = 0; i < bins; i++)
		{
			colorIndex.at<Vec3b>(0, i) = Vec3b(saturate_cast<uchar>(i * 180 / bins), 255, 255);
		}
		cvtColor(colorIndex, colorIndex, COLOR_HSV2BGR);
		for (int i = 0; i < bins; i++)
		{
			int val = saturate_cast<int>(hist.at<float>(i)*drawImg.rows / 255);
			rectangle(drawImg, Point(i*binw, drawImg.rows), Point((i + 1)*binw, drawImg.rows * val), Scalar(colorIndex.at<Vec3b>(0, i)), -1, 8, 0);
		}
 
		//计算直方图的反投影
		calcBackProject(&hue, 1, 0, hist, backproject, &hranges);
		backproject &= mask;
		RotatedRect trackBox = CamShift(backproject, selectionROI, TermCriteria((TermCriteria::COUNT | TermCriteria::EPS), 10, 1));
		Rect rect;
		rect.x = trackBox.center.x - trackBox.size.width / 2.0;
		rect.y = trackBox.center.y - trackBox.size.height / 2.0;
		rect.width = trackBox.size.width;
		rect.height = trackBox.size.height;
 
		rectangle(frame, rect, Scalar(255, 255, 0), 3);
 
		pt.push_back(Point(rect.x + rect.width / 2, rect.y + rect.height / 2));
		for (int i = 0; i<pt.size() - 1; i++)
		{
			line(frame, pt[i], pt[i + 1], Scalar(0, 255, 0), 2.5);
		}
		imshow("input", frame);
		imshow("output", drawImg);
		waitKey(500);
	}
 
	capture.release();
	return 0;
}