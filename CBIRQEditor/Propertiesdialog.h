#pragma once

#include "resource.h"
#include "CBIRQEditorDoc.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <deque>
// Propertiesdialog dialog
#include <psapi.h>
#include "CpuUsage.h"



using namespace std;
class Renewer;

class Propertiesdialog : public CDialog
{
	DECLARE_DYNAMIC(Propertiesdialog)

public:
	Propertiesdialog(CCBIRQEditorDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~Propertiesdialog();
	void PaintLoad();
	vector<int> _CPULoad;
	CxImage CPULoadBitmap;
	CCpuUsage CpuUsage;
	CCBIRQEditorDoc *pDoc;
	BOOL OnInitDialog();
	int OnDestroy();
	void OnOK(){};
	void Renew();
	UINT_PTR timerE;

	CString currentModuleName;
	CString currentModuleDuration;
	void FillView(CCBIRQElement *elem);
	void OnPaint();
	CCBIRQElement *pOldElement;

// Dialog Data
	enum { IDD = IDD_PROPD };
private:
   HKEY CPULoadKey;
   int CPULoad;
   ULONG Type;
   DWORD CPULoadSize;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeDescedit();
public:
	afx_msg void OnBnClickedButton1();
public:
	CEdit DescCtrl;
public:
	CListCtrl InPinCtrl;
public:
	CListCtrl OutPinCtrl;
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedAddnewelementbtn();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtntiming();
};

