//自适应二值化

#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
    int i=0;

    //Mat image = imread("E:\\photo\\03\\07\\4.jpg");
    VideoCapture cap("E:\\photo\\20200319_Trim.avi");
    Mat image;
    while(cap.read(image))
    {
    Mat imageROI=image(Rect(243,0,1252,610));
    cvtColor(imageROI,imageROI,CV_BGR2GRAY);
    namedWindow("gray",0);
    imshow("gray",imageROI);
  /*   stringstream str1;
    str1<<"E:\\photo\\03\\08\\01\\"<<i<<".png";
    cout<<str1.str()<<endl;
    imwrite(str1.str(),imageROI); */

    Mat imageROI_1;
    adaptiveThreshold(imageROI,imageROI_1,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,3);
    // threshold(imageROI,imageROI,90,255,THRESH_BINARY);
    /* stringstream str2;
    str2<<"E:\\photo\\03\\08\\02\\"<<i<<".png";
    cout<<str2.str()<<endl;
    imwrite(str2.str(),imageROI_1);
    i++; */

    namedWindow("结果",0);
    imshow("结果",imageROI_1);
    waitKey(10);
    }
    return 0;
}