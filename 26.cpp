//聚类
#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat img(500, 500, CV_8UC3);  //定义一张500*500的8位3通道图片
	RNG rng(12345);
	Scalar colorTab[] = {    //定义颜色数组
		Scalar(0,0,255),
		Scalar(255,0,255),
		Scalar(0,255,0),
		Scalar(255,0,0),
		Scalar(0,255,255),
	};
	int numCluster = rng.uniform(2, 5);  //4个分类
	printf("number of cluster:%d\n", numCluster);  //打印4个分类
	cout << "-----------------------------------" << endl;
	int sampleCount = rng.uniform(1, 1000); //1000个随机点
	Mat points(sampleCount, 1, CV_32FC2); //一行2列（x,y）

	Mat labels;
	Mat centers;
	
	//产生多高斯部分的随机采样点
	for (int k = 0; k < numCluster; k++)
	{
		Point center;
		center.x = rng.uniform(0, img.cols);
		center.y = rng.uniform(0, img.rows);
		
		cout << "x=" << center.x <<"  "<< "y=" << center.y << endl;
		Mat pointChunk = points.rowRange(k*sampleCount / numCluster,
			k == numCluster - 1 ? 
			sampleCount: (k + 1)*sampleCount / numCluster); //定义随机散点
		//rng.fill函数，会以center点为中心，产生高斯分布的随机点(位置点），并把位置点保存在矩阵pointChunk中。
		rng.fill(pointChunk, RNG::NORMAL, Scalar(center.x, center.y), Scalar(img.cols*0.05, img.rows*0.05));
	}
	/*
	rowRange(int x, int y):
	(其中y应小于等于行数，例如一个矩阵最大为5行，那么y最大为4) 的创建矩阵范围
	为从x行为首行开始，往后数y-x行。
	*/
	randShuffle(points, 1, &rng);//打乱points中值，第二个参数表示随机交换元素的数量的缩放因子，总的交换次数dst.rows*dst.cols*iterFactor，第三个参数是个随机发生器，决定选哪两个元素交换。

	//使用KMeans
	/*
	labels中放的是执行kmeans算法后sample中簇的索引,KMEANS_PP_CENTERS 算法决定簇的初始中心，否则就是采用随机值的方法决定初始中心。
	如果flags是CV_KMEANS_USE_INITIAL_LABELS，则需要初始化labels,就是初始指定点的分类。
	*/
	kmeans(points, numCluster, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1),2,KMEANS_PP_CENTERS,centers);

	//用不同颜色显示分类
	img = Scalar::all(255);  //把图的背景变成白色
	for (int i = 0; i < sampleCount; i++)
	{
		int index = labels.at<int>(i);  //获取每个点的索引
		Point p = points.at<Point2f>(i);
		circle(img, p, 2, colorTab[index], -1, 8, 0);

	}
	//绘制每个聚类中心
	for (int i =0 ; i < centers.rows; i++)
	{
		int x = centers.at<float>(i, 0);
		int y = centers.at<float>(i, 1);
		circle(img, Point(x, y), 50, colorTab[i], 2, 8, 0);

	}
	imshow("result", img);
	waitKey(0);
	return 0;
}



