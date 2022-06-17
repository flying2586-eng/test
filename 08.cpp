//视频转图片
#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;


int main()
{
    VideoCapture cap("E:\\xiayang\\20210731_03_08_113023131_G1.mp4");
    Mat frame;
    int i=0;
    while(cap.read(frame))
    {
        // frame=frame(Rect(500,0,1500,500));
        
        stringstream str;
        str<<"E:\\xiayang\\1\\"<<i<<".png";
        cout<<str.str()<<endl;
        imwrite(str.str(),frame);
        i++;
    }
    return 0;
}