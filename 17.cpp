//固定ROI区域
#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat a=imread("E:\\photo\\Videos\\run\\lyova_run\\0.jpg");
    Mat b=a(Rect(133,44,25,70));
    namedWindow("a",0);
    namedWindow("b",0);
    imshow("a",a);
    imshow("b",b);
    waitKey(0);
    return 0;
}