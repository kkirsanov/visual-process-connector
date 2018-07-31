// segment_baisic.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <math.h>
#include "segment_baisic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Csegment_baisicApp

BEGIN_MESSAGE_MAP(Csegment_baisicApp, CWinApp)
END_MESSAGE_MAP()


// Csegment_baisicApp construction

Csegment_baisicApp::Csegment_baisicApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Csegment_baisicApp object

Csegment_baisicApp theApp;


// Csegment_baisicApp initialization

BOOL Csegment_baisicApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	SegmentFilter *pIG = new SegmentFilter(pCore);
    return pIG;
}

SegmentFilter::SegmentFilter(Core *pCore): Plugin(pCore){	

	pluginType = CORE_PLG_FILTER;
	name = "Image segment";
	description = "Image segment";

	outPins.push_back(new SimpleOutImagePin<SegmentFilter>(this));
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);

}

SegmentFilter::~SegmentFilter(){

};
UINT SegmentFilter::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return MB_OK;
};
Counter SegmentFilter::Next(){
	StartTiming();	
	ImageInputPin *pin;
	pin = (ImageInputPin *) (inPins[0]);
	if (!pin->GetLink()){
		AfxMessageBox ("Connect all pins!");
		
		return Counter();
	};



	pCore->AddPinRequest(this, pin);
	pCore->RunPinRequest(this);
	Counter count;	

	image = boost::shared_dynamic_cast<Image>(pin->Get());

	//create result Image	
	tempBMP = PBitmap(new CxImage(*image->pBitmap));
	
	int xSize=image->pBitmap->GetWidth()/32;
	int ySize=xSize=image->pBitmap->GetHeight()/32;
	
	xSize=64;
	ySize=16;

	tempBMP->Resample(xSize,ySize,2);

	ColorDistance metric;
	
	//recover fom HSV
	ColorHandler myCol;

	ColorDistance dist;
	int	CurCol = 0;
	int toY = tempBMP->GetHeight();
	int toX = tempBMP->GetWidth();

	double xDiv = (double)image->pBitmap->GetWidth()/(xSize);
	double yDiv = (double)image->pBitmap->GetHeight()/(ySize);

	int H=100;
	int S=0;
	int V=0;

	for(int y=0;y<toY ;y++){
		for(int x=0;x<toX ;x++){
			RGBQUAD c1 = tempBMP->GetPixelColor(x,y,false);
			RGBQUAD c2 = tempBMP->GetPixelColor(x+1,y,false);
			double d = dist.distanceHSV(c1, c2, H,S,V);

			RGBQUAD median;
			median.rgbBlue = (c1.rgbBlue + c2.rgbBlue) /2;
			median.rgbRed = (c1.rgbRed + c2.rgbRed) /2;
			median.rgbGreen = (c1.rgbGreen + c2.rgbGreen) /2;

			if (d<=0.07)
				tempBMP->SetPixelColor(x,y,median);
		}
			
	}
	for(int y=0;y<toY ;y++){
		for(int x=0;x<toX ;x++){
			RGBQUAD c1 = tempBMP->GetPixelColor(x,y,false);
			RGBQUAD c2 = tempBMP->GetPixelColor(x,y+1,false);
			double d = dist.distanceHSV(c1, c2, H,S,V);

			RGBQUAD median;
			median.rgbBlue = (c1.rgbBlue + c2.rgbBlue) /2;
			median.rgbRed = (c1.rgbRed + c2.rgbRed) /2;
			median.rgbGreen = (c1.rgbGreen + c2.rgbGreen) /2;

			if (d<=0.1)
				tempBMP->SetPixelColor(x,y,median);
		}
			
	}

	image->pBitmap->Colorize(0,0);
	for(int y=0;y<toY ;y++){
		for(int x=0;x<toX ;x++){
			int sX = x*xDiv;
			int eX = (x+1)*(xDiv);
			int sY = y*yDiv;
			
			RGBQUAD c1 = tempBMP->GetPixelColor(x,y,false);
			RGBQUAD c2=tempBMP->GetPixelColor(x-1,y,false);
			
			double d1 = dist.distanceHSV(c1, c2, H,S,V);

			c1 = tempBMP->GetPixelColor(x,y,false);
			c2=tempBMP->GetPixelColor(x+1,y,false);
			
			double d2 = dist.distanceHSV(c1, c2, H,S,V);
			if (d1+d2<0.1){
				image->pBitmap->DrawLine(sX,eX,sY-2,sY-2, tempBMP->GetPixelColor(x,y,false));
				image->pBitmap->DrawLine(sX,eX,sY-1,sY-1, tempBMP->GetPixelColor(x,y,false));
				image->pBitmap->DrawLine(sX,eX,sY+1,sY+1, tempBMP->GetPixelColor(x,y,false));
				image->pBitmap->DrawLine(sX,eX,sY+2,sY+2, tempBMP->GetPixelColor(x,y,false));

			}
			

			if (d1+d2<0.07){
				RGBQUAD r;
				r.rgbBlue=255;
				r.rgbRed=255;
				r.rgbGreen=255;
				RGBQUAD c = tempBMP->GetPixelColor(x,y,false);


			}
			image->pBitmap->DrawLine(sX,eX,sY,sY, tempBMP->GetPixelColor(x,y,false));				

		};
		//break;
	};

	EndTiming();	
	return pin->counter;
};

