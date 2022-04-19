//图片转视频
#include<opencv2\opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
    // 构造一个VideoWriter
    VideoWriter video("E:\\photo\\03\\test.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));
    // 从一个文件夹下读取多张jpg图片
    String pattern = "E:\\photo\\03\\*.jpg";
    vector<String> fn;

    glob(pattern, fn, false);

    size_t count = fn.size();
    for (size_t i = 0; i < count; i++)
    {
        Mat image = imread(fn[i]);
        // 这个大小与VideoWriter构造函数中的大小一致。
        resize(image, image, Size(640, 480));
        // 流操作符，把图片传入视频
        video << image;
    }
    cout << "处理完毕！" << endl;
    // 处理完之后会在得到一个名为test.avi的视频文件。
}
