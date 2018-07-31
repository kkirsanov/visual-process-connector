// filter_panorama.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_panorama.h"
#include "PanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cfilter_panoramaApp

BEGIN_MESSAGE_MAP(Cfilter_panoramaApp, CWinApp)
END_MESSAGE_MAP()


// Cfilter_panoramaApp construction

Cfilter_panoramaApp::Cfilter_panoramaApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cfilter_panoramaApp object

Cfilter_panoramaApp theApp;


// Cfilter_panoramaApp initialization

BOOL Cfilter_panoramaApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API CBIR::Plugin* init(CBIR::Core *pCore)
{	
	Panorama *pIG = new Panorama(pCore);
    return pIG;
}


SImagePin::SImagePin(Plugin *plug): ImageOutPin(plug){
	ASSERT(m_plug);
}
Image* SImagePin::Get(bool copy){
	
	if(copy)
		return (dynamic_cast<Panorama*>(m_plug))->image;

	Image *pIm = new  Image(*((dynamic_cast<Panorama*>(m_plug))->image));

	return pIm;
};


Panorama::Panorama(Core *pCore): FilterPlugin(pCore){	
	image = NULL;

	sumCount=6;
	timeSpace = 2000;

	image = new Image();
	SImagePin *pin = new SImagePin(this);

	ImageInputPin *pin2 = new ImageInputPin(this);
	ImageInputPin *pin3 = new ImageInputPin(this);
	outPins.push_back(pin);
	inPins.push_back(pin2);
	inPins.push_back(pin3);
}

Panorama::~Panorama(){
	if(image)
		delete image;
	delete outPins[0];
	delete inPins[0];
	delete inPins[1];
};
UINT Panorama::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	PanDlg * dlg = new PanDlg(sumCount, timeSpace, &images, pCore);	
	if(dlg->DoModal()==IDOK){
		sumCount = dlg->summCount;
		timeSpace =dlg->timeStep;
	}
	delete dlg;
	return MB_OK;
};
Counter Panorama::Next(){
	StartTiming();	
	outPins.push_back(new SimpleOutImagePin<Resizer>(this));	
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);
	InPin *pPin2 = new ImageInputPin(this);
	inPins.push_back(pPin2);

	if (!pin1->Linked() || !pin2->Linked()){
		AfxMessageBox ("Connect all pins!");
	};

	
	//cklear old Images
	for(vector<Image*>::iterator it = images.begin(); it!=images.end();it++)
		delete (*it);
	images.clear();

//get new images
	pin1->QueryLinkedPinNext();
	pin1->Querry();
	for(int i=0; i<sumCount; i++){
		StopTiming();	
		Counter count1 = pin2->QueryLinkedPinNext();	
		Sleep(timeSpace);
		//Beep( 750, 100 );
		pin2->Querry();
		ContinueTiming();
		//
		int x = pin2->img->pBitmap->GetWidth()/2;
		int y = pin2->img->pBitmap->GetHeight()/2;
		
		RGBQUAD col;
		col.rgbBlue=255;
		col.rgbRed=255;
		col.rgbGreen=255;
		
		int s = rand()%30 + 10;
		pin2->img->pBitmap->DrawLine(x-s,x+s, y,y,col);
		pin2->img->pBitmap->DrawLine(x,x,y-s, y+s,col);

		images.push_back(new Image(pin2->img->pBitmap));
	};

	//clear previos image
	if(image)
		delete image;
	//create reult Image
	image = new Image(images.at(0)->pBitmap);

	//summ images
	//integrate

	//clear images
	//images.clear();

	EndTiming();	

	tagMSG message;
	while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		};
	return Counter();//zzzzzzzzzzzzzz
};

void Panorama::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<< Panorama::timeSpace;
		ar<< Panorama::sumCount;
	}else{
		ar>> Panorama::timeSpace;
		ar>> Panorama::sumCount;
	}
};