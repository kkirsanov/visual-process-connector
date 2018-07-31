// filter_stereo.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_stereo.h"

#include "StereoDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// Cfilter_stereoApp

BEGIN_MESSAGE_MAP(Cfilter_stereoApp, CWinApp)
END_MESSAGE_MAP()


// Cfilter_stereoApp construction

Cfilter_stereoApp::Cfilter_stereoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cfilter_stereoApp object

Cfilter_stereoApp theApp;


// Cfilter_stereoApp initialization

BOOL Cfilter_stereoApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	ColorFilter *pIG = new ColorFilter(pCore);
	return pIG;
}


ColorFilter::ColorFilter(Core *pCore): Plugin(pCore){	
	image1 = NULL;
	image2 = NULL;

	pluginType = CORE_PLG_FILTER;
	name = "Stereo view";
	description = "Stereo view";

//	outPins.push_back(new SimpleOutImagePin<ColorFilter>(this));	
	
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);

	InPin *pPin2 = new ImageInputPin(this);
	inPins.push_back(pPin2);

}

ColorFilter::~ColorFilter(){
	if(!outPins.empty())
		delete outPins.front();
	if(!inPins.empty())
		delete inPins.front();
};
UINT ColorFilter::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	IsWorking = true;
	StereoDlg * dlg = new StereoDlg(NULL);
	dlg->pImg1=image1->pBitmap;
	dlg->pImg1=image2->pBitmap;

	if(dlg->DoModal()==IDOK){
	//	boost::mutex::scoped_lock lock(mutex);	
	};
	delete dlg;
	IsWorking = false;
	return MB_OK;
};
Counter ColorFilter::Next(){
	StartTiming();	
	ImageInputPin *pin1,*pin2;
	pin1 = (ImageInputPin *) (inPins[0]);
	pin2 = (ImageInputPin *) (inPins[1]);

	if (!pin1->GetLink()){
		EndTiming();
//		AfxMessageBox ("Connect all pins!");
		return Counter();
	};
	//get image	

	pCore->AddPinRequest(this, pin1);
	//pCore->AddPinRequest(this, pin2);
	pCore->RunPinRequest(this);

	pCore->AddPinRequest(this, pin2);
	//pCore->AddPinRequest(this, pin2);
	pCore->RunPinRequest(this);

	Counter count;

	//create result Image
	if (image1)
		delete image1;
	if (image2)
		delete image2;
	image1 = (Image*)pin1->Get();
	image2 = (Image*)pin2->Get();

	tempBMP1.Copy(*(image1->pBitmap));
	tempBMP2.Copy(*(image2->pBitmap));
	//transform image


	EndTiming();	
	return pin1->counter;
};


void ColorFilter::Serialize(CArchive& ar){

};