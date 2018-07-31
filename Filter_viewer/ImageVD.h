#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include <deque>
using namespace std;

// CImageVD dialog

class CImageVD : public CDialog
{
	DECLARE_DYNAMIC(CImageVD)

public:
	int flag;
	bool IsP;
	bool* pIsD;
	boost::mutex mutex;
	boost::mutex fillMutex;
	CImageVD(PBitmap bitmap,bool *IsDialog, Core *core, CWnd* pParent = NULL);   // standard constructor
	int FirstStart;
	//deque<CxImage*> Images;
	void Sync(PBitmap bmp){
		
		IsP=true;
		boost::mutex::scoped_lock lock(fillMutex);

		if(FirstStart==0)
			FirstStart=1;
		pbitmap = PBitmap(new CxImage(*bmp.get(),true,false,false));

		if(pbitmap->GetWidth()<sizeX/2)
			pbitmap->Resample(pbitmap->GetWidth()*2, pbitmap->GetHeight()*2);

		InvalidateRect(CRect(0,0,sizeX,sizeY), 0);
		lock.unlock();

		tagMSG message;
		
		while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		};

	};
	BOOL OnInitDialog();
	virtual ~CImageVD();
	PBitmap pbitmap;
	Core *pCore;
	int sizeX, sizeY;

// Dialog Data
	enum { IDD = IDD_DIALOGV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CListBox DataList;
public:
	afx_msg void OnBnClickedButton1();
public:
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	CScrollBar VScroll;
public:
	CScrollBar HScroll;
public:
	CSliderCtrl HSlider;
public:
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CSliderCtrl VSlider;
public:
	afx_msg void OnNMReleasedcaptureSlider2(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnClose();
};
