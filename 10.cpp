//判断两张照片的匹配问题 SURF
#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
#include<iostream>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;
int main()
{
    Mat src=imread("E:\\photo\\03\\45.jpg",IMREAD_COLOR);
    Mat temp=imread("E:\\photo\\03\\46.jpg",IMREAD_COLOR);
    namedWindow("1",0);
    imshow("1",src);
    namedWindow("2",0);
    imshow("2",temp);

    //SURF特征点检测
    int minHessian = 400;
    Ptr<SURF> detector=SURF::create(minHessian,4,3,true,true);
    vector<KeyPoint>keypoints1,keypoints2;
    Mat src_vector,temp_vector;
    detector->detectAndCompute(src,Mat(),keypoints1,src_vector);
    detector->detectAndCompute(temp,Mat(),keypoints2,temp_vector);

    //FLANN匹配
    FlannBasedMatcher matcher;

    vector<DMatch> matches;

    matcher.match(src_vector,temp_vector,matches);

    //求最小最大距离
    double minDistance=1000;
    double maxDistance=0;
    for(int i=0;i<src_vector.rows;i++){
        double distance=matches[i].distance;
        if(distance>maxDistance){
            maxDistance=distance;
        }
        if(distance<minDistance){
            minDistance=distance;
        }
    }
    printf("max distance : %f\n",maxDistance);
    printf("min distance : %f\n",minDistance);

    sort(matches.begin(),matches.end());
    vector<DMatch>good_matches;
    int ptsPairs=std::min(300,(int)(matches.size()*0.15));//匹配点数量不大于50
    cout<<ptsPairs<<endl;
    for(int i=0;i<ptsPairs;i++){
        good_matches.push_back(matches[i]);
    }

    Mat MatchesImage;
    drawMatches(src,keypoints1,temp,keypoints2,good_matches,MatchesImage,Scalar::all(-1),Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    namedWindow("Flann Image",0);
    imshow("Flann Image",MatchesImage);
    // imwrite("E:\\photo\\03\\tt2.jpg",MatchesImage);

    waitKey(0);
    return 0;

}