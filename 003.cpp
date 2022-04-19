#include<opencv2\opencv.hpp>
using namespace cv;
// using namespace std;
int main()
{
	VideoCapture capture("01.mp4");

	while (1)
	{
		Mat frame;
		capture >> frame;
		imshow("01", frame);
		waitKey(30);
	}
	return 0;
}