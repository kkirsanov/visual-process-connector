#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// PanDlg dialog

class PanDlg : public CDialog
{
	DECLARE_DYNAMIC(PanDlg)

public:
	PanDlg(int s, int t,vector<Image *> *images, Core* core,  CWnd* pParent = NULL);   // standard constructor
	virtual ~PanDlg();
	Core *pCore;
	vector<Image *> *pImages;
	BOOL OnInitDialog(){
		Active=0;
		BOOL z = CDialog::OnInitDialog();
		tSpin.SetRange(0,5000);
		tSpin.SetBuddy((CEdit*)GetDlgItem(IDC_EDIT2));
		tSpin.SetPos(timeStep);

		sSpin.SetRange(2,1024);
		sSpin.SetBuddy((CEdit*)GetDlgItem(IDC_EDIT1));
		sSpin.SetPos(summCount);
		

		return z;
	};
	int timeStep;
	int summCount;
	int Active;

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CSpinButtonCtrl sSpin;
public:
	CSpinButtonCtrl tSpin;
public:
	CEdit samplesSpin;
public:
	CEdit TimeSpin;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
