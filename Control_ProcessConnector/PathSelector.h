#pragma once


// PathSelector dialog

class PathSelector : public CDialog
{
	DECLARE_DYNAMIC(PathSelector)

public:
	PathSelector(CString *Path, CWnd* pParent = NULL);   // standard constructor
	virtual ~PathSelector();
	CString *tmpPath;
	BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
