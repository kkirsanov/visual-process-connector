/*
Global media type identifier 	GUID
MEDIASUBTYPE_I420 	30323449-0000-0010-8000-00AA00389B71
MEDIASUBTYPE_IYUV 	56555949-0000-0010-8000-00AA00389B71
MEDIASUBTYPE_RGB1 	e436eb78-524f-11ce-9f53-0020af0ba770
MEDIASUBTYPE_RGB24 	e436eb7d-524f-11ce-9f53-0020af0ba770
MEDIASUBTYPE_RGB32 	e436eb7e-524f-11ce-9f53-0020af0ba770
MEDIASUBTYPE_RGB4 	e436eb79-524f-11ce-9f53-0020af0ba770
MEDIASUBTYPE_RGB555 	e436eb7c-524f-11ce-9f53-0020af0ba770
MEDIASUBTYPE_RGB565 	e436eb7b-524f-11ce-9f53-0020af0ba770
MEDIASUBTYPE_RGB8 	e436eb7a-524f-11ce-9f53-0020af0ba770
MEDIASUBTYPE_UYVY 	59565955-0000-0010-8000-00AA00389B71
MEDIASUBTYPE_VIDEOIMAGE 	1d4a45f2-e5f6-4b44-8388-f0ae5c0e0c37
MEDIASUBTYPE_YUY2 	32595559-0000-0010-8000-00AA00389B71
MEDIASUBTYPE_YV12 	31313259-0000-0010-8000-00AA00389B71
MEDIASUBTYPE_YVU9 	39555659-0000-0010-8000-00AA00389B71
MEDIASUBTYPE_YVYU 	55595659-0000-0010-8000-00AA00389B71
WMFORMAT_MPEG2Video 	e06d80e3-db46-11cf-b4d1-00805f6cbbea
WMFORMAT_Script 	5C8510F2-DEBE-4ca7-BBA5-F07A104F8DFF
WMFORMAT_VideoInfo 	05589f80-c356-11ce-bf01-00aa0055595a
WMFORMAT_WaveFormatEx 	05589f81-c356-11ce-bf01-00aa0055595a
WMFORMAT_WebStream 	da1e6b13-8359-4050-b398-388e965bf00c
WMMEDIASUBTYPE_ACELPnet 	00000130-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_Base 	00000000-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_DRM 	00000009-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_MP3 	00000055-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_MP43 	3334504D-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_MP4S 	5334504D-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_M4S2 	3253344D-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_P422 	32323450-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_MPEG2_VIDEO 	e06d8026-db46-11cf-b4d1-00805f6cbbea
WMMEDIASUBTYPE_MSS1 	3153534D-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_MSS2 	3253534D-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_PCM 	00000001-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WebStream 	776257d4-c627-41cb-8f81-7ac7ff1c40cc
WMMEDIASUBTYPE_WMAudio_Lossless 	00000163-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMAudioV2 	00000161-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMAudioV7 	00000161-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMAudioV8 	00000161-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMAudioV9 	00000162-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMSP1 	0000000A-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMV1 	31564D57-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMV2 	32564D57-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMV3 	33564D57-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMVA 	41564D57-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WMVP 	50564D57-0000-0010-8000-00AA00389B71
WMMEDIASUBTYPE_WVP2 	32505657-0000-0010-8000-00AA00389B71
WMMEDIATYPE_Audio 	73647561-0000-0010-8000-00AA00389B71
WMMEDIATYPE_FileTransfer 	D9E47579-930E-4427-ADFC-AD80F290E470
WMMEDIATYPE_Image 	34A50FD8-8AA5-4386-81FE-A0EFE0488E31
WMMEDIATYPE_Script 	73636d64-0000-0010-8000-00AA00389B71
WMMEDIATYPE_Text 	9BBA1EA7-5AB2-4829-BA57-0940209BCF3E
WMMEDIATYPE_Video 	73646976-0000-0010-8000-00AA00389B71
WMSCRIPTTYPE_TwoStrings 	82f38a70-c29f-11d1-97ad-00a0c95ea850
*/
// CameraSelectodGlg.cpp : implementation file
//

#include "stdafx.h"
#include "Input_capture.h"
#include "CameraSelectodGlg.h"


// CCameraSelectodGlg dialog

IMPLEMENT_DYNAMIC(CCameraSelectodGlg, CDialog)

