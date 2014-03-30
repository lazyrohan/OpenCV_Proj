
// CImageProcView.cpp : CImageProcView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageProc.h"
#endif

#include "ImageProcDoc.h"
#include "CImageProcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcView

IMPLEMENT_DYNCREATE(CImageProcView, CView)

BEGIN_MESSAGE_MAP(CImageProcView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_COMMAND(ID_IMAGEPROC_SHOW, &CImageProcView::OnImageprocShow)
END_MESSAGE_MAP()

// CImageProcView 构造/析构

CImageProcView::CImageProcView():
m_pImgTreeCtrl(NULL),
m_pRawImgWnd(NULL),
m_pProceedImgWnd(NULL)
{
	// TODO:  在此处添加构造代码
	
}

CImageProcView::~CImageProcView()
{
	//Release view controls

	if (m_pProceedImgWnd)
	{
		delete m_pProceedImgWnd;
		m_pProceedImgWnd = NULL;
	}

	if (m_pRawImgWnd)
	{
		delete m_pRawImgWnd;
		m_pRawImgWnd = NULL;

	}
	
	if (m_pImgTreeCtrl)
	{
		delete m_pImgTreeCtrl;
		m_pImgTreeCtrl = NULL;
	}
	
}

BOOL CImageProcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImageProcView 绘制

void CImageProcView::OnDraw(CDC* /*pDC*/)
{
	CImageProcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
}


// CImageProcView 打印

BOOL CImageProcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageProcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CImageProcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CImageProcView 诊断

#ifdef _DEBUG
void CImageProcView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcDoc* CImageProcView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcDoc)));
	return (CImageProcDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcView 消息处理程序


void CImageProcView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	CRect rc;
	GetClientRect(&rc);

	//Set imgtree ctrl rect
	//Create imgtree ctl
	m_dCtrlRect[0].top = rc.top;
	m_dCtrlRect[0].left = rc.left;
	m_dCtrlRect[0].right = 200;
	m_dCtrlRect[0].bottom = rc.bottom;

	m_pImgTreeCtrl = new CImgTreeCtrl;
	m_pImgTreeCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS, m_dCtrlRect[0], this, IDC_IMGFILE_TREECTRL);
	
	CString imgPath;
	imgPath = "E:\\DataBank\\superImg";
	m_pImgTreeCtrl->GetDirectoryImgFile(imgPath);
	m_pImgTreeCtrl->Expand(m_pImgTreeCtrl->GetRootItem(),TVE_EXPAND);

	//Set rawimg wnd rect
	//Create rawimg wnd
	m_dCtrlRect[1].top = m_dCtrlRect[0].top;
	m_dCtrlRect[1].left = m_dCtrlRect[0].right + 1;
	m_dCtrlRect[1].bottom = rc.bottom;
	m_dCtrlRect[1].right = (LONG)((rc.right + m_dCtrlRect[0].right)*0.5)+1;

	m_pRawImgWnd = new CRawImgWnd;
	
	m_pRawImgWnd->CreateEx(
		NULL,
		AfxRegisterWndClass(NULL,NULL,(HBRUSH)::GetStockObject(GRAY_BRUSH),NULL),//Set this parameter can erease back ground ,otherwise don`t
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER|WS_VSCROLL|WS_HSCROLL,m_dCtrlRect[1],this,IDC_RAWIMG_WND);

	//Set proceed image wnd rect
	//Create proceed image wnd
	m_dCtrlRect[2].top = m_dCtrlRect[1].top;
	m_dCtrlRect[2].left = m_dCtrlRect[1].right + 1;
	m_dCtrlRect[2].bottom = (LONG)(rc.bottom);
	m_dCtrlRect[2].right = rc.right;

	m_pProceedImgWnd = new CImgShowWnd;

	m_pProceedImgWnd->CreateEx(
		NULL,
		AfxRegisterWndClass(NULL, NULL, (HBRUSH)::GetStockObject(WHITE_BRUSH), NULL),//Set this parameter can erease back ground ,otherwise don`t
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL, m_dCtrlRect[2], this, IDC_PROCEEDIMG_WND);
		
	
}


void CImageProcView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	// Do nothing when minimized
		if (nType == 1)
			return;
		
		if (m_pImgTreeCtrl  || m_pRawImgWnd||m_pProceedImgWnd)
		{
			m_dCtrlRect[0].bottom = cy;
			m_dCtrlRect[1].bottom = cy;
			m_dCtrlRect[1].right =(LONG) ((cx + m_dCtrlRect[0].right)*0.5) + 1;
			m_dCtrlRect[2].top = m_dCtrlRect[1].top;
			m_dCtrlRect[2].left = m_dCtrlRect[1].right + 1;
			m_dCtrlRect[2].bottom = (LONG)(cy);
			m_dCtrlRect[2].right = cx;

			m_pImgTreeCtrl->MoveWindow(m_dCtrlRect[0]);
			m_pRawImgWnd->MoveWindow(m_dCtrlRect[1]);
			m_pProceedImgWnd->MoveWindow(m_dCtrlRect[2]);
		}
		
	
	
}



BOOL CImageProcView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CImageProcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return CView::OnNotify(wParam, lParam, pResult);

	// TODO:  在此添加专用代码和/或调用基类
	
	if (IDC_IMGFILE_TREECTRL==wParam)
	{
		LPNMTREEVIEW treeVP = (LPNMTREEVIEW)lParam;
		if (treeVP->hdr.code==NM_DBLCLK)
		{
			//Draw select image file
			CString selImg = m_pImgTreeCtrl->GetSelImgPath();
			CString str;
			str = "E:\\DataBank";
			str += selImg;
			if (!selImg.IsEmpty())
			{
				m_pRawImgWnd->UpdateImage(pDoc->m_dRawImg,str);
			}
		}
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}


void CImageProcView::OnImageprocShow()
{
	// TODO:  在此添加命令处理程序代码
	CImageProcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Mat sobelImg;
	m_pProceedImgWnd->ImgSobelOperate(pDoc->m_dRawImg,sobelImg);
	m_pProceedImgWnd->UpdateImage(sobelImg);
}
