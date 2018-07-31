#pragma once


// CResizerDlg dialog

class CResizerDlg : public CDialog
{
	DECLARE_DYNAMIC(CResizerDlg)

public:
	CResizerDlg(int sizex=0, int sizey=0, CWnd* pParent = NULL);   // standard constructor
	virtual ~CResizerDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGRESIZER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	unsigned int val_x;
	unsigned int val_y;
	afx_msg void OnBnClickedOk();
};
