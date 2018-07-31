// filter_mask.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_mask.h"
#include "Maskview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cfilter_maskApp

BEGIN_MESSAGE_MAP(Cfilter_maskApp, CWinApp)
END_MESSAGE_MAP()


// Cfilter_maskApp construction


Cfilter_maskApp::Cfilter_maskApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cfilter_maskApp object

Cfilter_maskApp theApp;


// Cfilter_maskApp initialization

BOOL Cfilter_maskApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	MaskFilter *pIG = new MaskFilter(pCore);
    return pIG;

}
extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

MaskFilter::MaskFilter(Core *pCore): Plugin(pCore), window1(3), window2(3), window3(3){	
	image = PImage(new Image());
	
	pluginType = CORE_PLG_FILTER;
	name = "Mask Filter";
	description = "Mask Filter";

	outPins.push_back(new SimpleOutImagePin<MaskFilter>(this));
	
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);

	/*SImagePin *pin = new SImagePin((Plugin*)this);*/
	/*ImageInputPin *pin2 = new ImageInputPin((Plugin*)this);*/
	
	/*outPins.push_back(pin);*/
	//outPins.push_back(new SImagePin1((Plugin*)this));
	//outPins.push_back(new SImagePin2((Plugin*)this));
	//outPins.push_back(new SImagePin3((Plugin*)this));
	
	
	/*inPins.push_back(pin2);*/


	ColorModel = "HSL";

	Mask* pMask = new Mask(3);
	pMask->divisor=9;
	pMask->name = "Average 3";
	pMask->mask.at(0).at(0) = 1;
	pMask->mask.at(0).at(1) = 1;
	pMask->mask.at(0).at(2) = 1;

	pMask->mask.at(1).at(0) = 1;
	pMask->mask.at(1).at(1) = 1;
	pMask->mask.at(1).at(2) = 1;

	pMask->mask.at(2).at(0) = 1;
	pMask->mask.at(2).at(1) = 1;
	pMask->mask.at(2).at(2) = 1;

	palette.push_back(pMask);

	pMask = new Mask(5);
	pMask->divisor=25;
	pMask->name = "Average 5";
	pMask->mask.at(0).at(0) = 1;
	pMask->mask.at(0).at(1) = 1;
	pMask->mask.at(0).at(2) = 1;
	pMask->mask.at(0).at(3) = 1;
	pMask->mask.at(0).at(4) = 1;


	pMask->mask.at(1).at(0) = 1;
	pMask->mask.at(1).at(1) = 1;
	pMask->mask.at(1).at(2) = 1;
	pMask->mask.at(1).at(3) = 1;
	pMask->mask.at(1).at(4) = 1;

	pMask->mask.at(2).at(0) = 1;
	pMask->mask.at(2).at(1) = 1;
	pMask->mask.at(2).at(2) = 1;
	pMask->mask.at(2).at(3) = 1;
	pMask->mask.at(2).at(4) = 1;

	pMask->mask.at(3).at(0) = 1;
	pMask->mask.at(3).at(1) = 1;
	pMask->mask.at(3).at(2) = 1;
	pMask->mask.at(3).at(3) = 1;
	pMask->mask.at(3).at(4) = 1;

	pMask->mask.at(4).at(0) = 1;
	pMask->mask.at(4).at(1) = 1;
	pMask->mask.at(4).at(2) = 1;
	pMask->mask.at(4).at(3) = 1;
	pMask->mask.at(4).at(4) = 1;

	palette.push_back(pMask);


	pMask = new Mask(3);
	pMask->divisor=16;
	pMask->name = "Blur 3";
	pMask->mask.at(0).at(0) = 1;
	pMask->mask.at(0).at(1) = 2;
	pMask->mask.at(0).at(2) = 1;

	pMask->mask.at(1).at(0) = 2;
	pMask->mask.at(1).at(1) = 4;
	pMask->mask.at(1).at(2) = 2;

	pMask->mask.at(2).at(0) = 1;
	pMask->mask.at(2).at(1) = 2;
	pMask->mask.at(2).at(2) = 1;

	palette.push_back(pMask);

	pMask = new Mask(3);
	pMask->divisor=1;
	pMask->bais=128;
	pMask->name = "Embos 3";
	pMask->mask.at(0).at(0) = -1;
	pMask->mask.at(0).at(1) = 0;
	pMask->mask.at(0).at(2) = 0;

	pMask->mask.at(1).at(0) = 0;
	pMask->mask.at(1).at(1) = 2;
	pMask->mask.at(1).at(2) = 0;

	pMask->mask.at(2).at(0) = 0;
	pMask->mask.at(2).at(1) = 0;
	pMask->mask.at(2).at(2) = -1;

	palette.push_back(pMask);

	pMask = new Mask(3);
	pMask->divisor=1;
	pMask->bais=0;
	pMask->name = "Unsharp 3";
	pMask->mask.at(0).at(0) = 0;
	pMask->mask.at(0).at(1) = -1;
	pMask->mask.at(0).at(2) = 0;

	pMask->mask.at(1).at(0) = -1;
	pMask->mask.at(1).at(1) = 5;
	pMask->mask.at(1).at(2) = -1;

	pMask->mask.at(2).at(0) = 0;
	pMask->mask.at(2).at(1) = -1;
	pMask->mask.at(2).at(2) = 0;

	palette.push_back(pMask);

	pMask = new Mask(3);
	pMask->divisor=1;
	pMask->bais=0;
	pMask->name = "Laplasian-1 3";
	pMask->mask.at(0).at(0) = -1;
	pMask->mask.at(0).at(1) = -1;
	pMask->mask.at(0).at(2) = -1;

	pMask->mask.at(1).at(0) = -1;
	pMask->mask.at(1).at(1) = 8;
	pMask->mask.at(1).at(2) = -1;

	pMask->mask.at(2).at(0) = -1;
	pMask->mask.at(2).at(1) = -1;
	pMask->mask.at(2).at(2) = -1;

	palette.push_back(pMask);
	pMask = new Mask(3);
	pMask->divisor=1;
	pMask->bais=0;
	pMask->name = "Laplasian+1 3";
	pMask->mask.at(0).at(0) = 1;
	pMask->mask.at(0).at(1) = 1;
	pMask->mask.at(0).at(2) = 1;

	pMask->mask.at(1).at(0) = 1;
	pMask->mask.at(1).at(1) = -8;
	pMask->mask.at(1).at(2) = 1;

	pMask->mask.at(2).at(0) = 1;
	pMask->mask.at(2).at(1) = 1;
	pMask->mask.at(2).at(2) = 1;

	palette.push_back(pMask);
	pMask = new Mask(3);
	pMask->divisor=0;
	pMask->bais=0;
	pMask->name = "Gradient H 3";
	pMask->mask.at(0).at(0) = -1;
	pMask->mask.at(0).at(1) = -2;
	pMask->mask.at(0).at(2) = -1;

	pMask->mask.at(1).at(0) = 0;
	pMask->mask.at(1).at(1) = 0;
	pMask->mask.at(1).at(2) = 0;

	pMask->mask.at(2).at(0) = -1;
	pMask->mask.at(2).at(1) = 2;
	pMask->mask.at(2).at(2) = 1;

	palette.push_back(pMask);
	pMask = new Mask(3);
	pMask->divisor=0;
	pMask->bais=0;
	pMask->name = "Gradient V 3";
	pMask->mask.at(0).at(0) = -1;
	pMask->mask.at(0).at(1) = 0;
	pMask->mask.at(0).at(2) = 1;

	pMask->mask.at(1).at(0) = -2;
	pMask->mask.at(1).at(1) = 0;
	pMask->mask.at(1).at(2) = 2;

	pMask->mask.at(2).at(0) = -1;
	pMask->mask.at(2).at(1) = 0;
	pMask->mask.at(2).at(2) = 1;

	palette.push_back(pMask);


	pMask = new Mask(5);
	pMask->divisor=3;
	pMask->name = "Unsharp 5";
	pMask->mask.at(0).at(0) = -1;
	pMask->mask.at(0).at(1) = 0;
	pMask->mask.at(0).at(2) = 0;
	pMask->mask.at(0).at(3) = 0;
	pMask->mask.at(0).at(4) = -1;


	pMask->mask.at(1).at(0) = 0;
	pMask->mask.at(1).at(1) = 0;
	pMask->mask.at(1).at(2) = 0;
	pMask->mask.at(1).at(3) = 0;
	pMask->mask.at(1).at(4) = 0;

	pMask->mask.at(2).at(0) = 0;
	pMask->mask.at(2).at(1) = 0;
	pMask->mask.at(2).at(2) = 7;
	pMask->mask.at(2).at(3) = 0;
	pMask->mask.at(2).at(4) = 0;

	pMask->mask.at(3).at(0) = 0;
	pMask->mask.at(3).at(1) = 0;
	pMask->mask.at(3).at(2) = 0;
	pMask->mask.at(3).at(3) = 0;
	pMask->mask.at(3).at(4) = 0;

	pMask->mask.at(4).at(0) = -1;
	pMask->mask.at(4).at(1) = 0;
	pMask->mask.at(4).at(2) = 0;
	pMask->mask.at(4).at(3) = 0;
	pMask->mask.at(4).at(4) = -1;

	palette.push_back(pMask);
}

