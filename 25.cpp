#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
 
using namespace cv;
using namespace std;


void getRandomColors(vector<Scalar> &colors, int numColors)//随机获取一种颜色
{
	RNG rng(0);//创造随机数
	for (int i = 0; i < numColors; i++)
	{
		colors.push_back(Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));//RNG::uniform(a, b ) 返回一个[a,b)范围的均匀分布的随机数，a,b的数据类型要一致，而且必须是int、float、double中的一种，默认是int。
	}
}

int main()
{
    Ptr<Tracker> tracker=TrackerKCF::create();
    vector<vector<Point>> vecPoint;
 
	Mat frame;
	vector<Rect> rois;
	

    while(true)
    {
        vecPoint.clear();
        VideoCapture video("E:\\photo\\20200319_Trim.avi");
        if(!video.isOpened())
        {
            cout<<"\n>> 读取视频失败";
            return -1;
        }
        video>>frame;
        

        cv::selectROIs("tracker",frame,rois,false);
        if(rois.size()<1)
        {
            return 0;
        }
        vector<Scalar>colors;
        getRandomColors(colors,rois.size());

        Ptr<MultiTracker>multiTracker=MultiTracker::create();
        for(int i=0;i<rois.size();i++)
        {
            multiTracker->add(tracker,frame,Rect2d(rois[i]));
            vector<Point>temp;
            vecPoint.push_back(temp);       //将temp加在vecPoint列表最后面
        }
        while(video.isOpened())
        {
            video>>frame;
            if(frame.empty())
            {
                break;
            }
            bool ok=multiTracker->update(frame);
            for(unsigned i=0;i<multiTracker->getObjects().size();i++)
            {
                rectangle(frame,multiTracker->getObjects()[i],colors[i],2,1);
            }
            /* for (int i = 0; i < multiTracker->getObjects().size(); i++)
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
			} */
            imshow("tracker",frame);
            if(char(waitKey(30))=='q')
            {
                break;
            }
        }
        cv::destroyWindow("tracker");


    }
    return 0;
}