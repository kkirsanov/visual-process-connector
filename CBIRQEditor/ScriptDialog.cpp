// ScriptDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CBIRQEditor.h"
#include "ScriptDialog.h"


// CScriptDialog dialog

IMPLEMENT_DYNAMIC(CScriptDialog, CDialog)

CScriptDialog::CScriptDialog(CString str, CWnd* pParent /*=NULL*/)
	: CDialog(CScriptDialog::IDD, pParent)
{
	script = str;
}

CScriptDialog::~CScriptDialog()
{
}

void CScriptDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScriptDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CScriptDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CScriptDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// CScriptDialog message handlers

void CScriptDialog::OnBnClickedOk()
{
	
	char str[2000];
	GetDlgItemText(IDC_EDIT1,str, 2000);
	script = CString(str);
	// TODO: Add your control notification handler code here
	OnOK();
}

void CScriptDialog::OnBnClickedButton1()
{
	char str[2000];
	GetDlgItemText(IDC_EDIT1,str, 2000);
	script = CString(str);
	OnCancel();
}
