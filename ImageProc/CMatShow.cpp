//#include "stdafx.h"
#include "CMatShow.h"


CMatShow::CMatShow()
{
}


CMatShow::~CMatShow()
{
	//Release mat buffers
	m_mProcImg.release();
	m_mSrcImg.release( );
}


// Load Image Resource 
bool CMatShow::LoadImgResource( string ImgSrcPath )
{
	//Load image source file
	//Check img mat buffer status
	if ( m_mSrcImg.data )
	{
		m_mSrcImg.release( );
	}
	//Load img src file original
	m_mSrcImg = imread(ImgSrcPath,-1 );
	if (!m_mSrcImg.data )
	{
		m_sErrStr = "<imread> Load image file failed.";
		m_mSrcImg.release( );
		return false;
	}

	return true;
}



// Convert img mat resolution to terminal show wnd.
Mat CMatShow::FitImgMatWnd(Mat srcImg, int wndHeight, int wndWidth )
{
	Mat dstImg;
	if (wndHeight <= 0 || wndWidth <= 0)
	{
		m_sErrStr = "Input invalid wnd height or width,check them.";
		return dstImg;
	}
	//Get properly desty size width/height
	Size wndSize;

	double srcRatio = srcImg.rows / srcImg.cols;
	double wndRatio = wndHeight / wndWidth;

	if (srcRatio <= wndRatio )
	{
		wndSize.height = (int)(wndWidth*srcRatio);
		wndSize.width = wndWidth;
	}
	else
	{
		wndSize.height = wndHeight;
		wndSize.width = (int)(wndHeight / srcRatio);
	}

	resize( m_mSrcImg, dstImg, wndSize, 0.0, 0.0, INTER_AREA );
	return dstImg;
}


// Convert mat to CImage
BOOL CMatShow::Mat2CImg(CImage& cimgShow,bool bOrgImg )
{
	//Choose orignal or proceed image
	Mat imgBuff;
	if (bOrgImg)
	{
		imgBuff = m_mSrcImg;
	}
	else
	{
		imgBuff = m_mProcImg;
	}
	//Create new image data
	if ( !cimgShow.IsNull( ) )
	{
		cimgShow.Destroy( );
	}
	BOOL bResult = TRUE;
	int width = imgBuff.cols;
	int height = imgBuff.rows;
	int channel = imgBuff.channels( );

	bResult = cimgShow.Create( width, height, channel * 8 );

	if ( !bResult )
	{
		m_sErrStr = "Create CImage failed.";
		return bResult;
	}

	//Copy mat data to CImage	
	if ( channel == 1 )
	{
		//灰度图像	
		//如果是1个通道的图像(灰度图像) DIB格式才需要对调色板设置
		//CImage中内置了调色板，我们要对他进行赋值：

		RGBQUAD* ColorTable;
		int MaxColors = 256;
		ColorTable = new RGBQUAD[ MaxColors ];
		cimgShow.GetColorTable( 0, MaxColors, ColorTable );
		for ( int i = 0; i<MaxColors; i++ )
		{
			ColorTable[ i ].rgbBlue = (BYTE)i;
			ColorTable[ i ].rgbGreen = (BYTE)i;
			ColorTable[ i ].rgbRed = (BYTE)i;
		}
		cimgShow.SetColorTable( 0, MaxColors, ColorTable );
		delete[ ]ColorTable;

		uchar *pS;
		//Get image data start address
		uchar *pImg = (uchar *)cimgShow.GetBits( );
		int step = cimgShow.GetPitch( );
		for ( int i = 0; i<height; i++ )
		{
			pS = imgBuff.ptr<uchar>( i );
			for ( int j = 0; j<width; j++ )
			{
				*(pImg + i*step + j) = pS[ j ];
			}
		}

	}


	if ( channel == 3 )
	{
		//彩色图像
		//如果是3个通道(彩色图像)没有调色板，直接赋值
		uchar *pS;
		uchar *pImg = (uchar *)cimgShow.GetBits( );//得到CImage数据区地址
		int step = cimgShow.GetPitch( );
		for ( int i = 0; i<height; i++ )
		{
			pS = imgBuff.ptr<uchar>( i );
			for ( int j = 0; j<width; j++ )
			{
				for ( int k = 0; k<3; k++ )
					*(pImg + i*step + j * 3 + k) = pS[ j * 3 + k ];
			}
		}

	}

	return bResult;
}


// Get Last Erro Message
string CMatShow::GetLastErrStr(void)
{
	return m_sErrStr;
}


// Sobel Operation to get image edge.
void CMatShow::ImgSobelOperate()
{
	if (!m_mSrcImg.data)
	{
		m_sErrStr= "Get invalidate Mat data";
		return;
	}

	//GaussianBlur
	GaussianBlur(m_mSrcImg, m_mProcImg, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//Covert image to gray
	cvtColor(m_mProcImg, m_mProcImg, CV_RGB2GRAY);

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(m_mProcImg, grad_x, CV_16S, 1, 0, 3, 3, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(m_mProcImg, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, m_mProcImg);
}
