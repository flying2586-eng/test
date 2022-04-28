//确定ROI区域

#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int a;      //a,b左角点
int a_max;
int b;
int b_max;
int c;      //c,行 d，列
int c_max;
int d;
int d_max;
Mat image;
Mat imageROI;
void on_Trackbar(int,void*)
{
    
    imageROI=image(Rect(a,b,c,d));
    imshow("结果",imageROI);
}


int main()
{
    image = imread("E:\\photo\\03\\07\\0.jpg");
    namedWindow("原图",0);
    imshow("原图",image);
    cout<<"原图行: "<<image.rows<<endl;
    cout<<"原图列: "<<image.cols<<endl;


    if(!image.data){cout<<"no picture!"<<endl;}

    a=0;
    b=0;
    c=10;
    d=10;       //设置初值

    namedWindow("结果",0);

    char TrackbarName[50];
    sprintf(TrackbarName,"左角点x %d",image.cols);
    createTrackbar(TrackbarName,"结果",&a,image.cols,on_Trackbar);
    on_Trackbar(a,0);

    char TrackbarName1[50];
    sprintf(TrackbarName1,"左角点y %d",image.rows);
    createTrackbar(TrackbarName1,"结果",&b,image.rows,on_Trackbar);
    on_Trackbar(b,0);

    char TrackbarName2[50];
    sprintf(TrackbarName2,"列 %d",image.cols-a);
    createTrackbar(TrackbarName2,"结果",&c,image.cols-a,on_Trackbar);
    on_Trackbar(c,0);

    char TrackbarName3[50];
    sprintf(TrackbarName3,"行 %d",image.rows-b);
    createTrackbar(TrackbarName3,"结果",&d,image.rows-b,on_Trackbar);
    on_Trackbar(d,0);

   

    waitKey(0);

    return 0;


    
}