#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
void imfill(Mat srcimage, Mat &dstimage)
{
	Size m_Size = srcimage.size();
	Mat temimage = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcimage.type());//延展图像      
																					//imshow("temimage", temimage);  
	srcimage.copyTo(temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));
	floodFill(temimage, Point(0, 0), Scalar(255));
	//imshow("temimage", temimage);  
	//waitKey(0);  
	Mat cutImg;//裁剪延展的图像   
	temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);
	dstimage = srcimage | (~cutImg);
}
int main() {
	Mat src = cv::imread("screen.png");
	Mat dst,dst1,gray;
	//medianBlur(src, src, 7);
	//GaussianBlur(src, src, Size(7, 7), 0, 0);
	//灰度图转换
	cvtColor(src, src, COLOR_BGR2GRAY);
	
	//GaussianBlur(src, src, Size(11, 11), 0, 0);
	//线性变换
	int hight = src.rows;
	int width = src.cols;
	int i = 0, j = 0;
	int p_min = 255;
	int p_max = 0;
	//equalizeHist(src, src);
	//imshow("eq", src);
	for (i = 0; i < hight; i++)
	{
		for (j = 0; j < width; j++)
		{
			uchar*data = src.ptr<uchar>(i);
			if (p_min > data[j])
			{
				p_min = data[j];
			}
			if (p_max < data[j])
			{
				p_max = data[j];
			}
		}
	}
	double p_prop = 1.0;
	p_prop = 255.0 / (p_max - p_min);
	for (i = 0; i < hight; i++)
	{
		for (j = 0; j < width; j++)
		{
			uchar*data = src.ptr<uchar>(i);
			data[j] = int((data[j] - p_min) * p_prop) ;
		}
	}
	imshow("linear", src);
	gray = src;
	//GaussianBlur(src, src, Size(11, 11), 0, 0);
	//均值滤波、高斯滤波
	blur(src, src, Size(7 ,7));
	GaussianBlur(src, src, Size(11, 11), 0, 0);
	blur(src, src, Size(7, 7));
	GaussianBlur(src, src, Size(11, 11), 0, 0);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(13, 13));
	//闭运算
	dilate(src, dst, element);
	erode(dst, dst, element);
	
	//imshow("ero", dst);
	/*Rect ccomp;
	floodFill(dst, Point(10, 10), Scalar(50, 50, 50), &ccomp, Scalar(1, 1, 1), Scalar(1, 1, 1));
	floodFill(dst, Point(700, 700), Scalar(50, 50, 50), &ccomp, Scalar(1, 1, 1), Scalar(1, 1, 1));
	imshow("dst", dst);*/
	//applyColorMap(dst, dst,COLORMAP_RAINBOW);
	imshow("closed", dst);
	//medianBlur(dst, dst, 7);
	//imshow("GausBlur", dst);
	/*Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Sobel(dst, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	Sobel(dst, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);*/
	//Rect ccomp;
	//floodFill(dst, Point(10, 10), Scalar(0, 255, 0), &ccomp, Scalar(8, 8, 8), Scalar(8, 8, 8));
	//cvtColor(dst, dst, COLOR_BGR2GRAY);
	//GaussianBlur(src, src, Size(11, 11), 0, 0);
	//Candy边缘检测
	Mat element_s = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	Canny(dst, dst, 6, 9, 3);
	//腐蚀
	dilate(dst, dst, element_s);
	//闭合区域填充
	imfill(dst, dst);
	//dilate(dst, dst, element);
	//imfill(dst, gray);
	//imshow("dst", dst);
	//imshow("res", dst);
	//applyColorMap(dst, dst,COLORMAP_WINTER);
	imshow("fill", dst);
	//与原灰度图融合
	for (i = 0; i < hight; i++)
	{
		for (j = 0; j < width; j++)
		{
			uchar*data = dst.ptr<uchar>(i);
			uchar*data_s = gray.ptr<uchar>(i);
			data[j] =  148 + data[j]* data_s[j];
		}
	}
	//伪彩色变换
	applyColorMap(dst, dst, COLORMAP_RAINBOW);
	//腐蚀
	dilate(dst, dst, element_s);
	//erode(dst, dst, element_s);
	imshow("result", dst);
	imwrite("reslut.png",dst);
	cv::waitKey();
	return 0;
}
