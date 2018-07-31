// filter_color.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_color.h"
#include "fview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cfilter_colorApp

BEGIN_MESSAGE_MAP(Cfilter_colorApp, CWinApp)
END_MESSAGE_MAP()


// Cfilter_colorApp construction

Cfilter_colorApp::Cfilter_colorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cfilter_colorApp object

Cfilter_colorApp theApp;


// Cfilter_colorApp initialization

BOOL Cfilter_colorApp::InitInstance()
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

	pluginType = CORE_PLG_FILTER;
	name = "Color Filter";
	description = "Color Filter";

	outPins.push_back(new SimpleOutImagePin<ColorFilter>(this));	
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);


	H=1;
	S=256;
	V=200;
	mH=100;
	mS=0;
	mV=0;
	val=100;
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
	CFView * dlg = new CFView(tempBMP,H,S,V,mH,mS,mV,val, pCore, NULL);

	if(dlg->DoModal()==IDOK){
		boost::mutex::scoped_lock lock(mutex);
		H=dlg->_h;
		S=dlg->_s;
		V=dlg->_v;

		mH = dlg->mH;
		mS = dlg->mS;
		mV = dlg->mV;
		val= dlg->val;
	};
	delete dlg;
	IsWorking = false;
	return MB_OK;
};
Counter ColorFilter::Next(){
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

	//tempBMP = image->pBitmap;
	
	//transform image
	ColorDistance metric;
	
	//recover fom HSV
	ColorHandler myCol;

	ColorHandler::_RGB myRGB = myCol.HSVtoRGB(ColorHandler::HSV(H,S,V));

	double maxVal = (double)val/1000;
	for(int y=0;y<image->pBitmap->GetHeight();y++)
		for(int x=0;x<image->pBitmap->GetWidth();x++){
			//sum for pixels at (x,y)			
			RGBQUAD col, col1;
			
			col1 = image->pBitmap->GetPixelColor(x,y, false);

			//
			double d = metric.distanceHSV(col1.rgbRed, col1.rgbGreen, col1.rgbBlue,myRGB.Red,myRGB.Green, myRGB.Blue,mH,mS,mV);
			col=col1;

			if(d<maxVal){			
				col.rgbBlue = 255;
				col.rgbRed = 255;
				col.rgbGreen = 255;
			}else{
				col.rgbBlue = 0;
				col.rgbRed = 0;
				col.rgbGreen = 0;
				image->pBitmap->SetPixelColor(x,y,col, false);
			};
			
			image->pBitmap->SetPixelColor(x,y,col, false);
		}

	EndTiming();	
	return pin->counter;
};


void ColorFilter::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<<H;
		ar<<S;
		ar<<V;
		ar<<mH;
		ar<<mS;
		ar<<mV;
		ar<<val;
	}else{
		ar>>H;
		ar>>S;
		ar>>V;
		ar>>mH;
		ar>>mS;
		ar>>mV;
		ar>>val;
	}
};