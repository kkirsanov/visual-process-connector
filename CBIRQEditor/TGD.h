#pragma once

#include "stdafx.h"
#include <boost/lexical_cast.hpp>



// TGD dialog

class TGD : public CDialog
{
	DECLARE_DYNAMIC(TGD)

public:
	TGD(Plugin *pPlug, CWnd* pParent = NULL);   // standard constructor
	virtual ~TGD();

// Dialog Data
	enum { IDD = IDD_TIMEGRAPHDLG };

protected:
	void Prepare();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double view_start;
	double view_finish;
	double median;
	double dispers;

	double maxVal;
	double minVal;

	double view_y;

	double curpos;
	double curval;
	int mosuseX;
	string curStr;
	vector<int> counts;
	double mulK;
	double maxval;
	Plugin *pPlugin;
	PBitmap Bitmap;
	deque<double> timings;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
