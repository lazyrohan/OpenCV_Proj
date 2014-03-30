// CImgTreeCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProc.h"
#include "CImgTreeCtrl.h"


// CImgTreeCtrl

IMPLEMENT_DYNAMIC(CImgTreeCtrl, CTreeCtrl)

CImgTreeCtrl::CImgTreeCtrl():
m_dHTI(NULL)
{

}

CImgTreeCtrl::~CImgTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CImgTreeCtrl, CTreeCtrl)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CImgTreeCtrl ��Ϣ�������




BOOL CImgTreeCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO:  �ڴ����ר�ô����/����û���
	
	return CTreeCtrl::Create(dwStyle, rect, pParentWnd, nID);
}



void CImgTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO:  �ڴ˴������Ϣ����������
	//Set focus on RichEdit
	/*
	SetFocus();
	//Popup menu
	CMenu codePopmenu;
	//codePopmenu.CreatePopupMenu();
	codePopmenu.LoadMenuW(IDR_POPMENU_IMGTREE);
	//show menu
	codePopmenu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWnd);
	codePopmenu.DestroyMenu();
	*/
}


//void CImgTreeCtrl::OnImgtreeLoadimagefiles()
//{
//	// TODO:  �ڴ���������������
//	
//}


void CImgTreeCtrl::GetDirectoryImgFile(CString rootPath)
{
	//Insert top root item
	if (m_dHTI == NULL)
	{
		m_dHTI = InsertItem(TEXT("superImg"), 0, 0);
	}

	//Insert directory items
	CFileFind fileSearch;
	CString fileName;
	BOOL bResult = FALSE;
	//Root hti
	HTREEITEM hti;
	hti = m_dHTI;
	rootPath += TEXT("\\*.*");
	bResult = fileSearch.FindFile(rootPath, NULL);
	while (bResult)
	{
		bResult = fileSearch.FindNextFileW();
		if (!fileSearch.IsDots())
		{

			if (fileSearch.IsDirectory())
			{

				m_dHTI = InsertItem(fileSearch.GetFileName(), hti, 0);
				fileName = fileSearch.GetFilePath();

				GetDirectoryImgFile(fileName);
			}
			else
				InsertItem(fileSearch.GetFileName(), hti, 0);
		}
	}

	fileSearch.Close();
}


CString CImgTreeCtrl::GetSelImgPath(void)
{
	HTREEITEM hti;
	CString filePath;

	hti = GetSelectedItem();
	if (hti != NULL)
	{
		//Open img files not directory
		if (!ItemHasChildren(hti))
		{

			while (hti != NULL)
			{
				CString csbuff;
				csbuff += TEXT("\\");
				csbuff += GetItemText(hti);
				filePath = csbuff + filePath;
				hti = GetParentItem(hti);


			}
			return filePath;

		}


	}

	return filePath;
}


