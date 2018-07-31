#pragma once

#include "dshow.h"
#include "qedit.h" //for CLSID_SampleGrabber
#include "uuids.h" //for CLSID_SampleGrabber
#include "dsutils.h" //for CLSID_SampleGrabber

using namespace dsutils;


class CCameraSelectodGlg : public CDialog
{
	DECLARE_DYNAMIC(CCameraSelectodGlg)

private:
	CString *m_str;
	CString *m_type;
	IGraphBuilder *pGB;
	ICaptureGraphBuilder2 *pCGB;
	IMediaControl *pMC;
	IMediaEventEx *pME;
	IVideoWindow  *pVW;
	IBasicAudio   *pBA;
	IBasicVideo   *pBV;
	IMediaSeeking *pMS;
	IMediaPosition *pMP;
	IVideoFrameStep *pFS;

	ICreateDevEnum *pDevEnum; 
	IEnumMoniker *pEnum;
	IMoniker *pMoniker;
	IBaseFilter *pCap;
	ICaptureGraphBuilder2 * pCapture;

	IBaseFilter *pSampleGrabber;
	ISampleGrabber *pGrabber;
	IBaseFilter *pVideoRender;
public:
	CCameraSelectodGlg(CString* name,CString* type, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCameraSelectodGlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void StopAndClear();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeList3();	
	afx_msg void OnLbnSelchangeListms();

	CListBox ModeSelectorCtrl;
	CListBox captureList;
};
