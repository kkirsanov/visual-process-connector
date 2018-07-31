// SummatorView.cpp : implementation file
//

#include "stdafx.h"
#include "filter_summator.h"
#include "SummatorView.h"


// SummatorView dialog

IMPLEMENT_DYNAMIC(SummatorView, CDialog)

SummatorView::SummatorView(int s, int t, CWnd* pParent /*=NULL*/)
	: CDialog(SummatorView::IDD, pParent)
	, summCount(s)
	, timeStep(t)
{


}

SummatorView::~SummatorView()
{
}

void SummatorView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, sSpin);
	DDX_Control(pDX, IDC_SPIN2, tSpin);
}


BEGIN_MESSAGE_MAP(SummatorView, CDialog)
	ON_BN_CLICKED(IDOK, &SummatorView::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &SummatorView::OnEnChangeEdit1)
END_MESSAGE_MAP()


// SummatorView message handlers

void SummatorView::OnBnClickedOk()
{
	summCount= GetDlgItemInt(IDC_EDIT1);
	timeStep = GetDlgItemInt(IDC_EDIT2);
	
	OnOK();
}

void SummatorView::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	summCount= GetDlgItemInt(IDC_EDIT1);
	timeStep = GetDlgItemInt(IDC_EDIT2);

	double reduction = 1/sqrt((double)summCount);
	CString str;
	str.Format("%f", reduction);
	SetDlgItemText(IDC_REDUCTION, str);
}
