//寻找轮廓
#include <iostream>  
#include<vector>
#include<algorithm>
#include <opencv2\opencv.hpp>  
#include <opencv2\highgui\highgui.hpp>  
using namespace std;
using namespace cv;


bool ascendSort(vector<Point> a, vector<Point> b) {
	return a.size() < b.size();
 
}

int main()
{
    Mat a=imread("E:\\photo\\03\\08\\02\\3.png");
    imshow("src",a);
    Mat a_cannyMat_output;
    Canny(a,a_cannyMat_output,150,100,3);
    vector<vector<Point>>g_vContours;       //所有轮廓
    vector<vector<Point>>contours2;         //  需要排除的轮廓
    vector<Point>tempV;     //  暂存的轮廓

    vector<Vec4i>g_vHierarchy;
    findContours(a_cannyMat_output,g_vContours,g_vHierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));
    Mat drawing=Mat::zeros(a_cannyMat_output.size(),CV_8UC3);

    sort(g_vContours.begin(),g_vContours.end(),ascendSort);     //对轮廓进行升序排序
    vector<vector<Point>>::iterator itc=g_vContours.begin();
    
    while(itc!=g_vContours.end())
    {
        Rect rect=boundingRect(*itc);
        int x=rect.x;
        int y=rect.y;
        int w=rect.width;
        int h=rect.height;
        // cout<<x<<" "<<y<<" "<<w<<" "<<h<<endl;

        if(itc->size()<20)                                     //将小于100的区域设置为全白
        {
            tempV.push_back(Point(x,y));
            tempV.push_back(Point(x, y+h));
			tempV.push_back(Point(x+w, y+h));
			tempV.push_back(Point(x+w, y));
            contours2.push_back(tempV);
            cv::drawContours(a,contours2,-1,Scalar(255,255,255),CV_FILLED);

        }
        tempV.clear();
        ++itc;

    }
    namedWindow("dst",0);
    imshow("dst",a);


    for(int i=0;i<g_vContours.size();i++)
    {
        Scalar color=Scalar(0,0,255);
        drawContours(drawing,g_vContours,i,color,2,8,g_vHierarchy,0,Point());
    }
    imshow("drawing",drawing);
    waitKey(0);
    return 0;
}
//将小区域删除掉
