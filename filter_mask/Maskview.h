#pragma once
#include "afxwin.h"
#include "mask.h"

// Maskview dialog

class Maskview : public CDialog
{
	DECLARE_DYNAMIC(Maskview)

public:
	Maskview(MaskPalette *pmp,Mask &m1, Mask &m2, Mask &m3, CString &ColorModel, CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog();
	MaskPalette *pMaskPalette;
	virtual ~Maskview();
	CString *pColorModel;
	Mask *pM1;
	Mask *pM2;
	Mask *pM3;
	vector<CEdit*> M1;
	vector<CEdit*> M2;
	vector<CEdit*> M3;

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CListBox CPS;
public:
	CListBox FilterSelect;
public:
	CListBox M1Size;
public:
	afx_msg void OnLbnSelchangeList4();
public:
	afx_msg void OnBnClickedButton1();
public:
	CListBox M2Size;
public:
	CListBox M3Size;
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnLbnSelchangeList5();
public:
	afx_msg void OnLbnSelchangeList6();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedOk();
};
