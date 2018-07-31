
#include "stdafx.h"
#include "Input_capture.h"
#include "CameraSelectodGlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CInput_captureApp, CWinApp)
END_MESSAGE_MAP()


// CInput_captureApp construction

CInput_captureApp::CInput_captureApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CInput_captureApp object

CInput_captureApp theApp;


// CInput_captureApp initialization

BOOL CInput_captureApp::InitInstance()
{
	CWinApp::InitInstance();
	HRESULT hr = CoInitialize(NULL);

	return TRUE;
}

extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isInputPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
 	CaptureReader *pCr = new CaptureReader(pCore);
    return pCr;
}


CaptureReader::CaptureReader(Core *pCore): Plugin(pCore){	
	counter = 0;
	runOk = false;
	IsWorking = false;
	pluginType = CORE_PLUGIN_TYPE::CORE_PLG_INPUT;
	
	SimpleOutImagePin<CaptureReader> *pPin = new SimpleOutImagePin<CaptureReader>(this);
	outPins.push_back(pPin);

	name = string("Capture Reader");
	description = string("Capture Reader");

	//inPins.push_back(new MeasureInputPin(this));

	cameraName="";

	testB=NULL;
	pBuffer = NULL;
	pCapture= NULL;
};

CaptureReader::~CaptureReader(){
	delete outPins.at(0);
	if (runOk)
		stop();
};
UINT CaptureReader::DoModal(){
	CString  str("");
	CString  str2("");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCameraSelectodGlg dlg(&str, &str2);
	dlg.DoModal();
	if (str.GetLength()!=0){
		cameraName = str;
		typeName = str2;
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
		Sleep(2000);
	}

	long cbBuffer = 0;
	HRESULT	hr = pGrabber->GetCurrentBuffer(&cbBuffer, NULL);
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

	pBuffer = new char[cbBuffer];

	if (!pBuffer) 
	{
		// Out of memory. Return an error code.
	}
	long eCode;
	pEvent->WaitForCompletion(1000, &eCode);
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
		EndTiming();
		return Counter();
	};

	if(mt.subtype==MEDIASUBTYPE_YUY2){
		EndTiming();
		return Counter();
	}

//	if (image.get())
//		images.push_back(image);
		
	image = PImage(new Image());

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

	tagMSG message;
	if (pBuffer)
		delete[] pBuffer;

	
//	if(images.size()>20)		
//		images.pop_front();

	EndTiming();
	return Counter();
	
};
void CaptureReader::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<<cameraName;
		ar<<typeName;
	}else{
		ar>>cameraName;
		ar>>typeName;
	}
};

/*
Image* CaptureImagePin::Get(bool copy){
	CaptureReader *pPlug = (dynamic_cast<CaptureReader*>(m_plug));
	Image *pImg = pPlug->image;
		
	if( pPlug->IsWorking)
		pImg = pPlug->images.back();

	if(copy)
		return pImg;
	Image *pIm = new  Image(*pImg);
	return pIm;
};
*/

CString CaptureReader::PMT2String(AM_MEDIA_TYPE *pMT)
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

