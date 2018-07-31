// Input_VideoReader.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Input_VideoReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CInput_VideoReaderApp, CWinApp)
END_MESSAGE_MAP()


// CInput_VideoReaderApp construction

CInput_VideoReaderApp::CInput_VideoReaderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CInput_VideoReaderApp object

CInput_VideoReaderApp theApp;


// CInput_VideoReaderApp initialization

BOOL CInput_VideoReaderApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isInputPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	CaptureReader *pCr = new CaptureReader(pCore);
	return pCr;

};

CaptureReader::CaptureReader(Core *pCore): Plugin(pCore){	
	counter = 0;
	runOk = false;
	CanStep=0;
	image = PImage(new Image());

	pluginType = CORE_PLUGIN_TYPE::CORE_PLG_INPUT;
	name = "Video Reader";
	description = "Video Reader";

	SimpleOutImagePin<CaptureReader> *pPin = new SimpleOutImagePin<CaptureReader>(this);
	outPins.push_back(pPin);

	FilePath = "";
	FrameStep = 1;

	pBuffer = NULL;
	pFile = NULL;

	pSampleGrabber=NULL;
	pVR=NULL;
	pFile=NULL;
	pFileI=NULL;
	pGrabber=NULL;
	pSampleGrabber=NULL;
	pMS=NULL;
	pMP=NULL;
	pFS=NULL;
	pME=NULL;

	pGraph=NULL;
};


CaptureReader::~CaptureReader(){
	if (runOk)
		stop();

};
UINT CaptureReader::DoModal(){
	CString  str("");
	CString  str2("");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	VideoSelector dlg(FrameStep, FilePath);
	dlg.DoModal();
	if (dlg.FilePath.GetLength()!=0){
		FrameStep = dlg.FrameStep;
		if(FrameStep <=0)
			FrameStep =1;

		if (FilePath!=dlg.FilePath){
			FilePath = dlg.FilePath;
			CanStep=0;
			stop();
			run();
		}

		FilePath = dlg.FilePath;

		return MB_OK;
	};
	return 0;
};
long CaptureReader::GetCount(){
	return 0;
};
Counter CaptureReader::Next(){
	StartTiming();
	if(!runOk){		
		run();

		Sleep(500);

	}
	if(FilePath==CString("")){
		EndTiming();
		return Counter();
	}

	//move to next frame

	HRESULT hr;

	LONGLONG pos=0;
	LONGLONG duration=0;
	hr = pMS->GetCurrentPosition(&pos);
	hr = pMS->GetDuration(&duration);


	if(CanStep){
		if(pFS->CanStep(FrameStep, 0)){
			hr = pFS->Step(FrameStep, 0);
		}else{
			pos = 0;
			pMS->GetDuration(&duration);
			hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, &duration, NULL);
		}
	}else{
		pos+=FrameStep;
		if (pos>=duration-FrameStep)
			pos=0;
		hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, &duration, NULL);
	}

	long EvCode = 0;
	hr = pME->WaitForCompletion( 100, &EvCode );

	image = PImage(new Image());
	long cbBuffer = 0;
	hr = pGrabber->GetCurrentBuffer(&cbBuffer, NULL);
	if (FAILED(hr))
	{
		TCHAR szErr[MAX_ERROR_TEXT_LEN];
		DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);
		if (res == 0)
		{
			//wsprintf(szErr, "Unknown Error: 0x%2x", hr);
		};

		AfxMessageBox(szErr,MB_OK | MB_ICONERROR);
	};

	if (pBuffer)
		delete[] pBuffer;
	pBuffer = new char[cbBuffer];

	if (!pBuffer) 
	{
		// Out of memory. Return an error code.
	}
	hr = pGrabber->GetCurrentBuffer(&cbBuffer, (long*)pBuffer);

	AM_MEDIA_TYPE mt;	
	hr = pGrabber->GetConnectedMediaType(&mt);
	if (FAILED(hr)){
		// Return error code.
	}
	// Examine the format block.

	if ((mt.formattype == FORMAT_VideoInfo) && 
		(mt.cbFormat >= sizeof(VIDEOINFOHEADER)) &&
		(mt.pbFormat != NULL) ) {
			pVih = (VIDEOINFOHEADER*)mt.pbFormat; //352 x 288
	}else{
		return Counter();
	};

	////{47504A57-0000-0010-8000-00AA00389B71}
	//
	if(mt.subtype==MEDIASUBTYPE_YUY2){
		EndTiming();
		return Counter();
	}

	if(mt.subtype==MEDIASUBTYPE_RGB24){
		RGB *a = (RGB *)pBuffer;	

		image->pBitmap = PBitmap(new CxImage());
		image->pBitmap->CreateFromArray((BYTE*)pBuffer,pVih->bmiHeader.biWidth, 
			pVih->bmiHeader.biHeight,24,pVih->bmiHeader.biWidth*3,false);

	}

	if(mt.subtype==MEDIASUBTYPE_RGB32){
		RGBA *a = (RGBA *)pBuffer;
		image->pBitmap = PBitmap(new CxImage());
		image->pBitmap->CreateFromArray((BYTE*)pBuffer,pVih->bmiHeader.biWidth, 
			pVih->bmiHeader.biHeight,32,pVih->bmiHeader.biWidth*4,false);

	}
	EndTiming();
	tagMSG message;
	while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	};
	Counter cnt;

	cnt.count=(duration-pos);
	cnt.isInfinity=false;
	cnt.isUnknown=false;

	return cnt;

};
void CaptureReader::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<<FilePath;
		ar<<FrameStep;	
	}else{
		ar>>FilePath;
		ar>>FrameStep;
	}
};



