#pragma once


// VideoSelector dialog

class VideoSelector : public CDialog
{
	DECLARE_DYNAMIC(VideoSelector)

public:
	int FrameStep;
	CString FilePath;

	VideoSelector(int step, CString path, CWnd* pParent = NULL); 
	virtual BOOL OnInitDialog();
	virtual ~VideoSelector();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedOk();
};
