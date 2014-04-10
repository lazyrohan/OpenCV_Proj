// CImgShowWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProc.h"
#include "CImgShowWnd.h"


// CImgShowWnd

IMPLEMENT_DYNAMIC(CImgShowWnd, CWnd)

CImgShowWnd::CImgShowWnd()
{

}

CImgShowWnd::~CImgShowWnd()
{
	m_iImage.Destroy();
}


BEGIN_MESSAGE_MAP(CImgShowWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CImgShowWnd 消息处理程序

// Update image data to showout.
void CImgShowWnd::UpdateImage(void)
{

}


void CImgShowWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()

	//if no image data exit
	if (!m_iImage.IsNull())
	{

		//Scale img to proper size to fit window
		CRect rc;
		GetClientRect(&rc);
		CRect imgrc;
		imgrc = rc;

		float wndRatio = (float)rc.right / rc.bottom;
		float imgRatio = (float)m_iImage.GetWidth() / m_iImage.GetHeight();

		//Scale image draw area
		if (wndRatio >= imgRatio)
		{
			imgrc.top = rc.top;
			imgrc.left = (LONG)((rc.right - rc.bottom*imgRatio)*0.5);
			imgrc.bottom = rc.bottom;
			imgrc.right = (LONG)((rc.right + rc.bottom*imgRatio)*0.5);
		}
		else
		{
			imgrc.top = (LONG)((rc.bottom - rc.right / imgRatio)*0.5);
			imgrc.left = rc.left;
			imgrc.right = rc.right;
			imgrc.bottom = (LONG)((rc.bottom + rc.right / imgRatio)*0.5);
		}

		//Protect color lose
		::SetStretchBltMode(dc.GetSafeHdc(), HALFTONE);
		::SetBrushOrgEx(dc.GetSafeHdc(), 0, 0, NULL);
		if (!m_iImage.StretchBlt(dc.GetSafeHdc(), imgrc, SRCCOPY))
		{
			MessageBoxW(TEXT("Draw image failed for wrong parameters!"));
		}
	

	}

	dc.TextOutW(0, 0, TEXT("Image Proceed"));
}


void CImgShowWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	MessageBoxW( TEXT("Lbtn dClk"));
	CWnd::OnLButtonDblClk(nFlags, point);
}



// Effecient way to scan image.
void CImgShowWnd::ScanImageAndReduceC(Mat& imgIn,Mat& imgOut)
{
	// accept only char type matrices
	//CV_Assert(I.depth() != sizeof(uchar));

	int channels = imgIn.channels();
	int nRows = imgIn.rows;
	int nCols = imgIn.cols * channels;
	if (imgIn.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int* ip;
	int* op;
	for (int i = 0; i < nRows; ++i)
	{
		ip = imgIn.ptr<int>(i);
		op = imgOut.ptr<int>(i);
		for (int j = 0; j < nCols; ++j)
		{
			op[j] = ip[j]/2;
		}
	}
}



// Scan Image And Reduce with Iterator method.
void CImgShowWnd::ScanImageAndReduceIterator(Mat& imgIn, Mat& imgOut)
{
	// accept only char type matrices
	//CV_Assert(I.depth() != sizeof(uchar));

	const int channels = imgIn.channels();
	switch (channels)
	{
	case 1:
	{
		MatIterator_<int> it, iend, ot;
		for (it = imgIn.begin<int>(), iend = imgIn.end<int>(); it != iend; ++it)
			*ot = *it;
		break;
	}
	case 3:
	{
		MatIterator_<Vec3i> it, iend,ot;
		for (it = imgIn.begin<Vec3i>(), iend = imgIn.end<Vec3i>(); it != iend; ++it)
		{
			(*ot)[0] = (*it)[0];
			(*ot)[1] = (*it)[1];
			(*ot)[2] = (*it)[2];
		}
	}
	}
	
}
