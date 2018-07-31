// QV.cpp : implementation file
//

#include "stdafx.h"
#include "filter_quester.h"
#include "QV.h"


// QV dialog

IMPLEMENT_DYNAMIC(QV, CDialog)

QV::QV(int s, int t, CWnd* pParent /*=NULL*/)
	: CDialog(QV::IDD, pParent)
	, summCount(s)
	, timeStep(t)
{

}

QV::~QV()
{
}

void QV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN2, tSpin);
	DDX_Control(pDX, IDC_SPIN1, sSpin);
}


BEGIN_MESSAGE_MAP(QV, CDialog)
	ON_BN_CLICKED(IDOK, &QV::OnBnClickedOk)
END_MESSAGE_MAP()


// QV message handlers

void QV::OnBnClickedOk()
{
	summCount= GetDlgItemInt(IDC_EDIT1);
	timeStep = GetDlgItemInt(IDC_EDIT2);

	OnOK();
}
