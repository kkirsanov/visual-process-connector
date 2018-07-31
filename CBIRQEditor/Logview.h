#pragma once


// CLogview dialog

class CLogview : public CDialog
{
	DECLARE_DYNAMIC(CLogview)

public:
	CLogview(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogview();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
