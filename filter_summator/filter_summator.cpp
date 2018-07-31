// filter_summator.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_summator.h"
#include "SummatorView.h"

#include <vector>
#include <algorithm>



using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cfilter_summatorApp

BEGIN_MESSAGE_MAP(Cfilter_summatorApp, CWinApp)
END_MESSAGE_MAP()


// Cfilter_summatorApp construction

Cfilter_summatorApp::Cfilter_summatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cfilter_summatorApp object

Cfilter_summatorApp theApp;


// Cfilter_summatorApp initialization

BOOL Cfilter_summatorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API CBIR::Plugin* init(CBIR::Core *pCore)
{	
	Summator *pIG = new Summator(pCore);
    return pIG;
	return NULL;
}


SImagePin::SImagePin(Plugin *plug): ImageOutPin(plug){
	ASSERT(m_plug);
}
Image* SImagePin::Get(bool copy){
	
	if(copy)
		return (dynamic_cast<Summator*>(m_plug))->image;

	Image *pIm = new  Image(*((dynamic_cast<Summator*>(m_plug))->image));

	return pIm;
};


Summator::Summator(Core *pCore): FilterPlugin(pCore){	
	image = NULL;

	sumCount=4;
	timeSpace = 1000/25;

	image = new Image();
	SImagePin *pin = new SImagePin(this);
	ImageInputPin *pin2 = new ImageInputPin(this);
	outPins.push_back(pin);
	inPins.push_back(pin2);
}

Summator::~Summator(){
};
UINT Summator::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SummatorView * dlg = new SummatorView(sumCount, timeSpace);	
	if(dlg->DoModal()==IDOK){
		sumCount = dlg->summCount;
		timeSpace =dlg->timeStep;
	}
	delete dlg;
	return MB_OK;
};
Counter Summator::Next(){
	StartTiming();	
	ImageInputPin *pin;
	pin = (ImageInputPin *) (inPins[0]);
	if (!pin->Linked()){
		AfxMessageBox ("Connect all pins!");
	};

	vector<Image *> images;
	//get 10 images
	for(int i=0; i<sumCount; i++){
		StopTiming();	
		
		//Counter count1 = pin->QueryLinkedPinNext();	

		pCore->AddPinRequest(this, pin);
		pCore->RunPinRequest(this);

		Sleep(timeSpace);
		Beep( 750, 100 );
		pin->Querry();
		ContinueTiming();
		
		

		images.push_back(pin->img);
	};

	//clear previos image
	if(image)
		delete image;
	//create reult Image
	image = new Image(*(images.at(0)));

	//summ images
	//vector<int> rV, gV,bV;
	for(int y=0;y<image->pBitmap->GetHeight();y++)
		for(int x=0;x<image->pBitmap->GetWidth();x++){
			//sum for pixels at (x,y)
			//rV.clear();
			//gV.clear();
			//bV.clear();
			double summR =0;
			double summG =0;
			double summB =0;
			int count = images.size();
			
			RGBQUAD col;
			col.rgbReserved=0;

			for(vector<Image*>::iterator it = images.begin(); it!=images.end();it++){
				RGBQUAD col = (*it)->pBitmap->GetPixelColor(x,y,false);

				summR+= col.rgbRed;
				summG+= col.rgbGreen;
				summB+= col.rgbBlue;
			};

			col.rgbReserved=0;
			col.rgbRed		= (int) summR/count;
			col.rgbBlue		= (int) summG/count;
			col.rgbGreen	= (int) summB/count;

			image->pBitmap->SetPixelColor(x,y,col,false);
		}

	//clear images
	for(vector<Image*>::iterator it = images.begin(); it!=images.end();it++)
		delete (*it);

	images.clear();

	EndTiming();	
	return Counter();//zzzzzzzzzzzzzz
};

void Summator::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<< Summator::timeSpace;
		ar<< Summator::sumCount;
	}else{
		ar>> Summator::timeSpace;
		ar>> Summator::sumCount;
	}
};

void ImageInputPin::Querry(){
	if(m_linkedPin==NULL)
		return;
	ImageOutPin *pin = (ImageOutPin *)m_linkedPin;
	img = pin->Get(false);
	return;
};
