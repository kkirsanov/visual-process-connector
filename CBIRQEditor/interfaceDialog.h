#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CinterfaceDialog dialog

class CinterfaceDialog : public CDialog
{
	DECLARE_DYNAMIC(CinterfaceDialog)

public:
	CinterfaceDialog(CBIR::Core *cc, CWnd* pParent = NULL);   // standard constructor
	virtual ~CinterfaceDialog();
	BOOL OnInitDialog();	

// Dialog Data
	enum { IDD = IDD_INTERFACEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CBIR::Core * core;

	DECLARE_MESSAGE_MAP()
public:
public:
	CListBox MyListBox;
public:
	afx_msg void OnBnClickedButton1();
};