MaskFilter::~MaskFilter(){
	for(vector<Mask*>::iterator it = palette.begin(); it!=palette.end();it++){
		delete *it;
	}
	palette.clear();

};
UINT MaskFilter::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	boost::mutex::scoped_lock scoped_lock(mutex);
	IsWorking = true;
	Maskview *maskDlg = new Maskview(&palette, window1, window2, window3, ColorModel);
	INT_PTR z = maskDlg->DoModal();
	delete maskDlg;
	IsWorking = false;

	return MB_OK;
};

void MaskFilter::ApplayMask(CxImage *pImg, Mask &window){
	CxImage tmpImg(*pImg,true,false,false);

	for(int y=window.mask.size()/2;y<pImg->GetHeight() - window.mask.size()/2;y++){
		for(int x=window.mask.size()/2;x<pImg->GetWidth()-window.mask.size()/2;x++){
			long res=0;

			int xCount=0;
			int yCount=0;

			BYTE col;
			for(int i=y - window.mask.size()/2; i<=y + window.mask.size()/2; i++){
				xCount=0;
				for(int j=x - window.mask.size()/2; j<=x + window.mask.size()/2; j++){
					col = tmpImg.GetPixelIndex(j,i);

					res+= col*(window.mask.operator [](xCount).operator [](yCount));

					xCount++;
				}
				yCount++;
			}
			
			if (window.divisor>0)
				res	= res/window.divisor;
			res+=window.bais;

			if(res<0)
				res=0;
			if (res>255)
				res=255;
			col=res;

			pImg->SetPixelIndex(x,y, col);
		}
	}
}