void CCameraSelectodGlg::StopAndClear(){
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	if(pMC!=NULL)
		pMC->Stop();
	
	SAFE_RELEASE(pMC);
	SAFE_RELEASE(pME);
	SAFE_RELEASE(pVW);
	SAFE_RELEASE(pBA);
	SAFE_RELEASE(pBV);
	SAFE_RELEASE(pMS);
	SAFE_RELEASE(pMP);
	SAFE_RELEASE(pFS);
	
	SAFE_RELEASE(pDevEnum);
	SAFE_RELEASE(pEnum);
	//SAFE_RELEASE(pMoniker);
	SAFE_RELEASE(pCap);
	SAFE_RELEASE(pCapture);
	SAFE_RELEASE(pVideoRender);

	SAFE_RELEASE(pSampleGrabber);
	SAFE_RELEASE(pGrabber);
	SAFE_RELEASE(pCGB);
	SAFE_RELEASE(pGB);

};

CCameraSelectodGlg::CCameraSelectodGlg(CString* name,CString* type, CWnd* pParent /*=NULL*/)
	: CDialog(CCameraSelectodGlg::IDD, pParent)
{
	m_str = name;
	*m_str=CString("");
	m_type = type;
	*m_type=CString("");
	HRESULT hr = CoInitialize(NULL);

	pGB= NULL;
	pMC= NULL;
	pME= NULL;
	pVW= NULL;
	pBA= NULL;
	pBV= NULL;
	pMS= NULL;
	pMP= NULL;
	pFS= NULL;
	pCGB=NULL;

	pDevEnum= NULL;
	pEnum= NULL;
	pMoniker= NULL;
	pCap= NULL;
	pCapture= NULL;
	pVideoRender=NULL;

	pSampleGrabber= NULL;
	pGrabber= NULL;

	pMoniker=NULL;

//	captureList.ResetContent();
}

CCameraSelectodGlg::~CCameraSelectodGlg()
{
	StopAndClear();
}

void CCameraSelectodGlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, captureList);
	DDX_Control(pDX, IDC_LISTMS, ModeSelectorCtrl);
}


BEGIN_MESSAGE_MAP(CCameraSelectodGlg, CDialog)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &CCameraSelectodGlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST3, &CCameraSelectodGlg::OnLbnSelchangeList3)
	ON_LBN_SELCHANGE(IDC_LISTMS, &CCameraSelectodGlg::OnLbnSelchangeListms)
END_MESSAGE_MAP()


// CCameraSelectodGlg message handlers

int CCameraSelectodGlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CCameraSelectodGlg::OnInitDialog(){
	BOOL z = CDialog::OnInitDialog();
	captureList.ResetContent();

	HRESULT hr;	

// Create the System Device Enumerator.
	 hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum));
	if (SUCCEEDED(hr)){
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,
			&pEnum, 0);
		if(hr!=S_OK){
			pDevEnum->Release();
			pDevEnum=NULL;
			return FALSE;
		};
	}

	pMoniker = NULL;
	pCap	=	NULL;

	while (pEnum->Next(1, &pMoniker, NULL) == S_OK){
		IPropertyBag *pPropBag;
		 hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
			(void**)(&pPropBag));
		if (FAILED(hr)){
			pMoniker->Release();
			continue;  // Skip this one, maybe the next one will work.
		} 
		// Find the description or friendly name.
		VARIANT varName;
		VariantInit(&varName);
		hr = pPropBag->Read(L"Description", &varName, 0);
		if (FAILED(hr)){
			hr = pPropBag->Read(L"FriendlyName", &varName, 0);
		}
		if (SUCCEEDED(hr)){
			captureList.AddString(CString(varName.bstrVal));
			VariantClear(&varName); 
		}
		pPropBag->Release();
		pMoniker->Release();
	}
	return z;

}
void CCameraSelectodGlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}

