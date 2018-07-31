#pragma once
#include "resource.h"


// CScriptDialog dialog

class CScriptDialog : public CDialog
{
	DECLARE_DYNAMIC(CScriptDialog)

public:
	CScriptDialog(CString str, CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog(){
		BOOL z = CDialog::OnInitDialog();
		SetDlgItemText(IDC_EDIT1, script);
		return z;
	};
	virtual ~CScriptDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };
	CString script;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
