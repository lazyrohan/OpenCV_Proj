// CRawImgWnd.cpp : ʵ���ļ�
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



// CRawImgWnd ��Ϣ�������


void CRawImgWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	//if no image data exit
	if (m_iImage.IsNull())
		return;

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
void CRawImgWnd::UpdateImage(void)
{	

	//Update window
	Invalidate(TRUE);
}