void CaptureReader::run()
{
	HRESULT hr = CoInitialize(NULL);

	if(FilePath.GetLength()==0){
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CString  str("");		
		VideoSelector dlg(FrameStep, FilePath);
		dlg.DoModal();
		if (dlg.FilePath.GetLength()!=0){
			FrameStep = dlg.FrameStep;
			if(FrameStep <=0)
				FrameStep =1;

			if (FilePath!=dlg.FilePath){
				stop();
				run();
			}
			FilePath = dlg.FilePath;			
		}else{
			AfxMessageBox("Select File!");
			return;
		};
	}

	// Initialize the COM library.
	if (FAILED(hr))
		return ;

	// Create the filter graph manager and query for interfaces.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, (void **)&pGraph);

	hr = CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER, 
		IID_IBaseFilter, (void **)&pFile);

	hr = pFile->QueryInterface(IID_IFileSourceFilter,
		reinterpret_cast<void**>(&pFileI));

	hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, reinterpret_cast<void**>(&pSampleGrabber));

	hr = pSampleGrabber->QueryInterface(IID_ISampleGrabber,
		reinterpret_cast<void**>(&pGrabber));

	CComBSTR str(FilePath);
	hr = pFileI->Load(str, 0);

	if (FAILED(hr)){
		AfxMessageBox("Error opening video file!");
		return;
	}

	hr = pGraph->AddFilter(pFile, L"Source");

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pMC);
	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pMS);
	hr = pGraph->QueryInterface(IID_IMediaPosition, (void **)&pMP);
	hr = pGraph->QueryInterface(IID_IVideoFrameStep, (void **)&pFS);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pME);

	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_RGB24;

	hr = pGrabber->SetMediaType(&mt);

	hr = AddFilterByCLSID(pGraph, CLSID_AviSplitter, L"Splitter", &pSplitter);
	hr = ConnectFilters(pGraph, pFile, pSplitter);

	if (hr!=S_OK){
		//mpeg
		pGraph->RemoveFilter(pSplitter);
		SAFE_RELEASE (pSplitter);

		hr = AddFilterByCLSID(pGraph, CLSID_MPEG1Splitter, L"Splitter", &pSplitter);
		hr = ConnectFilters(pGraph, pFile, pSplitter);
		hr = pGraph->AddFilter(pSampleGrabber, L"SampleGrabber");
		hr = ConnectFilters(pGraph, pSplitter, pSampleGrabber);

	}else{
		//avi
		hr = AddFilterByCLSID(pGraph, CLSID_AVIDec, L"Decompressor", &pAviDeocompressor);
		hr = ConnectFilters(pGraph, pSplitter, pAviDeocompressor);	

		hr = pGraph->AddFilter(pSampleGrabber, L"SampleGrabber");
		hr = ConnectFilters(pGraph, pAviDeocompressor, pSampleGrabber);
	}

	hr = AddFilterByCLSID(pGraph, CLSID_VideoRenderer, L"Render", &pVR);
	hr = ConnectFilters(pGraph, pSampleGrabber, pVR);

	hr = pGrabber->SetMediaType(&mt);
	hr = pGrabber->SetBufferSamples(true);



	long eCode;
	hr =  pME->WaitForCompletion(1000, &eCode);

	hr = pMC->Run();
	hr = pMC->Pause();

	hr = pME->WaitForCompletion(1000, &eCode);

	if(S_OK == pFS->CanStep(1,0))
		CanStep=1;


	if(!CanStep){
		pMS->SetTimeFormat( &TIME_FORMAT_FRAME );

		LONGLONG pos=0;		
		LONGLONG duration=0;
		hr = pMS->GetCurrentPosition(&pos);
		hr = pMS->GetDuration(&duration);
	}


	runOk = true;
	return;
};

void CaptureReader::stop(){

	pBuffer = NULL;

	if (pGraph){
		IVideoWindow  *pVW;
		HRESULT hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVW);
		pVW->put_Visible(0);
	}
	SAFE_RELEASE (pSampleGrabber);
	SAFE_RELEASE (pVR);
	SAFE_RELEASE (pFile);
	SAFE_RELEASE (pFileI);
	SAFE_RELEASE (pGrabber);
	SAFE_RELEASE (pSampleGrabber);
	SAFE_RELEASE (pMS);
	SAFE_RELEASE (pMP);
	SAFE_RELEASE (pME);
	SAFE_RELEASE (pFS);

	SAFE_RELEASE (pGraph);
	runOk=false;

	//	CoUninitialize(); // And uninitalize COM
};