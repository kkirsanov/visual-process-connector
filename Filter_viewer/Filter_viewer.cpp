// Filter_viewer.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Filter_viewer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ImageVD.h"

BEGIN_MESSAGE_MAP(CFilter_viewerApp, CWinApp)
END_MESSAGE_MAP()


// CFilter_viewerApp construction

CFilter_viewerApp::CFilter_viewerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFilter_viewerApp object

CFilter_viewerApp theApp;


// CFilter_viewerApp initialization

BOOL CFilter_viewerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	ImageViewer *pIG = new ImageViewer(pCore);
    return pIG;
	return NULL;
}



ImageViewer::ImageViewer(Core *pCore): Plugin(pCore){	
	pluginType = CORE_PLG_FILTER;
	name = "Image viewer";
	description = "Image viewer";
	IsDialog=false;

	outPins.push_back(new SimpleOutImagePin<ImageViewer>(this));
	dlg = 0;
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);
}

ImageViewer::~ImageViewer(){
	delete outPins.at(0);
	delete inPins.at(0);
};
UINT ImageViewer::DoModal(){
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
		
	boost::mutex::scoped_lock scoped_lock(mutex);
	IsWorking=true;
	if(image.get())
		if(image->pBitmap.get())
			if(!IsDialog){
				dlg = new CImageVD(image->pBitmap, &IsDialog, pCore);
				dlg->Create(IDD_DIALOGV);
				dlg->ShowWindow(SW_SHOWDEFAULT);
				tagMSG message;			
				while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
						::TranslateMessage(&message);
						::DispatchMessage(&message);			
				}
			}
	IsWorking=false;
	return MB_OK;
	
};
Counter ImageViewer::Next(){
	
	StartTiming();	
	ImageInputPin *pin = (ImageInputPin *) (inPins[0]);
	if (!pin->IsLinked()){
		EndTiming();
		AfxMessageBox ("Connect all pins!");
		IsWorking = false;
		return Counter();
	};
	Counter count1;


	pCore->AddPinRequest(this, pin);
	pCore->RunPinRequest(this);
	count1 = pin->counter;

	PData d= pin->Get(true);
	image = boost::shared_dynamic_cast<Image>(d);

	CImageVD *pD =(CImageVD *)dlg;
	if(dlg){
		if (!IsDialog){
			//delete dlg;
			pD=NULL;
			dlg=NULL;
		}else{
			//boost::mutex::scoped_lock scoped_lock(pD->fillMutex);
			pD->Sync(image->pBitmap);
		}
	}

	EndTiming();	
	

	return count1;
};