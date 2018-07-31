#pragma once
#include "afxcmn.h"


// QV dialog

class QV : public CDialog
{
	DECLARE_DYNAMIC(QV)

public:
	QV(int s, int t, CWnd* pParent = NULL);   // standard constructor
	virtual ~QV();
	BOOL OnInitDialog(){
		BOOL z = CDialog::OnInitDialog();
		tSpin.SetRange(0,5000);
		tSpin.SetBuddy((CEdit*)GetDlgItem(IDC_EDIT2));
		tSpin.SetPos(timeStep);

		sSpin.SetRange(0,1024);
		sSpin.SetBuddy((CEdit*)GetDlgItem(IDC_EDIT1));
		sSpin.SetPos(summCount);

		return z;
	}
// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int timeStep;
	int summCount;
	afx_msg void OnBnClickedOk();
public:
	CSpinButtonCtrl tSpin;
public:
	CSpinButtonCtrl sSpin;
};
