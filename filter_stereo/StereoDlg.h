#pragma once
#include "afxcmn.h"


// StereoDlg dialog

class StereoDlg : public CDialog
{
	DECLARE_DYNAMIC(StereoDlg)

public:
	StereoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~StereoDlg();
	CxImage *pImg1, *pImg2;

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl SLD;
public:
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedButton1();
};
