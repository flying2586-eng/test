#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
using namespace cv;

int main()
{
    Mat img=imread("E:/photo/93a7b4064c30eb6ec86374d07440939e.png");
    namedWindow("测试",0);
    imshow("测试",img);
    waitKey(0);
    return 0;
}
