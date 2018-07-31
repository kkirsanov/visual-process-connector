// metric_wavelet.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "metric_wavelet.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Cmetric_waveletApp, CWinApp)
END_MESSAGE_MAP()


// Cmetric_waveletApp construction

Cmetric_waveletApp::Cmetric_waveletApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cmetric_waveletApp object

Cmetric_waveletApp theApp;


// Cmetric_waveletApp initialization

BOOL Cmetric_waveletApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isMetricPlugin	=	TRUE;

extern "C" PLUGIN_API CBIR::Plugin* init(CBIR::Core *pCore)
{
	WaveletMetric *pMetric = new WaveletMetric(pCore);
    return pMetric;
}

void WaveletMetric::ShowPropertiesDilog(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//MetricDialog Dlg();
//	Dlg.DoModal();	
};

Logic* LOutPin::Get(bool copy){
	Logic *log = new Logic();
	//ImageIndex *index1, *index2;
	log->l = (dynamic_cast<WaveletMetric*>(m_plug))->compare;//(index1, index2);
	ImageInputPin *ipin = (ImageInputPin *)(dynamic_cast<WaveletMetric*>(m_plug))->inPins[1];
	log->path = ipin->img->path;
	return log;
};


UINT WaveletMetric::DoModal(){
//	MetricDialog Dlg();
//	Dlg.DoModal();
	return 0;
}
void ImageInputPin::Querry(){
	if(m_linkedPin==NULL)
		return;
	ImageOutPin *pin = (ImageOutPin *)m_linkedPin;
	img = pin->Get();
	img->pBitmap->Resample(128,128);

	return;
};
Counter WaveletMetric::Next(){	
	StartTiming();
	ImageInputPin *pin1, *pin2;

	pin1 = (ImageInputPin *) (inPins[0]);
	pin2 = (ImageInputPin *) (inPins[1]);

	if (!pin1->Linked() || !pin2->Linked()){
		AfxMessageBox ("Connect all pins!");
	};

	StopTiming();

	pCore->AddPinRequest(this, pin1);
	pCore->AddPinRequest(this, pin2);
	pCore->RunPinRequest(this);

	pin1->Querry();
	pin2->Querry();
	ContinueTiming();
	
	ImageIndex *index1 =NULL;
	ImageIndex *index2 =NULL;

	Counter count1 = pin1->count;
	Counter count2 = pin2->count;

	index1 = WaveletMetric::GetIndex(pin1->img->pBitmap); 
	index2 = WaveletMetric::GetIndex(pin2->img->pBitmap);
	
	compare = Compare(index1, index2);
	if (index1)
		delete index1;
	if (index2)
		delete index2;

	EndTiming();
	//if(count1.count>count2.count)
	//	return count2;
	tagMSG message;
	while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		};
	return count2;
};
WaveletMetric::WaveletMetric(Core *pCore): MetricPlugin(pCore){	
	inPins.push_back(new ImageInputPin(this));
	inPins.push_back(new ImageInputPin(this));

	outPins.push_back(new LOutPin(this));
};

ImageIndex* WaveletMetric::GetIndex(CxImage *bitmap){
	MetricData *pIndex = new MetricData;	
	int x,y;

	if(bitmap->GetColorType()!=2){
	}else{
		Wavelet wave(bitmap);

		pIndex->data.resize(wave.wavelet->dat.size());

		for(multivecI::iterator it = pIndex->data.begin(); it!= pIndex->data.end();it++){
			(*it).resize(wave.wavelet->dat.size());
		};

		double max=0;
		for(int i =0;i<wave.wavelet->dat.size();i++)
			for(int j =0;j<wave.wavelet->dat.size();j++){
				if(i!=j!=0){
					if(max<wave.wavelet->dat[i][j])
						max=wave.wavelet->dat[i][j];
				}
			}
	
		for(int i =0;i<wave.wavelet->dat.size();i++)
			for(int j =0;j<wave.wavelet->dat.size();j++){				
				pIndex->data[i][j] = (int)(wave.wavelet->dat[i][j]);///max)*32;
			}
		
		wave.wavelet->dat.clear();
	};
	return pIndex;

};
double WaveletMetric::Compare(ImageIndex* index1, ImageIndex* index2){	
	MetricData *ind1, *ind2;
	ind1 = (MetricData*) index1;
	ind2 = (MetricData*) index2;
	if (ind1->data.size()==0)
		return 999;
	if (ind2->data.size()==0)
		return 999;

	double distanse = 0.0;
	
	int max=0;
	
	for(int y=0;y<32;y++){
		for(int x=0;x<32;x++){
		if ((x!=0)&&(y!=0))
			if (max<ind1->data[x][y])
				max=ind1->data[x][y];
		}
	}
	
	for(int y=0;y<32;y++){
		for(int x=0;x<32;x++){
		if ((x!=0)&&(y!=0))
			if (ind1->data[x][y]>max/4 || ind1->data[x][y]<(-max/4))
				distanse+= pow((double)(ind1->data[x][y] - ind2->data[x][y]),2);
		}
	}

	return sqrt(distanse);
};

Wavelet::Wavelet(CxImage *bmp){
	//get image
	wavelet = NULL;

	int h = bmp->GetWidth();

	int size = 4;
	multivec myVec;
	myVec.resize(h);
	for(multivec::iterator it = myVec.begin(); it!= myVec.end();it++){
		(*it).resize(h);
	};
	ColorHandler myCol;
	for(int y=0;y<h;y++){
		for(int x=0;x<h;x++){
		
			
			RGBQUAD col;
			col = bmp->GetPixelColor(x,y, false);
			ColorHandler::HSV myHSV = myCol.RGBtoHSV(ColorHandler::_RGB(col.rgbRed, col.rgbGreen,col.rgbBlue));
			//myVec[y][x]=myHSV.Hue;
			myVec[y][x]=myHSV.value;
		};
	}
	wavelet = new Wavelet2D(myVec);
	myVec.clear();
};