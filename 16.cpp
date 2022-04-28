#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("E:\\photo\\03\\07\\4.jpg");
    Mat imageROI=image(Rect(243,0,1252,610));
    cvtColor(imageROI,imageROI,CV_BGR2GRAY);
    namedWindow("gray",0);
    imshow("gray",imageROI);

    adaptiveThreshold(imageROI,imageROI,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,3);
    // threshold(imageROI,imageROI,90,255,THRESH_BINARY);

    namedWindow("结果",0);
    imshow("结果",imageROI);
    waitKey(0);
    return 0;
}