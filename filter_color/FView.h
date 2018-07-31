#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CFView dialog

class CFView : public CDialog
{
	DECLARE_DYNAMIC(CFView)

public:
	//CFView(GFL_BITMAP * bmp, int H, int S, int V, Core *core, CWnd* pParent = NULL);   // standard constructor
	CFView(PBitmap bmp, int H, int S, int V,  int _mH, int _mS, int _mV, int _val, Core *core, CWnd* pParent /*= NULL*/);

	void UpdateColors(int col=0/*0=rgb, else HSV*/);
	void UpdateImage();
	virtual ~CFView();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

	int _h, _s, _v;

	int mH,mS,mV;
	int val;

protected:

	
	PBitmap bitmap, scaledBitmap,preview_Bitmap;
	Core *pCore;
	PBitmap palette;
	void GeneratePalette(int val);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl CR;
	CSliderCtrl CG;
	CSliderCtrl CB;
	CSliderCtrl CH;
	CSliderCtrl CS;
	CSliderCtrl CV;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	BOOL OnInitDialog();
public:
	afx_msg void OnNMReleasedcaptureR(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureB(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureG(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMCustomdrawR(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMCustomdrawH(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMCustomdrawG(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMCustomdrawB(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureH(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureS(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureV(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CStatic PC;
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	CSliderCtrl CMH;
public:
	CSliderCtrl CMS;
public:
	CSliderCtrl CMV;
public:
	CSliderCtrl CMVAL;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureSlider3(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureSlider2(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMReleasedcaptureSlider4(NMHDR *pNMHDR, LRESULT *pResult);
};
