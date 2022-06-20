//检测边缘
#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
using namespace std;
using namespace cv;
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
    Mat a=imread("E:\\photo\\03\\08\\09\\0.png");
    cvtColor(a,a,COLOR_BGR2GRAY);
    threshold(a,a,100,255,THRESH_BINARY);

    Mat element=getStructuringElement(MORPH_RECT,Size(15,15));
    erode(a,a,element);
    // namedWindow("原图",0);
    // imshow("原图",a);
    //定义轮廓和层次结构
    vector<vector<Point>>contours;
    vector<Vec4i>hierarchy;
    Canny(a,a,100,200,3);
    findContours(a,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
    Mat dstImage=Mat::zeros(a.size(),CV_8UC3);
    int index=0;
    for(;index>=0;index=hierarchy[index][0])
    {
        Scalar color(0,0,255);
        drawContours(dstImage,contours,index,color,FILLED,8,hierarchy);
    }
    
    //找到矩形包围框
    
    vector<Rect>boundRect(contours.size());
    if(contours.size()<=3)
    {
    for(int i=0;i<contours.size();i++)
    {
        
        boundRect[i]=boundingRect(Mat(contours[i]));
    }
    }
    //画出框
    Point left_point;
    Point right_point;
    for(int i=0;i<contours.size();i++)
    {
        Scalar color(0,255,0);
        rectangle(dstImage,boundRect[i].tl(),boundRect[i].br(),color,2,8,0);
        double x_area;
        x_area=contourArea(contours[i]);
        double max_x_area;
        
        if(x_area>max_x_area)
        {
            max_x_area=x_area;
            left_point=boundRect[i].tl();
            right_point=boundRect[i].br();
        }
        
        cout<<boundRect[i].tl()<<" "<<boundRect[i].br()<<endl;
        
    }
    cout<<"最大轮廓左上角: "<<left_point<<" "<<"右下角: "<<right_point<<endl;
    
    namedWindow("轮廓图",0);
    // imshow("轮廓图",dstImage);
    // if(!contours.empty()&&!hierarchy.empty())
    // {
    //     vector<vector<Point>>::const_iterator itc=contours.begin();
    //     int j=1;
    //     static double maxarea=0;
    //     while(itc!=contours.end())
    //     {
    //         double area=contourArea(*itc);
    //         cout<<"第"<<j<<"个轮廓的面积为: "<<area<<endl;

            
    //         if(area>maxarea)
    //         {
    //             maxarea=area;
    //         }
            

    //         j++;
    //         itc++;

    //     }
    //     cout<<"最大面积: "<<maxarea<<endl;
    // }
    
    Ptr<Tracker>tracker=TrackerKCF::create();
    vector<vector<Point>>vecPoint;

    Mat frame;
    Rect rois(243+left_point.x,0+left_point.y,right_point.x-left_point.x,right_point.y-left_point.y);

    while(true)
    {
        vecPoint.clear();
        VideoCapture video("E:\\photo\\02.avi");
        if(!video.isOpened())
        {
            cout<<"\n>> 读取视频失败";
            return -1;
        }
        video>>frame;
        vector<Scalar>colors;
        getRandomColors(colors,1);
        Ptr<MultiTracker>multiTracker=MultiTracker::create();
       
        multiTracker->add(tracker,frame,Rect2d(rois));
        vector<Point>temp;
        vecPoint.push_back(temp);       
        while(video.isOpened())
        {
            video>>frame;
            if(frame.empty()){
                break;
            }
            bool ok=multiTracker->update(frame);
            for(unsigned i=0;i<multiTracker->getObjects().size();i++)
            {
                rectangle(frame,multiTracker->getObjects()[i],colors[i],2,1);
            }
            imshow("tracker",frame);
            if(char(waitKey(30))=='q')
            {
                break;
            }
        }
        cv::destroyWindow("tracker");
    }

    // waitKey(0);
    return 0;
}