// filter_marker.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_marker.h"
#include <algorithm>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cfilter_markerApp

BEGIN_MESSAGE_MAP(Cfilter_markerApp, CWinApp)
END_MESSAGE_MAP()


// Cfilter_markerApp construction

Cfilter_markerApp::Cfilter_markerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cfilter_markerApp object

Cfilter_markerApp theApp;


// Cfilter_markerApp initialization

BOOL Cfilter_markerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	Marker *pIG = new Marker(pCore);
	return pIG;
}

Marker::Marker(Core *pCore): Plugin(pCore){	
	image = NULL;

	pluginType = CORE_PLG_FILTER;
	name = "Marker Detection";
	description = "Marker Detection";

	outPins.push_back(new SimpleOutImagePin<Marker>(this));	
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);
}

Marker::~Marker(){
	if(!outPins.empty())
		delete outPins.front();
	if(!inPins.empty())
		delete inPins.front();
};
UINT Marker::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	IsWorking = true;
	IsWorking = false;
	return MB_OK;
};
Counter Marker::Next(){
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
	if (image)
		delete image;
	Image *Pimage = (Image*) pin->Get(true);

	image = new Image(*Pimage);
	
	int w = image->pBitmap->GetWidth();
	int h = image->pBitmap->GetHeight();
	int size = h*w;

	vector<unsigned> oMap;
	oMap.reserve(size);
	oMap.resize(size);
	std::fill(oMap.begin(), oMap.end(), 0);

	for(int y=0;y<h;y++){
		for(int x=0;x<w;x++){
			//проверяем и нтенсивность.
			RGBQUAD color = image->pBitmap->GetPixelColor(x,y);
			BYTE val = color.rgbBlue;
			if(val>128){
				//если достаточно высока, то смотрим окружающие пиксели карты.
				//если они 0, то создаем новый объект. Если ненулевые, то устанавливаем на маске
				//новый объект
				

			}
		}
	}

		


	//transform image
	
	EndTiming();	
	return pin->counter;
};


void Marker::Serialize(CArchive& ar){
	if(ar.IsStoring()){
	}else{
	}
};