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
	//Source raw image mat 
	Mat m_mSrcImg;
	//Proceed image mat
	Mat m_mProcImg;
	//Erro string buffer
	string m_sErrStr;

public:
	// Load Image Resource 
	bool LoadImgResource(string ImgSrcPath);
	// Convert img mat resolution to terminal show wnd.
	Mat FitImgMatWnd(Mat srcImg, int wndHeight,int wndWidth);
	// Convert mat to CImage
	BOOL Mat2CImg( CImage& cimgShow,bool bOrgImg );
	// Get Last Erro
	string GetLastErrStr(void);
	// Sobel Operation
	void ImgSobelOperate();
};

