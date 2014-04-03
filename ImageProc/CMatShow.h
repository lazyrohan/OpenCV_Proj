/////////////////////////////////////////////////////
//
//CMatShow class for output mat data as terminal show
//
////////////////////////////////////////////////////

#pragma once

//Include OpenCV header files
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
using namespace cv;
//C++standard header files
#include <stdio.h>
using namespace std;
//Include CIamge class
#include <atlimage.h>
class CMatShow
{
public:
	CMatShow();
	virtual ~CMatShow( );

private:
	Mat m_mSrcImg;


	// Load Image Resource 
	void LoadImgResource(string ImgSrcPath);
	public:
	// Convert img mat resolution to terminal show wnd.
	void FitImgMatWnd(Mat srcImg,Mat dstImg, int wndHeight,int wndWidth);
	// Convert mat to CImage
	bool Mat2CImg( Mat imgMat,CImage& cimgShow );
};

