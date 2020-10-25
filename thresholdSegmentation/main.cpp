#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>

#include "thresholdSegmentation.h"



int main()
{
	Mat img_rgb;

	img_rgb = imread("pic/otsu2.jpg");//traffic.jpg");// 
	thresholdSegmentation threshSeg(img_rgb, 90);//arbitrary set T=90
	threshSeg.rgb2gray();
	threshSeg.binarize();
	imshow("resBefore", threshSeg.getResImg());

	threshSeg.otsuGt();
	threshSeg.binarize();

	imshow("gray", threshSeg.getGrayImg());
	imshow("resAfter", threshSeg.getResImg());

	waitKey(0);

	return 0;
	
}