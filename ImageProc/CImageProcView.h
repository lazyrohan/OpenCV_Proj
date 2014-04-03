
// CImageProcView.h : CImageProcView 类的接口
//

#pragma once
#include "CImgTreeCtrl.h"
#include "CRawImgWnd.h"
#include "CImgShowWnd.h"
class CImageProcView : public CView
{
protected: // 仅从序列化创建
	CImageProcView();
	DECLARE_DYNCREATE(CImageProcView)

// 特性
public:
	CImageProcDoc* GetDocument() const;

// 操作
public:
	//View controls
	CImgTreeCtrl* m_pImgTreeCtrl;
	CRawImgWnd*   m_pRawImgWnd;
	CImgShowWnd*  m_pProceedImgWnd;

private:
	CRect m_dCtrlRect[4];

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImageProcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // CImageProcView.cpp 中的调试版本
inline CImageProcDoc* CImageProcView::GetDocument() const
   { return reinterpret_cast<CImageProcDoc*>(m_pDocument); }
#endif

