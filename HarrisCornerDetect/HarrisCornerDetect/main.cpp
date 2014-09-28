#include <iostream>
#include <math.h>
#include "../3rdparty/include/opencv.hpp"

using namespace std;
using namespace cv;
#pragma comment(lib, "opencv_highgui249d.lib")
#pragma comment(lib, "opencv_imgproc249d.lib")
#pragma comment(lib, "opencv_core249d.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
#pragma comment(lib, "opencv_core249.lib")

int maximumValue(Mat src , Rect rect)
{
	int maximum = 0;
	int start1 = rect.x<(src.rows - rect.width )?rect.x:(src.rows - rect.width );
	int end1 = (rect.x + rect.width)< src.rows?(rect.x + rect.width):src.rows;
	int start2 = rect.y<(src.cols - rect.height )?rect.y:(src.cols - rect.height );
	int end2 = (rect.y + rect.height)< src.cols?(rect.y + rect.height):src.cols;
	for(int i = start1; i< end1; i++)
		for (int j = start2; j< end2; j++)
		{
			if (maximum < src.at<uchar>(i,j))
			{
				maximum = src.at<uchar>(i,j);
			}
		}
		return maximum;
}
int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "hello1.jpg";
	Mat src = imread(filename,1);
	if(src.empty())
	{
		cout << "can not open " << filename << endl;
		return -1;
	}
	Mat dst, cdst;
	namedWindow("hello1");
	imshow("hello1",src);
	int width = src.cols;
	int height = src.rows;
	src.copyTo(dst);
	cvtColor(src,src,COLOR_BGR2GRAY);

	//for (int row = 1; row < height - 1; row++)
	//{
	//	for (int col = 1; col < width - 1; col++)
	//	{
	//		if (src.channels() == 1)
	//		{

	//			int center = src.at<uchar>(row,col);
	//			int up = src.at<uchar>(row-1,col);
	//			int down = src.at<uchar>(row+1,col);
	//			int left = src.at<uchar>(row,col-1);
	//			int right = src.at<uchar>(row,col+1);
	//			int Ey = center - down;
	//			int Ex = center - right;
	//			int Exx = left + right - 2*center;
	//			int Eyy = up + down - 2*center;
	//		}
	//		else
	//		{
	//			src.at<Vec3b>(row,col)[2];
	//			src.at<Vec3b>(row,col)[1];
	//			src.at<Vec3b>(row,col)[0];
	//		}
	//	}
	//}

	Mat Ix, Iy, Ixx, Iyy, Ixy, Cim,Mx;
	Ix.create(Size(src.cols,src.rows),CV_32FC1 );
	Iy.create(Size(src.cols,src.rows),CV_32FC1);
	Ixx.create(Size(src.cols,src.rows),CV_32FC1);
	Iyy.create(Size(src.cols,src.rows),CV_32FC1);
	Ixy.create(Size(src.cols,src.rows),CV_32FC1);
	Cim.create(Size(src.cols,src.rows),CV_32FC1);
	Mx.create(Size(src.cols,src.rows),CV_32FC1);
	Sobel( src, Ix, src.depth(), 1, 0, 1 );
	Sobel( src, Iy, src.depth(), 0, 1, 1 );
	Ixx = Ix.mul(Ix);//矩阵点乘
	Iyy = Iy.mul(Iy);
	Ixy = Ix.mul(Iy);

	GaussianBlur(Ixx,Ixx,Size(3,3),1);
	GaussianBlur(Iyy,Iyy,Size(3,3),1);
	GaussianBlur(Ixy,Ixy,Size(3,3),1);

	Cim = (Ixx.mul(Ixx) + Iyy.mul(Iyy) )/(Ixx + Iyy + 1e-4);
	
	int radius = 4;
	int thresh = 10;
	int sze = 2*radius +1;

	for (int i = 0; i< Cim.rows ; i++)
	{
		for (int j = 0 ; j < Cim.cols ; j++)
		{
			Rect rect ;
			rect.width = sze;
			rect.height = sze;
			rect.x = i;
			rect.y = j;
			Mx.at<float>(i,j) = maximumValue(Cim,rect);
			//printf("%d,%d ",Cim.at<uchar>(i,j),Mx.at<uchar>(i,j));
		}
		//printf("\n");
	}
	//imshow("Mx",Mx - Cim);

	//Mx - Cim 如果为0表示相等，不为0表示不相等
	// Cim > thresh ,结果如果是255表示大于阈值，如果为0 表示小于阈值
	//如果将 第一个结果 和 （第二个结果的取反） 进行 或 运算，结果表示，为0 的即是同时满足(cim==mx)&(cim>thresh)条件的，不为0的肯定不满足。
	//Mat temp = (Mx - Cim)|(~(Cim> thresh));
	////imshow("temp",~temp);
	//temp = ~temp;
	//cout<<temp<<endl;
	for (int i = 0 ;i< Cim.rows ;i++)
	{
		for (int j = 0; j< Cim.cols; j++)
		{
			//cout <<Mx<<endl;
			
			printf("%f \n",Cim.at<float>(i,j));
			printf("%f \n",Mx.at<float>(i,j));
			if (Cim.at<float>(i,j) == Mx.at<float>(i,j) && Cim.at<float>(i,j)> thresh)
			{
				circle(dst,Point(j,i),2,Scalar(0,0,255),1);
			}
			//if (Cim.at<float>(i,j) > 100)
			//{
			//	//cout <<(int)temp.at<uchar>(i,j) <<endl;
			//	
			//	//imshow("dst",dst);
			//	//waitKey(0);
			//}
		}
		cout <<endl;
	}
	//mx = 

	//sze = 2*radius+1;                   % Size of mask.
	//	mx = ordfilt2(cim,sze^2,ones(sze)); % Grey-scale dilate. maximum filter
	//	cim = (cim==mx)&(cim>thresh);       % Find maxima.

	//	[r,c] = find(cim);                  % Find row,col coords.
	//cout<<Mx<<endl;
	imshow("dst",dst);
	waitKey(0);
	return 0;
}