// ResizerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filter_resize.h"
#include "ResizerDlg.h"


// CResizerDlg dialog

IMPLEMENT_DYNAMIC(CResizerDlg, CDialog)

CResizerDlg::CResizerDlg(int sizex, int sizey, CWnd* pParent /*=NULL*/)
	: CDialog(CResizerDlg::IDD, pParent)
	, val_x(sizex)
	, val_y(sizey)
{

}

CResizerDlg::~CResizerDlg()
{
}

BOOL CResizerDlg::OnInitDialog()
{
	SetDlgItemInt(IDC_EDITX, val_x);
	SetDlgItemInt(IDC_EDITY, val_y);
	return CDialog::OnInitDialog();
}

void CResizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResizerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CResizerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CResizerDlg message handlers

void CResizerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	val_x = GetDlgItemInt(IDC_EDITX);
	val_y = GetDlgItemInt(IDC_EDITY);
	OnOK();
}
