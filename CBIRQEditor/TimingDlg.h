#pragma once


// TimingDlg dialog

#include "../core/core/core.h"

class TimingDlg : public CDialog
{
	DECLARE_DYNAMIC(TimingDlg)

public:
	TimingDlg(Plugin *pPlug, CWnd* pParent = NULL);   // standard constructor
	virtual ~TimingDlg();
	void OnPaint();
	void OnMouseMove(UINT nFlags, CPoint point);


// Dialog Data
	enum { IDD = 136/*IDD_TIMEGRAPHDLG*/ };

protected:
	double curval;
	int mosuseX;
	string curStr;
	vector<int> counts;
	double mulK;
	double maxval;
	Plugin *pPlugin;
	PBitmap Bitmap;
	deque<double> timings;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