void CCameraSelectodGlg::OnBnClickedOk()
{
	CString str;
	int selInd =captureList.GetCurSel();
	
	int n = captureList.GetTextLen( selInd );
	captureList.GetText( selInd, str.GetBuffer(n) );
	str.ReleaseBuffer();
	*m_str= str;

	selInd =ModeSelectorCtrl.GetCurSel();
	n = ModeSelectorCtrl.GetTextLen( selInd );
	ModeSelectorCtrl.GetText( selInd, str.GetBuffer(n) );
	str.ReleaseBuffer();
	*m_type= str;

	OnOK();
}
CString PMT2String(AM_MEDIA_TYPE *pMT)
{
	CString str;
	VIDEOINFOHEADER *pVI = (VIDEOINFOHEADER *)pMT->pbFormat;
	if((pMT->majortype == MEDIATYPE_Video)){
		
		str.Format("%d x %d | ",pVI->bmiHeader.biWidth,pVI->bmiHeader.biHeight );
		if (pVI->bmiHeader.biCompression ==BI_RGB){
			str += CString ("RGB");
			if (pMT->subtype == MEDIASUBTYPE_RGB555)
				str += CString ("555");
			if (pMT->subtype == MEDIASUBTYPE_RGB565)
				str += CString ("565");
			if (pMT->subtype == MEDIASUBTYPE_RGB24)
				str += CString ("24");
			if (pMT->subtype == MEDIASUBTYPE_RGB32)
				str += CString ("32");
		}else{
			if (pMT->subtype == MEDIASUBTYPE_YUY2)
				str += CString ("YUY2");
			if (pMT->subtype == MEDIASUBTYPE_YUYV)
				str += CString ("YUYV");
			if (pMT->subtype == MEDIASUBTYPE_YVYU)
				str += CString ("YVYU");
			if (pMT->subtype == MEDIASUBTYPE_Y211)
				str += CString ("Y211");
			if (pMT->subtype == MEDIASUBTYPE_YV12)
				str += CString ("YV12");
		}
	};
	return str;
}
void CCameraSelectodGlg::OnLbnSelchangeList3()
{
	HRESULT hr;
	StopAndClear();
	ModeSelectorCtrl.ResetContent();
	//Fill avaliable modes for this camera
	
	//scan cameras
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                        IID_IGraphBuilder, (void **)&pGB);

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum));
	if (SUCCEEDED(hr)){
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,
			&pEnum, 0);
	}
	//list avaliable Cpture;
	pMoniker = NULL;
	pCap	=	NULL;
	
	bool isOk = false;
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK){
		IPropertyBag *pPropBag = NULL;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
			(void**)(&pPropBag));
		if (FAILED(hr)){
			pMoniker->Release();
			pMoniker=NULL;
			continue;  // Skip this one, maybe the next one will work.
		} 
		// Find the description or friendly name.
		VARIANT varName;
		VariantInit(&varName);
		hr = pPropBag->Read(L"Description", &varName, 0);
		if (FAILED(hr)){
			hr = pPropBag->Read(L"FriendlyName", &varName, 0);
		}
		if (SUCCEEDED(hr)){
			CString str(varName.bstrVal);
			CString selectedName("");
			VariantClear(&varName);
			
			int selInd = captureList.GetCurSel();
			
			int n = captureList.GetTextLen( selInd );
			captureList.GetText( selInd, selectedName.GetBuffer(n) );
			selectedName.ReleaseBuffer();

			if(str==selectedName){
				hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
							CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, 
							(void **)&pCapture);
				if (SUCCEEDED(hr)){
					pCapture->SetFiltergraph(pGB);
				};
				// Creating VideoCapture filter
				hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCap);
				if (SUCCEEDED(hr)){
					hr = pGB->AddFilter(pCap, NULL);
				}			
			
				pPropBag->Release();			
				pMoniker->Release();
				pPropBag= NULL;
				pMoniker=NULL;
				
				isOk = true;
				break;

			};
		}
		pPropBag->Release();
		pMoniker->Release();
		pPropBag= NULL;
		pMoniker=NULL;
	}

	if(!isOk)
		return;

	IEnumMediaTypes *pEnuminator;
	IPin *outPin=NULL;
	outPin = GetPin(pCap,PINDIR_OUTPUT);
	outPin->EnumMediaTypes(&pEnuminator);
	AM_MEDIA_TYPE *pMT;
	do{
		hr = pEnuminator->Next(1, &pMT, NULL);
		if(hr != S_OK)
			break;
		CString str = PMT2String(pMT);
		int z = ModeSelectorCtrl.AddString(str);
	}while(1);

	StopAndClear();
}
void CCameraSelectodGlg::OnLbnSelchangeListms()
{
	HRESULT hr;
	StopAndClear();
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                        IID_IGraphBuilder, (void **)&pGB);
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, 
                        IID_ICaptureGraphBuilder2, (void **)&pCGB);

	hr = pCGB->SetFiltergraph(pGB);

	//capture
	// Create the System Device Enumerator.
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum));
	if (SUCCEEDED(hr)){
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,
			&pEnum, 0);
	}

	//list avaliable Cpture;
	pMoniker = NULL;
	pCap	=	NULL;
	
	bool isOk = false;
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK){
		IPropertyBag *pPropBag = NULL;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
			(void**)(&pPropBag));
		if (FAILED(hr)){
			pMoniker->Release();
			pMoniker=NULL;
			continue;  // Skip this one, maybe the next one will work.
		} 
		// Find the description or friendly name.
		VARIANT varName;
		VariantInit(&varName);
		hr = pPropBag->Read(L"Description", &varName, 0);
		if (FAILED(hr)){
			hr = pPropBag->Read(L"FriendlyName", &varName, 0);
		}
		if (SUCCEEDED(hr)){
			CString str(varName.bstrVal);
			CString selectedName("");
			VariantClear(&varName);
			
			int selInd = captureList.GetCurSel();
			
			int n = captureList.GetTextLen( selInd );
			captureList.GetText( selInd, selectedName.GetBuffer(n) );
			selectedName.ReleaseBuffer();

			if(str==selectedName){
				hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
							CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, 
							(void **)&pCapture);
				if (SUCCEEDED(hr)){
					pCapture->SetFiltergraph(pGB);
				};
				// Creating VideoCapture filter
				hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCap);
				if (SUCCEEDED(hr)){
					hr = pGB->AddFilter(pCap, NULL);
				}			
			
				pPropBag->Release();			
				pMoniker->Release();
				pPropBag= NULL;
				pMoniker=NULL;
				
				isOk = true;
				break;

			};
		}
		pPropBag->Release();
		pMoniker->Release();
		pPropBag= NULL;
		pMoniker=NULL;
	}

	if(!isOk)
		return;
	hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, reinterpret_cast<void**>(&pSampleGrabber));
	if (FAILED(hr))
	{
		// Error handling is omitted for clarity;
	}

	hr = pSampleGrabber->QueryInterface(IID_ISampleGrabber,
		reinterpret_cast<void**>(&pGrabber));

	hr = pGB->AddFilter(pSampleGrabber, L"SampleGrabber");

	/*********************************************/
	hr = AddFilterByCLSID(pGB, CLSID_VideoRenderer, L"Render", &pVideoRender);	

	//Get Pin 
	IEnumMediaTypes *pEnuminator;
	IPin *outPin=NULL;
	outPin = GetPin(pCap,PINDIR_OUTPUT);
	outPin->EnumMediaTypes(&pEnuminator);
	AM_MEDIA_TYPE *pMT;
	BOOL isOK = FALSE;
	
	//Get Selected Type
	CString SelectedType;
	int selInd =ModeSelectorCtrl.GetCurSel();
	int n = ModeSelectorCtrl.GetTextLen( selInd );
	ModeSelectorCtrl.GetText( selInd, SelectedType.GetBuffer(n) );
	SelectedType.ReleaseBuffer();

	do{
		hr = pEnuminator->Next(1, &pMT, NULL);
		if(hr != S_OK)
			break;
		CString smt = PMT2String (pMT);

		if (smt == SelectedType){
			hr = pGrabber->SetMediaType(pMT);
			isOK = TRUE;
			break;
		}
	}while(1);
	if (!isOk){
		StopAndClear();
		return;
	}
	hr = pGrabber->SetMediaType(pMT);

	//SettingUotputType;
	IAMStreamConfig *pConf;	

	
	hr=pCGB->FindInterface(NULL,&MEDIATYPE_Video, pCap,   IID_IAMStreamConfig, (void **)&pConf);
	hr=pConf->SetFormat(pMT);
	
	SAFE_RELEASE(pConf)
	hr = ConnectFilters(pGB, pCap, pSampleGrabber);
	if(hr!= S_OK){
		StopAndClear();
		AfxMessageBox("Unsupported!");
		return;
	}

	hr = ConnectFilters(pGB, pSampleGrabber, pVideoRender);

	DWORD dwRegister;


	hr = pGB->QueryInterface(IID_IMediaControl, (void **)&pMC);
	hr = pGB->QueryInterface(IID_IMediaEventEx, (void **)&pME);

	hr = pGB->QueryInterface(IID_IVideoWindow, (void **)&pVW);
	pVW->put_Owner((OAHWND)CCameraSelectodGlg::GetSafeHwnd());
    pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	//pVW->put_WindowStyle(WS_CHILD);
	

	CRect myRect;
	CCameraSelectodGlg::GetWindowRect(myRect);
	int x = myRect.Width()-260;
	int y = myRect.Height()-90;
	pVW->put_Top(10);
	pVW->put_Left(220);

	pVW->put_Width(x);
	pVW->put_Height(y);

	#ifdef _DEBUG
		hr = AddToRot(pGB, &dwRegister);
	#endif


	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;

	// Find the current bit depth.
	HWND hwnd = ::GetDesktopWindow();

	HDC hdc = ::GetDC(hwnd);
	
	int iBitDepth = ::GetDeviceCaps(hdc, BITSPIXEL);
	::ReleaseDC(hwnd, hdc);

	// Set the media type.
	mt.majortype = MEDIATYPE_Video;
	switch (iBitDepth)
	{
	case 8:
		mt.subtype = MEDIASUBTYPE_RGB8;
		break;
	case 16:
		mt.subtype = MEDIASUBTYPE_RGB555;
		break;
	case 24:
		mt.subtype = MEDIASUBTYPE_RGB24;
		break;
	case 32:
		mt.subtype = MEDIASUBTYPE_RGB32;
		break;
	default:
		return ;
	}

	mt = *pMT;

	mt.subtype = MEDIASUBTYPE_RGB24;	
	
	hr = pGrabber->SetMediaType(&mt);	
	hr = pGrabber->SetBufferSamples(TRUE);

	hr = pMC->Run();
}
