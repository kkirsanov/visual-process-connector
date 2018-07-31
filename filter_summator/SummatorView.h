#pragma once
#include "afxcmn.h"
#include <math.h>


// SummatorView dialog

class SummatorView : public CDialog
{
	DECLARE_DYNAMIC(SummatorView)

public:
	SummatorView(int s, int t, CWnd* pParent = NULL);   // standard constructor
	virtual ~SummatorView();
	BOOL OnInitDialog(){
		BOOL z = CDialog::OnInitDialog();
		tSpin.SetRange(0,5000);
		tSpin.SetBuddy((CEdit*)GetDlgItem(IDC_EDIT2));
		tSpin.SetPos(timeStep);

		sSpin.SetRange(2,1024);
		sSpin.SetBuddy((CEdit*)GetDlgItem(IDC_EDIT1));
		sSpin.SetPos(summCount);
		double reduction = 1/sqrt((double)summCount);

		CString str;
		str.Format("%f", reduction);
		SetDlgItemText(IDC_REDUCTION, str);

		//samplesSpin.SetRange(2,100);
		//TimeSpin.SetRange(0,5000);
		//TimeSpin.SetBuddy((CEdit*)GetDlgItem(IDC_EDIT4));

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

	CSpinButtonCtrl sSpin;
	CSpinButtonCtrl tSpin;

public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnEnChangeEdit1();
};
