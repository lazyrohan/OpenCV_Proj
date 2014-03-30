// CImgShowWnd.cpp : ʵ���ļ�
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



// CImgShowWnd ��Ϣ�������




// Copy Mat image data to CImage for show.
BOOL CImgShowWnd::MatToCImage(Mat& imgMat, CImage& cimgShow)
{
	//Create new image data
	BOOL bResult = TRUE;
	int width = imgMat.cols;
	int height = imgMat.rows;
	int channel = imgMat.channels();

	bResult = cimgShow.Create(width, height, channel * 8);

	if (!bResult)
	{
		MessageBoxW(TEXT("Create CImage failed!"));
		return bResult;
	}

	//Copy mat data to CImage	
	if (channel == 1)
	{
		//�Ҷ�ͼ��	
		//�����1��ͨ����ͼ��(�Ҷ�ͼ��) DIB��ʽ����Ҫ�Ե�ɫ������
		//CImage�������˵�ɫ�壬����Ҫ�������и�ֵ��

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
			pS = imgMat.ptr<uchar>(i);
			for (int j = 0; j<width; j++)
			{
				*(pImg + i*step + j) = pS[j];
			}
		}

	}


	if (channel == 3)
	{
		//��ɫͼ��
		//�����3��ͨ��(��ɫͼ��)û�е�ɫ�壬ֱ�Ӹ�ֵ
		uchar *pS;
		uchar *pImg = (uchar *)cimgShow.GetBits();//�õ�CImage��������ַ
		int step = cimgShow.GetPitch();
		for (int i = 0; i<height; i++)
		{
			pS = imgMat.ptr<uchar>(i);
			for (int j = 0; j<width; j++)
			{
				for (int k = 0; k<3; k++)
					*(pImg + i*step + j * 3 + k) = pS[j * 3 + k];
			}
		}

	}

	return bResult;
}


// Update image data to showout.
void CImgShowWnd::UpdateImage(Mat& imgMat)
{
	//Destroy image first
	if (!m_iImage.IsNull())
		m_iImage.Destroy();

	//Read new image  with opencv	
	if (!imgMat.data)
	{
		MessageBoxW(TEXT("Input invalidate Mat data for show!"));
		imgMat.release();
		return;
	}

	//Copy mat data to CImage	
	if (MatToCImage(imgMat, m_iImage))
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


void CImgShowWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd::OnLButtonDblClk(nFlags, point);
}


// Soble Operate to get image edge.
void CImgShowWnd::ImgSobelOperate(Mat& imgMat,Mat& outImg)
{
	if (!imgMat.data)
	{
		MessageBoxW(TEXT("Get invalidate Mat data"));
		return;
	}

	//GaussianBlur
	GaussianBlur(imgMat,outImg,Size(3,3),0,0,BORDER_DEFAULT);
	//Covert image to gray
	cvtColor(outImg,outImg,CV_RGB2GRAY);

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(outImg, grad_x, CV_16S, 1, 0, 3, 3, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(outImg, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, outImg);
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
