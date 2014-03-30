#pragma once

//Include OpenCV header files
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
using namespace cv;


//Standard c++ headers
#include <iostream>
#include <iomanip>
//#include <fstream>
#include <string>
#include <vector>
#include <sstream>
//For time caculate
//#include <ctime>
using namespace std;

// CImgShowWnd

class CImgShowWnd : public CWnd
{
	DECLARE_DYNAMIC(CImgShowWnd)

public:
	CImgShowWnd();
	virtual ~CImgShowWnd();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CImage m_iImage;
	// Copy Mat image data to CImage for show.
	BOOL MatToCImage(Mat& imgMat, CImage& cimgShow);
public:
	// Update image data to showout.
	void UpdateImage(Mat& imgMat);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// Soble Operate to get image edge.
	void ImgSobelOperate(Mat& imgMat, Mat& outImg);

	//Common tools 
	//A variable to string function
	template <class T> string Var2Str(T varName)
	{
		stringstream ss;
		ss << varName;
		return ss.str();
	};

	//A little timer
	//Timer start
	void Tmstat(time_t& time)
	{
		time = clock();
	};
	//Timer end and return time cost by seconds
	float Tmendt(time_t starttime)
	{
		return ((clock() - starttime) / float(CLOCKS_PER_SEC));
	};

	/*
	//Log to file function
	void Log2File(string funcName, string funcInfo, bool logFlag)
	{
		if (logFlag)
		{
			ofstream ofoper;
			ofoper.open("log.txt", ios_base::out | ios_base::app);

			//CPP time function to get time
			tm *loctime = new tm;
			//Get calender time
			time_t nowtime = time(NULL);
			//Get local time
			localtime_s(loctime, &nowtime);

			//Output hh:mm:ss YY/MM/DD header
			ofoper << setfill('0') << setw(2) << loctime->tm_hour << ':' << setw(2) << loctime->tm_min << ':' << setw(2) << loctime->tm_sec << "  "    //hh:mm:ss
				<< (loctime->tm_year + 1900) << '-' << setw(2) << (loctime->tm_mon + 1) << '-' << setw(2) << loctime->tm_mday << "      "             //yyyy-mm-dd
				<< "<" << funcName << ">:" << "  " << flush;                                                                                //info lable
			//Output function running info
			ofoper << funcInfo << "\n" << flush;

			//Exit log process
			ofoper.clear();
			ofoper.close();
		}
		else
			return;
	};

	*/

	// Effecient way to scan image.
	void ScanImageAndReduceC(Mat& imgIn,Mat& imgOut);
	// Scan Image And Reduce with Iterator method.
	void ScanImageAndReduceIterator(Mat& imgIn, Mat& imgOut);
};


