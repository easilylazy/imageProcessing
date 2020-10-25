#include "thresholdSegmentation.h"
//transfer rgb image to grayImg im
void thresholdSegmentation::rgb2gray()
{
	int width = rgbImg.cols;
	int height = rgbImg.rows;

	// make use of array 
	Mat b, g, r, pb;
	Mat blue_channel, green_channel, red_channel;
	vector<cv::Mat> v_channel;
	split(rgbImg, v_channel);
	blue_channel = v_channel.at(0);
	green_channel = v_channel.at(1);
	red_channel = v_channel.at(2);
	grayImg= (0.114 * blue_channel+0.587 * green_channel+ 0.299 * red_channel);
	//imshow("add", grayImg);
	//cout << grayImg.channels();	
}
void thresholdSegmentation::otsuGt()
{
	//统计直方图
	countHistogram();

	//choose T to saparate it into C0 and C1
	//
	calVariance();


}
//binarize a picture's pixels based on threshold
void thresholdSegmentation::binarize()
{

	int maxi, mini;
	maxi = 100;
	mini = 100;
	//visit each pixel to read its value
	for (int i = 0; i < grayImg.rows; ++i)
	{
		//cout << grayImg.rows<<" "<<rgbImg.cols;
		
		//获取第 i 行首像素指针
		uchar* p = grayImg.ptr<uchar>(i);// Vec3b > (i);
		uchar* r = resImg.ptr<uchar>(i);// Vec3b > (i);
		for (int j = 0; j < grayImg.cols; ++j)
		{
			//cout << i << " " << j ;
			//cout << (int)p[j] << endl;
			if ((int)p[j] > maxi)
			{
				maxi = (int)p[j];
			}
			if ((int)p[j] < mini)
			{
				mini = (int)p[j];
			}
			if ((int)p[j] > Threshold)
			{
				r[j] = 255;
			}
			else
			{
				r[j] = 0;
			}
			//cout << p[j][0];
			//p[j][0] = i % 255; //Blue
			//p[j][1] = j % 255; //Green
			//p[j][2] = 0; //Red
		}
	}
	//cout << mini << " " << maxi << endl;
}

void thresholdSegmentation::countHistogram()
{
	//visit each pixel to read its value
	for (int i = 0; i < grayImg.rows; ++i)
	{

		uchar* p = grayImg.ptr<uchar>(i);// Vec3b > (i);
		for (int j = 0; j < grayImg.cols; ++j)
		{
			Histogram[(int)p[j]] += 1;
			
			
		}
	}

	for (int T = 0; T < 256; T++)
	{
		//cout << Histogram[T] << endl;
		this->P[T] = (double)Histogram[T] / (double)(grayImg.rows * grayImg.cols);
		//cout << P[T] << endl;
		if (T == 0)
		{
			sigmaPi0[T] = P[T] * T;
			Omiga0[T] = P[T];
		}
		else
		{
			sigmaPi0[T] = sigmaPi0[T - 1] + P[T] * T;
			Omiga0[T] = Omiga0[T - 1] + P[T];
		}
		Miu0[T] = sigmaPi0[T] / Omiga0[T];

		Omiga1[T] = 1 - Omiga0[T];

	}

	for (int T = 0; T < 256; T++)
	{
		sigmaPi1[T] = sigmaPi0[255] - sigmaPi0[T];
		Miu1[T] = sigmaPi1[T] / Omiga1[T];
		SigmaB[T] = Omiga0[T] * Omiga1[T] * pow(Miu0[T] - Miu1[T], 2);
	}




}

void thresholdSegmentation::calVariance()
{
	double maxYita = 0;
	for (int T = 0; T < L; T++)
	{
		Sigma0[T] = 0;
		Sigma1[T] = 0;

		for (int i = 0; i < 256; i++)
		{


			if (i <= T)
			{
				Sigma0[T] += pow((i - Miu0[T]), 2) * P[i] / Omiga0[T];


			}
			else
			{
				Sigma1[T] += pow((i - Miu1[T]), 2) * P[i] / Omiga1[T];

			}


		}
		SigmaW[T] = Sigma0[T] * Omiga0[T] + Sigma1[T] * Omiga1[T];
		SigmaT[T] = SigmaB[T] + SigmaW[T];
		Yita[T] = SigmaB[T] / SigmaT[T];
		if (Yita[T]>maxYita)
		{
			maxYita = Yita[T];
			Threshold = T;
			cout << "maxYita: "<<maxYita << endl;
			cout << "Threshold: "<<Threshold << endl;
			
		}
	}
	
}
