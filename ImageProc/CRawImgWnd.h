#pragma once

//Include opencv core headers
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

// CRawImgWnd

class CRawImgWnd : public CWnd
{
	DECLARE_DYNAMIC(CRawImgWnd)

public:
	CRawImgWnd();
	virtual ~CRawImgWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	// Update Image data.
	void UpdateImage( void );
public:
	CImage m_iImage;
	
	
public:

private:
	// Copy Mat data to CImage for using at MFC
	BOOL MatToCImage(Mat& rawMat,CImage& cimgShow);
};


