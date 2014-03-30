#pragma once


// CImgTreeCtrl

class CImgTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CImgTreeCtrl)

public:
	CImgTreeCtrl();
	virtual ~CImgTreeCtrl();

private:
	HTREEITEM m_dHTI;
public:
	//CString m_sSelImPath;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
//	afx_msg void OnImgtreeLoadimagefiles();
private:
	
public:
	void GetDirectoryImgFile(CString rootPath);
	CString GetSelImgPath(void);
	
};


