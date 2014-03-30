// CRawImgWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProc.h"
#include "CRawImgWnd.h"


// CRawImgWnd

IMPLEMENT_DYNAMIC(CRawImgWnd, CWnd)

CRawImgWnd::CRawImgWnd()
{

}

CRawImgWnd::~CRawImgWnd()
{
	m_iImage.Destroy();
}


BEGIN_MESSAGE_MAP(CRawImgWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CRawImgWnd 消息处理程序


void CRawImgWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	//if no image data exit
	if (m_iImage.IsNull())
		return;

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
	::SetBrushOrgEx(dc.GetSafeHdc(),0,0,NULL);
	if (!m_iImage.StretchBlt(dc.GetSafeHdc(), imgrc, SRCCOPY))
	{
		MessageBoxW(TEXT("Draw image failed for wrong parameters!"));
	}
	
}


 //Update Image data.
void CRawImgWnd::UpdateImage(Mat& rawMat,CString selImgPath)
{
	//Destroy image first
	if (!m_iImage.IsNull())
		m_iImage.Destroy();

	//Read new image  with opencv	
	if (rawMat.data)
	{
		rawMat.release();
	}
	USES_CONVERSION;
	rawMat = imread(W2A(selImgPath),-1);
	if (!rawMat.data)
	{
		MessageBoxW(TEXT("OpenCV read file failed"));
		return;
	}

	//Copy mat data to CImage	
	if (MatToCImage(rawMat, m_iImage))
	{
		//Update window
		Invalidate(TRUE);
	}
	else
	{
		MessageBoxW(TEXT("Failed copy Mat data for show!"));
		return;
	}
		

	
	
}


// Copy Mat data to CImage for using at MFC
BOOL CRawImgWnd::MatToCImage(Mat& rawMat,CImage& cimgShow)
{
	//Create new image data
	BOOL bResult = TRUE;
	int width = rawMat.cols;
	int height = rawMat.rows;
	int channel = rawMat.channels();

	bResult = cimgShow.Create(width, height, channel * 8);

	if (!bResult)
	{
		MessageBoxW(TEXT("Create CImage failed!"));
		return bResult;
	}

	//Copy mat data to CImage	
	if (channel == 1)
	{
		//灰度图像	
		//如果是1个通道的图像(灰度图像) DIB格式才需要对调色板设置
		//CImage中内置了调色板，我们要对他进行赋值：

		RGBQUAD* ColorTable;
		int MaxColors = 256;
		ColorTable = new RGBQUAD[MaxColors];
		cimgShow.GetColorTable(0, MaxColors, ColorTable);
		for (int i = 0; i<MaxColors; i++)
		{
			ColorTable[i].rgbBlue = (BYTE)i;
			ColorTable[i].rgbGreen = (BYTE)i;
			ColorTable[i].rgbRed = (BYTE)i;
		}
		cimgShow.SetColorTable(0, MaxColors, ColorTable);
		delete[]ColorTable;

		uchar *pS;
		//Get image data start address
		uchar *pImg = (uchar *)cimgShow.GetBits();
		int step = cimgShow.GetPitch();
		for (int i = 0; i<height; i++)
		{
			pS = rawMat.ptr<uchar>(i);
			for (int j = 0; j<width; j++)
			{
				*(pImg + i*step + j) = pS[j];
			}
		}

	}


	if (channel == 3)
	{
		//彩色图像
		//如果是3个通道(彩色图像)没有调色板，直接赋值
		uchar *pS;
		uchar *pImg = (uchar *)cimgShow.GetBits();//得到CImage数据区地址
		int step = cimgShow.GetPitch();
		for (int i = 0; i<height; i++)
		{
			pS = rawMat.ptr<uchar>(i);
			for (int j = 0; j<width; j++)
			{
				for (int k = 0; k<3; k++)
					*(pImg + i*step + j * 3 + k) = pS[j * 3 + k];
			}
		}

	}

	return bResult;
}
