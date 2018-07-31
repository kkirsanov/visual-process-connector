// filter_resize.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_resize.h"
#include "ResizerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Cfilter_resizeApp, CWinApp)
END_MESSAGE_MAP()


// Cfilter_resizeApp construction

Cfilter_resizeApp::Cfilter_resizeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cfilter_resizeApp object

Cfilter_resizeApp theApp;


// Cfilter_resizeApp initialization

BOOL Cfilter_resizeApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	Resizer *pIG = new Resizer(pCore);
    return pIG;
}

Resizer::Resizer(Core *pCore): Plugin(pCore){

	pluginType = CORE_PLG_FILTER;
	name = "Resize";
	sizeX=256;
	sizeY=256;
	qual=1;
	description = "Resize";

	outPins.push_back(new SimpleOutImagePin<Resizer>(this));	
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);
}

Resizer::~Resizer(){
	if(!outPins.empty())
		delete outPins.front();
	if(!inPins.empty())
		delete inPins.front();
};

UINT Resizer::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	IsWorking = true;
	
	CResizerDlg * dlg = new CResizerDlg(sizeX, sizeY, NULL);

	if(dlg->DoModal()==IDOK){
		boost::mutex::scoped_lock lock(mutex);
		sizeX = dlg->val_x;
		sizeX = dlg->val_y;
	};
	delete dlg;
	IsWorking = false;

	return MB_OK;
};
Counter Resizer::Next(){
	StartTiming();	
	ImageInputPin *pin;
	pin = (ImageInputPin *) (inPins[0]);
	if (!pin->GetLink()){
		EndTiming();
		AfxMessageBox ("Connect all pins!");
		return Counter();
	};
	//get image	

	pCore->AddPinRequest(this, pin);
	pCore->RunPinRequest(this);

	Counter count;

	//create result Image
	
	image = boost::shared_dynamic_cast<Image>(pin->Get());
	tempBMP = PBitmap(new CxImage());
	tempBMP->Copy(*(image->pBitmap.get()));
	tempBMP->Resample(sizeX, sizeY, qual);

	image->pBitmap = tempBMP;

	//recover fom HSV

	EndTiming();	
	return pin->counter;
};


void Resizer::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<<sizeX;
		ar<<sizeY;
		ar<<qual;
	}else{
		ar>>sizeX;
		ar>>sizeY;
		ar>>qual;
	}
};