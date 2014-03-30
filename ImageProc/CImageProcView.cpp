
// CImageProcView.cpp : CImageProcView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_COMMAND(ID_IMAGEPROC_SHOW, &CImageProcView::OnImageprocShow)
END_MESSAGE_MAP()

// CImageProcView ����/����

CImageProcView::CImageProcView():
m_pImgTreeCtrl(NULL),
m_pRawImgWnd(NULL),
m_pProceedImgWnd(NULL)
{
	// TODO:  �ڴ˴���ӹ������
	
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
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CImageProcView ����

void CImageProcView::OnDraw(CDC* /*pDC*/)
{
	CImageProcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CImageProcView ��ӡ

BOOL CImageProcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImageProcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImageProcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CImageProcView ���

#ifdef _DEBUG
void CImageProcView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcDoc* CImageProcView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcDoc)));
	return (CImageProcDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcView ��Ϣ�������


void CImageProcView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO:  �ڴ����ר�ô����/����û���
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

	// TODO:  �ڴ˴������Ϣ����������
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

	// TODO:  �ڴ����ר�ô����/����û���
	
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
	// TODO:  �ڴ���������������
	CImageProcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Mat sobelImg;
	m_pProceedImgWnd->ImgSobelOperate(pDoc->m_dRawImg,sobelImg);
	m_pProceedImgWnd->UpdateImage(sobelImg);
}
