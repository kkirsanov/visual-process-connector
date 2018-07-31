#pragma once
#include "c:\program files\microsoft visual studio 8\vc\atlmfc\include\afxwin.h"


// CSortViewDialog dialog

class CSortViewDialog : public CDialog
{
	DECLARE_DYNAMIC(CSortViewDialog)

public:
	CSortViewDialog(CBIR::Core *pCore, vector<Logic> *logics, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSortViewDialog();
	BOOL OnInitDialog();

	CBIR::Core *pCore;
	CxImage *bitmap;
	vector<Logic> *logics;
	
// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CListBox picList;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	afx_msg void OnLbnSelchangeList1();
};
