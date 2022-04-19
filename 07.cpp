//光流法
   #include <opencv2/video/video.hpp>
   #include <opencv2/highgui/highgui.hpp>
   #include <opencv2/imgproc/imgproc.hpp>
   #include <opencv2/core/core.hpp>
   #include <iostream>
   #include <cstdio>
  using namespace std;
 using namespace cv;
  //  描述：声明全局函数
  void tracking(Mat &frame, Mat &output);
  bool addNewPoints();
  bool acceptTrackedPoint(int i);
  //  描述：声明全局变量
  string window_name = "optical flow tracking";
  Mat gray;   // 当前图片
  Mat gray_prev;  // 预测图片
  vector<Point2f> points[2];  // point0为特征点的原来位置，point1为特征点的新位置
  vector<Point2f> initial;    // 初始化跟踪点的位置
  vector<Point2f> features;   // 检测的特征
  int maxCount = 500; // 检测的最大特征数
  double qLevel = 0.01;   // 特征检测的等级
  double minDist = 10.0;  // 两特征点之间的最小距离
  vector<uchar> status;   // 跟踪特征的状态，特征的流发现为1，否则为0
  vector<float> err;
  int i;
  int main()
  {
      Mat frame;
      Mat result;
      //加载使用的视频文件，放在项目程序运行文件下
      VideoCapture capture("E:\\photo\\03\\test.avi");
    
      // 摄像头读取文件开关
     if(capture.isOpened())  
      {
          while(true)
          {
              capture >> frame;
   
              if(!frame.empty())
              { 
                  tracking(frame, result);
              }
              else
              { 
                  printf(" --(!) No captured frame -- Break!");
                  break;
             }
              int c = waitKey(500);
              if( (char)c == 27 )
              {
                  break; 
              } 
         }
     }
     return 0;
  }
   
  // parameter: frame 输入的视频帧
  //            output 有跟踪结果的视频帧
  void tracking(Mat &frame, Mat &output)
  {
      cvtColor(frame, gray, CV_BGR2GRAY);
      frame.copyTo(output);

        // Mat imageROI;
	// imageROI = frame(Rect(500, 0, 1000, 500));
	/* namedWindow("1", 0);
	imshow("1", imageROI); */
    // imageROI.copyTo(output);

	/* Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat imageROI_1; */
	

	//  Mat imageROI_2;
     /*
	 dilate(imageROI,imageROI_1,kernel_dilate);
	 namedWindow("膨胀",0);
	 imshow("膨胀",imageROI_1); 

	 erode(imageROI_1, imageROI_2, kernel_erode);
	 namedWindow("腐蚀", 0);
	 imshow("腐蚀", imageROI_2); */
//二值化
  
    //cvtColor(imageROI,gray,CV_BGR2GRAY);
    // threshold(imageROI_2,gray,150,255,CV_THRESH_BINARY);



      // 添加特征点
      if (addNewPoints())
      {
          goodFeaturesToTrack(gray, features, maxCount, qLevel, minDist);
          points[0].insert(points[0].end(), features.begin(), features.end());
          initial.insert(initial.end(), features.begin(), features.end());
      }
  
     if (gray_prev.empty())
      {
          gray.copyTo(gray_prev);
      }
     // l-k光流法运动估计
     calcOpticalFlowPyrLK(gray_prev, gray, points[0], points[1], status, err);
      // 去掉一些不好的特征点
      int k = 0;
      for (size_t i=0; i<points[1].size(); i++)
     {
         if (acceptTrackedPoint(i))
         {
             initial[k] = initial[i];
             points[1][k++] = points[1][i];
          }
    }
     points[1].resize(k);
     initial.resize(k);
     // 显示特征点和运动轨迹     
     for (size_t i=0; i<points[1].size(); i++)
     {
         line(output, initial[i], points[1][i], Scalar(0, 0, 255));
         circle(output, points[1][i], 3, Scalar(0, 255, 0), -1);
     }
  
     // 把当前跟踪结果作为下一次参考
    swap(points[1], points[0]);
    swap(gray_prev, gray);  
    imshow(window_name, output);
    /*************新增****************/
    stringstream str;
    str<<"E:\\photo\\04\\"<<i<<".jpg";
    cout<<str.str()<<endl;
    imwrite(str.str(),output);
    i++;
 }
  
 //  检测新点是否应该被添加
 // return: 是否被添加标志
 bool addNewPoints()
 {
     return points[0].size() <= 10;
 }
  
 //决定哪些跟踪点被接受
 bool acceptTrackedPoint(int i)
 {
    return status[i] && ((abs(points[0][i].x - points[1][i].x) + abs(points[0][i].y - points[1][i].y)) > 2);
 }