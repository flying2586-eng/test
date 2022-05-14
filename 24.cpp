//测试可以
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
 
using namespace cv;
using namespace std;
 
vector<string> trackerTypes = { "BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "MOSSE", "CSRT" };
 
/**
 * @brief Create a Tracker By Name object 根据设定的类型初始化跟踪器
 *
 * @param trackerType
 * @return Ptr<Tracker>
 */
Ptr<Tracker> createTrackerByName(string trackerType)
{
	Ptr<Tracker> tracker;
	if (trackerType == trackerTypes[0])
		tracker = TrackerBoosting::create();
	else if (trackerType == trackerTypes[1])
		tracker = TrackerMIL::create();
	else if (trackerType == trackerTypes[2])
		tracker = TrackerKCF::create();
	else if (trackerType == trackerTypes[3])
		tracker = TrackerTLD::create();
	else if (trackerType == trackerTypes[4])
		tracker = TrackerMedianFlow::create();
	else if (trackerType == trackerTypes[5])
		tracker = TrackerMOSSE::create();
	else if (trackerType == trackerTypes[6])
		tracker = TrackerCSRT::create();
	return tracker;
}
 
void getRandomColors(vector<Scalar> &colors, int numColors)
{
	RNG rng(0);
	for (int i = 0; i < numColors; i++)
	{
		colors.push_back(Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
	}
}
 
int main()
{
	vector<vector<Point>> vecPoint;
 
	Mat frame;
	vector<Rect> rois;
	string trackerType;
 
	string videoName;
 
	while (true)
	{
		vecPoint.clear();
 
		while (true)
		{
			cout << "\n\n------------------------------------------------------------------\n";
			cout << "\n\n>> 可测试算法有  BOOSTING  MIL  KCF  TLD  MEDIANFLOW  MOSSE  CSRT"
				"\n>> 请输入要测试的算法并按回车,退出输入exit。"
				"\n>> ";
 
			cin >> trackerType;
 
			if (trackerType == "exit")
			{
				return 0;
			}
 
			if (trackerType == "BOOSTING")
			{
				cout << "\n>> 选择BOODSTING成功！\n";
				break;
			}
			else if (trackerType == "MIL")
			{
				cout << "\n>> 选择MIL成功！\n";
				break;
			}
			else if (trackerType == "KCF")
			{
				cout << "\n>> 选择KCF成功！\n";
				break;
			}
			else if (trackerType == "TLD")
			{
				cout << "\n>> 选择TLD成功！\n";
				break;
			}
			else if (trackerType == "MEDIANFLOW")
			{
				cout << "\n>> 选择MEDIANFLOW成功！\n";
				break;
			}
			else if (trackerType == "MOSSE")
			{
				cout << "\n>> 选择MOSSE成功！\n";
				break;
			}
			else if (trackerType == "CSRT")
			{
				cout << "\n>> 选择CSRT成功！\n";
				break;
			}
			else
			{
				cout << "\n>> 选择失败\n";
				continue;
			}
		}
 
		cout << "\n>> 输入1选择本地视频进行播放"
			"\n>> 输入2选择实时摄像头播放"
			"\n>> ";
		int judgement;
		cin >> judgement;
		if (1 == judgement)
		{
			while (true)
			{
				cout << "\n +--------------+"
					"\n | 1.步行的人_1 |"
					"\n | 2.步行的人_2 |"
					"\n | 3.步行的人_3 |"
					"\n | 4.车         |"
					"\n | 5.超车       |"
					"\n | 6.大卫       |"
					"\n | 7.跳绳       |"
					"\n | 8.摩托越野   |"
					"\n | 9.熊猫       |"
					"\n | 10.大众汽车  |"
					"\n +--------------+"
					"\n\n>> 请输入要播放视频的序列号（例如4）"
					"\n>> ";
 
				int videoNo;
				cin >> videoNo;
 
				if (videoNo == 1)
				{
					videoName = "20200319_Trim.avi";
					cout << "\n>> 选择《步行的人 1》成功！";
					break;
				}
				else if (videoNo == 2)
				{
					videoName = "pedestrian2.mpg";
					cout << "\n>> 选择《步行的人 2》成功！";
					break;
				}
				else if (videoNo == 3)
				{
					videoName = "pedestrian3.mpg";
					cout << "\n>> 选择《步行的人 3》成功！";
					break;
				}
				else if (videoNo == 4)
				{
					videoName = "car.mpg";
					cout << "\n>> 选择《车》成功！";
					break;
				}
				else if (videoNo == 5)
				{
					videoName = "carchase.mpg";
					cout << "\n>> 选择《超车》成功！";
					break;
				}
				else if (videoNo == 6)
				{
					videoName = "david.mpg";
					cout << "\n>> 选择《大卫》成功！";
					break;
				}
				else if (videoNo == 7)
				{
					videoName = "jumping.mpg";
					cout << "\n>> 选择《跳绳》成功！";
					break;
				}
				else if (videoNo == 8)
				{
					videoName = "motocross.mpg";
					cout << "\n>> 选择《摩托越野》成功！";
					break;
				}
				else if (videoNo == 9)
				{
					videoName = "panda.mpg";
					cout << "\n>> 选择《熊猫》成功！";
					break;
				}
				else if (videoNo == 10)
				{
					videoName = "volkswagen.mpg";
					cout << "\n>> 选择《大众汽车》成功！";
					break;
				}
				else
				{
					cout << "\n>> 序列号错误，请重新输入！";
					continue;
				}
			}
			// Read video 读视频
			VideoCapture video("E:\\photo\\" + videoName);
 
			// Exit if video is not opened 如果没有视频文件
			if (!video.isOpened())
			{
				cout << "\n>> 读取视频失败";
				return -1;
			}
			cout << "\n +----------------------------+"
				"\n | 点击 c 逐帧播放视频        |"
				"\n | 点击 q 开始选择目标        |"
				"\n | 点击空格开始播放并追踪目标 |"
				"\n | 播放期间按q退出播放        |"
				"\n +----------------------------+\n";
 
			video >> frame;
			//! [getframe]
 
			while (1)
			{
				char key = waitKey(1);
				if (key == 'c')  // 按c键跳帧
				{
					video >> frame;
				}
				if (key == 'q')  // 按q键退出跳帧
				{
					break;
				}
				imshow("Tracker", frame);
			}
 
			cv::destroyWindow("Tracker");
			//end added
 
			//! [selectroi]选择目标roi以GUI的形式
			cv::selectROIs("tracker", frame, rois, false);
			//! [selectroi]
 
			if (rois.size() < 1)
			{
				return 0;
			}
 
			vector<Scalar> colors;
			getRandomColors(colors, rois.size());
 
			Ptr<MultiTracker> multiTracker = MultiTracker::create();
 
			for (int i = 0; i < rois.size(); i++)
			{
				multiTracker->add(createTrackerByName(trackerType), frame, Rect2d(rois[i]));
 
				vector<Point> temp;
				vecPoint.push_back(temp);
			}
 
			cout << "\n>> 开始播放\n";
			while (video.isOpened())
			{
				// get frame from the video
				video >> frame;
 
				if (frame.empty())
				{
					break;
				}
 
				// update the tracking result
				//! [update]
				bool ok = multiTracker->update(frame);
				//! [update]
 
				//! [visualization]
				// draw the tracked object
				for (unsigned i = 0; i < multiTracker->getObjects().size(); i++)
				{
					rectangle(frame, multiTracker->getObjects()[i], colors[i], 2, 1);
				}
				//! [visualization]
 
				for (int i = 0; i < multiTracker->getObjects().size(); i++)
				{
					Point presentPoint;
					presentPoint.x = (multiTracker->getObjects()[i].x + (multiTracker->getObjects()[i].width*0.5)) * 2;
					presentPoint.y = (multiTracker->getObjects()[i].y + (multiTracker->getObjects()[i].height*0.5)) * 2;
					vecPoint[i].push_back(presentPoint);
				}
 
				if (vecPoint.size() > 1)
				{
					for (int j = 0; j < vecPoint.size(); j++)
					{
						for (int k = 1; k < vecPoint[j].size(); k++)
						{
							line(frame, vecPoint[j][k - 1], vecPoint[j][k], colors[j], 1, 8, 1);
						}
					}
				}
 
				// show image with the tracked object
				imshow("tracker", frame);
				//! [visualization]
 
				if (char(waitKey(30)) == 'q') {
					break;
				}
			}
 
			cv::destroyWindow("tracker");
			cout << "\n>> 播放完毕\n";
		}
		else if (2 == judgement)
		{
			VideoCapture video(0, CAP_DSHOW);
			if (!video.isOpened())
			{
				cout << "\n>> 读取视频失败";
				continue;
			}
 
			cout << "\n>> 请按空格开始截取图片\n\n";
 
			while (1)
			{
				video >> frame;  //读取当前帧
 
				//若视频完成播放，退出循环
				if (frame.empty())
				{
					cout << "发生错误，请检查摄像头是否已断开！";
					break;
				}
 
				imshow("tracker", frame);  //显示当前帧
 
				if (char(waitKey(30)) == 32) {
					break;
				}
 
				//waitKey(30);  //延时30ms
			}
 
			//! [selectroi]选择目标roi以GUI的形式
			cv::selectROIs("tracker", frame, rois, false);
			//! [selectroi]
			if (rois.size() < 1)
			{
				return 0;
			}
 
			vector<Scalar> colors;
			getRandomColors(colors, rois.size());
 
			Ptr<MultiTracker> multiTracker = MultiTracker::create();
 
			for (int i = 0; i < rois.size(); i++)
			{
				multiTracker->add(createTrackerByName(trackerType), frame, Rect2d(rois[i]));
 
				vector<Point> temp;
				vecPoint.push_back(temp);
			}
 
			cout << "\n>> 开始播放\n";
 
			cout << "\n>> 如需退出请按q退出播放\n";
 
			while (video.isOpened())
			{
				// get frame from the video
				video >> frame;
 
				if (frame.empty())
				{
					break;
				}
 
				// update the tracking result
				//! [update]
				bool ok = multiTracker->update(frame);
				//! [update]
 
				//! [visualization]
				// draw the tracked object
				for (unsigned i = 0; i < multiTracker->getObjects().size(); i++)
				{
					rectangle(frame, multiTracker->getObjects()[i], colors[i], 2, 1);
				}
				//! [visualization]
 
				for (int i = 0; i < multiTracker->getObjects().size(); i++)
				{
					Point presentPoint;
					presentPoint.x = (multiTracker->getObjects()[i].x + (multiTracker->getObjects()[i].width*0.5)) * 2;
					presentPoint.y = (multiTracker->getObjects()[i].y + (multiTracker->getObjects()[i].height*0.5)) * 2;
					vecPoint[i].push_back(presentPoint);
				}
 
				if (vecPoint.size() > 1)
				{
					for (int j = 0; j < vecPoint.size(); j++)
					{
						for (int k = 1; k < vecPoint[j].size(); k++)
						{
							line(frame, vecPoint[j][k - 1], vecPoint[j][k], colors[j], 1, 8, 1);
						}
					}
				}
 
				// show image with the tracked object
				imshow("tracker", frame);
				//! [visualization]
 
				if (char(waitKey(30)) == 'q') {
					break;
				}
			}
 
			cv::destroyWindow("tracker");
			cout << "\n>> 播放完毕\n";
		}
		else
		{
			cout << "输入有误！";
		}
	}
	return 0;
}