// Input_capture.h : main header file for the Input_capture DLL
//

#pragma once

#include <dshow.h>
#include "qedit.h" //for CLSID_SampleGrabber
#include "uuids.h" //for CLSID_SampleGrabber
#include <deque>
#include "dsutils.h" //for CLSID_SampleGrabber

using namespace dsutils;


#include "afxwin.h"
#include "windows.h"

#include "stdafx.h"
#include "math.h"


struct RGB{
	unsigned char B1;	
	unsigned char B2;
	unsigned char B3;
	
};
struct RGBA{	
	unsigned char B1;
	unsigned char B2;	
	unsigned char B3;
	unsigned char A;
};


using namespace std;
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define PLUGIN_API __declspec(dllexport)
// CInput_captureApp
// See Input_capture.cpp for the implementation of this class
//

class CInput_captureApp : public CWinApp
{
public:
	CInput_captureApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
class CaptureReader: public Plugin {
	friend class SimpleOutImagePin<CaptureReader>;
private:
	PBitmap Bitmap;
	PImage image;
	int counter;
	void run();
	void stop();
	bool runOk;
	CString cameraName;
	CString typeName;

	CString PMT2String(AM_MEDIA_TYPE *pMT);
public:
	CaptureReader(Core *pCore);
	~CaptureReader();
	UINT DoModal();
	long GetCount();
	Counter Next();
	void Serialize(CArchive& ar);
private:
	IGraphBuilder *pGraph;
	IBaseFilter *pCap;
	ICaptureGraphBuilder2 * pCapture;
	IMediaControl *pControl;
	IMediaEvent   *pEvent;
	IVideoWindow *pVidWin;
	ICreateDevEnum *pDevEnum;
	IEnumMoniker *pEnum;
	IMoniker *pMoniker;
	DWORD dwRegister;
	
	CBitmap *testB;
	char *pBuffer;
	VIDEOINFOHEADER *pVih;

	IBaseFilter *pSmartTee;
	IBaseFilter *pSampleGrabber;
	IBaseFilter *pAviDeocompressor;
	
	ISampleGrabber *pGrabber;
	IBaseFilter *pVideoRender;

};