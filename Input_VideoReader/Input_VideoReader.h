// Input_VideoReader.h : main header file for the Input_VideoReader DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "VideoSelector.h"

#include "dsutils.h" //for CLSID_SampleGrabber

#define PLUGIN_API __declspec(dllexport)

class CInput_VideoReaderApp : public CWinApp
{
public:
	CInput_VideoReaderApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

using namespace dsutils;
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


class CaptureReader: public Plugin {
	friend class SimpleOutImagePin<CaptureReader>;
private:
	PBitmap Bitmap;
	PImage image;
	int counter;
	void run();
	int CanStep;
	void stop();
	bool runOk;
	CString FilePath;
	UINT FrameStep;

public:
	CaptureReader(Core *pCore);
	virtual ~CaptureReader();
	UINT DoModal();
	long GetCount();
	Counter Next();

	void Serialize(CArchive& ar);

private:
	IGraphBuilder *pGraph;

	IFileSourceFilter *pFileI;

	DWORD dwRegister;

	char *pBuffer;
	VIDEOINFOHEADER *pVih;

	IBaseFilter *pSampleGrabber;
	IBaseFilter *pAviDeocompressor;
	IBaseFilter *pFile;
	IBaseFilter *pSplitter;

	IBaseFilter *pVR;

	IMediaSeeking *pMS;
	IMediaEvent *pME;
	IMediaPosition *pMP;
	IVideoFrameStep *pFS;
	IMediaControl *pMC;

	ISampleGrabber *pGrabber;

};