//视频转图片
#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;


int main()
{
    VideoCapture cap("E:\\photo\\Videos\\run\\lyova_run.avi");
    Mat frame;
    int i=0;
    while(cap.read(frame))
    {
        // frame=frame(Rect(500,0,1500,500));
        stringstream str;
        str<<"E:\\photo\\Videos\\run\\lyova_run\\"<<i<<".jpg";
        cout<<str.str()<<endl;
        imwrite(str.str(),frame);
        i++;
    }
    return 0;
}