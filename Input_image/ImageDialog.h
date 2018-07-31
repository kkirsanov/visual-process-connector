#pragma once




// CImageDialog dialog
class CImageDialog : public CDialog
{
	DECLARE_DYNAMIC(CImageDialog)

public:
	CImageDialog(PImage img,Core *core, CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };
	CString FileName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	PImage img;
	PBitmap bitmap;
	Core *pCore;
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnClose();
};
