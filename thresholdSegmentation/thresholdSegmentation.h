#include "opencv.hpp"
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp> 
using namespace cv;
using namespace std;



#pragma once
class thresholdSegmentation
{
private:
	Mat rgbImg;
	Mat grayImg;
	Mat resImg;
	int Threshold;
	int Histogram[256] = {0};
	int L;
	double P[256];
	double Omiga0[256];//sigma p from 0 to T
	double Omiga1[256];//sigma p from T to L-1(=1-Omiga0[T])

	double sigmaPi0[256];//sigma P*i from 0 to T
	double sigmaPi1[256];//sigma P*i from T to L-1(=sigmaPi0[255]-sigmaPi0[T])
	double Miu0[256];//mean i from 0 to T
	double Miu1[256];//mean i from T to L-1

	double Sigma0[256];//variance of C0
	double Sigma1[256];//variance of C1
	double SigmaW[256];//variance in C
	double SigmaB[256];//variance between C
	double SigmaT[256];//variance of both
	double Yita[256];//variance of both
public:
	thresholdSegmentation(Mat img,Mat gray, int T)
	{
		rgbImg = img;
		grayImg = gray;     //³õÊ¼»¯»Ò¶ÈÍ¼Ïñ¾ØÕó

		resImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));     //³õÊ¼»¯Í¼Ïñ¾ØÕó
		Threshold = T;
		L = 256;
	}

	thresholdSegmentation(Mat img, int T)
	{
		rgbImg = img;
		grayImg = Mat(img.rows,img.cols, CV_8UC1, Scalar(0));     //³õÊ¼»¯»Ò¶ÈÍ¼Ïñ¾ØÕó

		resImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));     //³õÊ¼»¯Í¼Ïñ¾ØÕó
		Threshold = T;
		L = 256;

		//Histogram = { 0 };

		//imshow("img", rgbImg);
		//waitKey(0);
	}
	void rgb2gray();
	void otsuGt();
	void binarize();
	void countHistogram();
	void calVariance();
	Mat getGrayImg()
	{
		return grayImg;
	}
	Mat getResImg()
	{
		return resImg;
	}
	


};