Counter MaskFilter::Next(){
	StartTiming();

	Counter counter;
	ImageInputPin *pin;
	pin = (ImageInputPin *) (inPins[0]);
	if (!pin->GetLink()){
		AfxMessageBox ("Connect all pins!");
		EndTiming();
		return Counter();
	};
	//get image	
	

	Counter count1;
	pCore->AddPinRequest(this, pin);
	pCore->RunPinRequest(this);
	counter = pin->counter;

	image = boost::shared_dynamic_cast<Image>(pin->Get());

	CxImage *bmp1=new CxImage();
	CxImage *bmp2=new CxImage();	
	CxImage *bmp3=new CxImage();

	if(ColorModel==CString("HSL")) image->pBitmap->SplitHSL(bmp1,bmp2,bmp3);
	if(ColorModel==CString("RGB")) image->pBitmap->SplitRGB(bmp1,bmp2,bmp3);
	if(ColorModel==CString("XYZ")) image->pBitmap->SplitXYZ(bmp1,bmp2,bmp3);
	if(ColorModel==CString("YIQ")) image->pBitmap->SplitYIQ(bmp1,bmp2,bmp3);
	if(ColorModel==CString("YUV")) image->pBitmap->SplitYUV(bmp1,bmp2,bmp3);

	//process img1 with Window1
	if (window1.mask.size()>2)
		ApplayMask(bmp1, window1);
	if (window2.mask.size()>2)
		ApplayMask(bmp2, window2);
	if (window3.mask.size()>2)
		ApplayMask(bmp3, window3);

	if(ColorModel==CString("HSL")){
		RGBQUAD color;
		RGBQUAD sourceColor;
		for(long y=0; y<image->pBitmap->GetHeight(); y++){
			for(long x=0; x<image->pBitmap->GetWidth(); x++){
				sourceColor.rgbRed		= bmp1->GetPixelIndex(x,y);
				sourceColor.rgbGreen	= bmp2->GetPixelIndex(x,y);
				sourceColor.rgbBlue		= bmp3->GetPixelIndex(x,y);

				color = image->pBitmap->HSLtoRGB(sourceColor);
				image->pBitmap->SetPixelColor(x, y, color);
			}
		}
	};
	if(ColorModel==CString("RGB")){
		RGBQUAD sourceColor;
		for(long y=0; y<image->pBitmap->GetHeight(); y++){
			for(long x=0; x<image->pBitmap->GetWidth(); x++){
				sourceColor.rgbRed		= bmp1->GetPixelIndex(x,y);
				sourceColor.rgbGreen	= bmp2->GetPixelIndex(x,y);
				sourceColor.rgbBlue		= bmp3->GetPixelIndex(x,y);

				image->pBitmap->SetPixelColor(x, y, sourceColor);
			}
		}
	};
	if(ColorModel==CString("XYZ")){
		RGBQUAD color;
		RGBQUAD sourceColor;
		for(long y=0; y<image->pBitmap->GetHeight(); y++){
			for(long x=0; x<image->pBitmap->GetWidth(); x++){
				sourceColor.rgbRed		= bmp1->GetPixelIndex(x,y);
				sourceColor.rgbGreen	= bmp2->GetPixelIndex(x,y);
				sourceColor.rgbBlue		= bmp3->GetPixelIndex(x,y);

				color = image->pBitmap->XYZtoRGB(sourceColor);
				image->pBitmap->SetPixelColor(x, y, color);
			}
		}
	};
	if(ColorModel==CString("YIQ")){
		RGBQUAD color;
		RGBQUAD sourceColor;
		for(long y=0; y<image->pBitmap->GetHeight(); y++){
			for(long x=0; x<image->pBitmap->GetWidth(); x++){
				sourceColor.rgbRed		= bmp1->GetPixelIndex(x,y);
				sourceColor.rgbGreen	= bmp2->GetPixelIndex(x,y);
				sourceColor.rgbBlue		= bmp3->GetPixelIndex(x,y);

				color = image->pBitmap->YIQtoRGB(sourceColor);
				image->pBitmap->SetPixelColor(x, y, color);
			}
		}
	};

	if(ColorModel==CString("YUV")){
		RGBQUAD color;
		RGBQUAD sourceColor;
		for(long y=0; y<image->pBitmap->GetHeight(); y++){
			for(long x=0; x<image->pBitmap->GetWidth(); x++){
				sourceColor.rgbRed		= bmp1->GetPixelIndex(x,y);
				sourceColor.rgbGreen	= bmp2->GetPixelIndex(x,y);
				sourceColor.rgbBlue		= bmp3->GetPixelIndex(x,y);

				color = image->pBitmap->YUVtoRGB(sourceColor);
				image->pBitmap->SetPixelColor(x, y, color);
			}
		}
	};
	delete bmp1;
	delete bmp2;
	delete bmp3;

	EndTiming();
	return counter;
};

void MaskFilter::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<<ColorModel;
		window1.Serialize(ar);		
		window2.Serialize(ar);
		window3.Serialize(ar);
	}else{
		ar>>ColorModel;
		window1.Serialize(ar);		
		window2.Serialize(ar);
		window3.Serialize(ar);
	}
};