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
int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "car1.png";
	Mat src = imread(filename,0);
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

	Mat Ix, Iy, Ixx, Iyy, Ixy, Cim;
	Ix.create(Size(src.cols,src.rows),src.type());
	Iy.create(Size(src.cols,src.rows),src.type());
	Ixx.create(Size(src.cols,src.rows),src.type());
	Iyy.create(Size(src.cols,src.rows),src.type());
	Ixy.create(Size(src.cols,src.rows),src.type());
	Cim.create(Size(src.cols,src.rows),src.type());
	Sobel( src, Ix, src.depth(), 1, 0, 1 );
	Sobel( src, Iy, src.depth(), 0, 1, 1 );
	Ixx = Ix.mul(Ix);//¾ØÕóµã³Ë
	Iyy = Iy.mul(Iy);
	Ixy = Ix.mul(Iy);

	GaussianBlur(Ixx,Ixx,Size(3,3),2);
	GaussianBlur(Iyy,Iyy,Size(3,3),2);
	GaussianBlur(Ixy,Ixy,Size(3,3),2);

	Cim = (Ixx.mul(Ixx) + Iyy.mul(Iyy) )/(Ixx + Iyy + 1e-4);
	
	int radius = 1;
	int sze = 2*radius +1;
	//mx = 

	//sze = 2*radius+1;                   % Size of mask.
	//	mx = ordfilt2(cim,sze^2,ones(sze)); % Grey-scale dilate.
	//	cim = (cim==mx)&(cim>thresh);       % Find maxima.

	//	[r,c] = find(cim);                  % Find row,col coords.
	cout<<Cim<<endl;
	imshow("dst",Cim);
	waitKey(0);
	return 0;
}