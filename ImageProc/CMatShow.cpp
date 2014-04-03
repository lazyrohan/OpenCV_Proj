//#include "stdafx.h"
#include "CMatShow.h"


CMatShow::CMatShow()
{
}


CMatShow::~CMatShow()
{
	m_mSrcImg.release( );
}


// Load Image Resource 
void CMatShow::LoadImgResource( string ImgSrcPath )
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
		m_mSrcImg.release( );
		return;
	}
}



// Convert img mat resolution to terminal show wnd.
void CMatShow::FitImgMatWnd(Mat srcImg,Mat dstImg, int wndHeight, int wndWidth )
{
	if ( wndHeight==0||wndWidth==0 )
		return;
	//Get properly desty size width/height
	Size wndSize;

	double srcRatio = srcImg.rows / srcImg.cols;
	double wndRatio = wndHeight / wndWidth;

	if (srcRatio <= wndRatio )
	{
		wndSize.height = (int)wndWidth*srcRatio;
		wndSize.width = wndWidth;
	}
	else
	{
		wndSize.height = wndHeight;
		wndSize.width = (int)wndHeight / srcRatio;
	}

	resize( m_mSrcImg, dstImg, wndSize, 0.0, 0.0, INTER_AREA );
}


// Convert mat to CImage
bool CMatShow::Mat2CImg( Mat imgMat,CImage& cimgShow )
{
	//Create new image data
	if ( !cimgShow.IsNull( ) )
	{
		cimgShow.Destroy( );
	}
	bool bResult = TRUE;
	int width = imgMat.cols;
	int height = imgMat.rows;
	int channel = imgMat.channels( );

	bResult = cimgShow.Create( width, height, channel * 8 );

	if ( !bResult )
	{
		return bResult;
	}

	//Copy mat data to CImage	
	if ( channel == 1 )
	{
		//�Ҷ�ͼ��	
		//�����1��ͨ����ͼ��(�Ҷ�ͼ��) DIB��ʽ����Ҫ�Ե�ɫ������
		//CImage�������˵�ɫ�壬����Ҫ�������и�ֵ��

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
			pS = imgMat.ptr<uchar>( i );
			for ( int j = 0; j<width; j++ )
			{
				*(pImg + i*step + j) = pS[ j ];
			}
		}

	}


	if ( channel == 3 )
	{
		//��ɫͼ��
		//�����3��ͨ��(��ɫͼ��)û�е�ɫ�壬ֱ�Ӹ�ֵ
		uchar *pS;
		uchar *pImg = (uchar *)cimgShow.GetBits( );//�õ�CImage��������ַ
		int step = cimgShow.GetPitch( );
		for ( int i = 0; i<height; i++ )
		{
			pS = imgMat.ptr<uchar>( i );
			for ( int j = 0; j<width; j++ )
			{
				for ( int k = 0; k<3; k++ )
					*(pImg + i*step + j * 3 + k) = pS[ j * 3 + k ];
			}
		}

	}

	return bResult;
}