void CaptureReader::run(){
	if(cameraName.GetLength()==0){
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CString  str("");		
		CString  str2("");	
		CCameraSelectodGlg *dlg = new CCameraSelectodGlg(&str, &str2);
		dlg->DoModal();
		if (str.GetLength()!=0){
			cameraName = str;
			typeName = str2;
			delete dlg;
		}else{
			delete dlg;
			return;
		};
	}
	HRESULT hr = CoInitialize(NULL);
    // Initialize the COM library.
   if (FAILED(hr))
        return ;

	// Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                        IID_IGraphBuilder, (void **)&pGraph);
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, 
                        IID_ICaptureGraphBuilder2, (void **)&pCapture);


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

		CString str(varName.bstrVal);

		if (SUCCEEDED(hr)){
			if(str==cameraName){
				hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
							CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, 
							(void **)&pCapture);
				if (SUCCEEDED(hr)){
					pCapture->SetFiltergraph(pGraph);
				};
				// Creating VideoCapture filter
				hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCap);
				if (SUCCEEDED(hr)){
					hr = pGraph->AddFilter(pCap, NULL);
				}			
			
				pPropBag->Release();			
				pMoniker->Release();
				pPropBag= NULL;
				pMoniker=NULL;
				break;
			};
		}
		pPropBag->Release();
		pMoniker->Release();
	}

	

	hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, reinterpret_cast<void**>(&pSampleGrabber));
	if (FAILED(hr))
	{
		// Error handling is omitted for clarity;
	}

	hr = pSampleGrabber->QueryInterface(IID_ISampleGrabber,
		reinterpret_cast<void**>(&pGrabber));

	hr = pGraph->AddFilter(pSampleGrabber, L"SampleGrabber");



	
	//Scan for avaliable Mediatypes
	
	IEnumMediaTypes *pEnuminator;
	IPin *outPin=NULL;
	outPin = GetPin(pCap,PINDIR_OUTPUT);
	outPin->EnumMediaTypes(&pEnuminator);

	AM_MEDIA_TYPE *pMT;
	BOOL isOK = FALSE;
	do{
		hr = pEnuminator->Next(1, &pMT, NULL);
		if(hr != S_OK)
			break;
		CString smt = PMT2String (pMT);

		if (smt == typeName){
			isOK = TRUE;
			break;
		}
	}while(1);
	
	if (!isOK){
		return;
	}
	//setting Format;

	IAMStreamConfig *pConf;
	hr=pCapture->FindInterface(NULL,&MEDIATYPE_Video, pCap,   IID_IAMStreamConfig, (void **)&pConf);
	hr=pConf->SetFormat(pMT);
	
	SAFE_RELEASE(pConf)

	VIDEOINFOHEADER *pVI = (VIDEOINFOHEADER *)pMT->pbFormat;
	if ((pVI->bmiHeader.biCompression ==BI_RGB) && (pMT->subtype==MEDIASUBTYPE_RGB24 || pMT->subtype==MEDIASUBTYPE_RGB32)){ 
		//uncompressed data		
		ConnectFilters(pGraph, pCap, pSampleGrabber);
	}else{//Compressed
		AM_MEDIA_TYPE mt;
		ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
		mt.majortype = MEDIATYPE_Video;
		mt.subtype = MEDIASUBTYPE_RGB24;
		hr = pGrabber->SetMediaType(&mt);
		hr = AddFilterByCLSID(pGraph, CLSID_AVIDec, L"decompresser", &pAviDeocompressor);
		hr = ConnectFilters(pGraph, pCap, pAviDeocompressor);
		hr = ConnectFilters(pGraph, pAviDeocompressor, pSampleGrabber);
		
		hr = pGrabber->SetMediaType(&mt);

		ConnectFilters(pGraph, pCap, pSampleGrabber);
	}

    #ifdef _DEBUG
		hr = AddToRot(pGraph, &dwRegister);
	#endif

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);


	hr = pGrabber->SetBufferSamples(true);
	runOk = true;
	hr = pControl->Run();
	Sleep(100);
	
	
	FILTER_STATE state;

	hr = pSampleGrabber->GetState(100, &state);
	if (FAILED(hr))
		throw "Oblom";


	while(state!=State_Running){
		hr = pSampleGrabber->GetState(100, &state);
		if (FAILED(hr))
			throw "Oblom";
	}
 
};
void CaptureReader::stop(){
	pControl->Stop();
	pControl->Release();
	//pVideoRender->Release();
	pBuffer = NULL;

	SAFE_RELEASE (pCap);
	SAFE_RELEASE (pCapture);
	SAFE_RELEASE (pControl);
	SAFE_RELEASE (pSampleGrabber);
	SAFE_RELEASE (pGrabber);
	SAFE_RELEASE (pGraph);

	CoUninitialize(); // And reinitialize COM
};