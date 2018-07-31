#pragma once
#include "afxcmn.h"


// CNetDlg dialog

class CNetDlg : public CDialog
{
	DECLARE_DYNAMIC(CNetDlg)

public:
	CNetDlg(DWORD *ip, UINT *remotePort,UINT *localPort, CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog();
	virtual ~CNetDlg();
	DWORD *IP;
	UINT *LOCALPORT;
	UINT *REMOTEPORT;

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl IPCtrl;
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedOk();
};