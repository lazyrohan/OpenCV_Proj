
// CImageProcView.h : CImageProcView ��Ľӿ�
//

#pragma once
#include "CImgTreeCtrl.h"
#include "CRawImgWnd.h"
#include "CImgShowWnd.h"
class CImageProcView : public CView
{
protected: // �������л�����
	CImageProcView();
	DECLARE_DYNCREATE(CImageProcView)

// ����
public:
	CImageProcDoc* GetDocument() const;

// ����
public:
	//View controls
	CImgTreeCtrl* m_pImgTreeCtrl;
	CRawImgWnd*   m_pRawImgWnd;
	CImgShowWnd*  m_pProceedImgWnd;

private:
	CRect m_dCtrlRect[4];

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CImageProcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnImageprocShow();
	afx_msg void OnCoremodualMaskoperation( );
};

#ifndef _DEBUG  // CImageProcView.cpp �еĵ��԰汾
inline CImageProcDoc* CImageProcView::GetDocument() const
   { return reinterpret_cast<CImageProcDoc*>(m_pDocument); }
#endif

