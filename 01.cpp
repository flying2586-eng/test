#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
#include<math.h>
using namespace cv;
using namespace std;
int main()
{
   Mat a=imread("E:\\photo\\03\\08\\001\\0.png");
   a=a(Rect(243,0,1252,310));
   imshow("",a);
   waitKey(0);
   return 0;
}
