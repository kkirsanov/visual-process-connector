// Metric_histogram.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Metric_histogram.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CMetric_histogramApp, CWinApp)
END_MESSAGE_MAP()


// CMetric_histogramApp construction

CMetric_histogramApp::CMetric_histogramApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMetric_histogramApp object

CMetric_histogramApp theApp;


// CMetric_histogramApp initialization

BOOL CMetric_histogramApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isMetricPlugin	=	TRUE;

extern "C" PLUGIN_API CBIR::Plugin* init(CBIR::Core *pCore)
{
	HMetric *pMetric = new HMetric(pCore);
    return pMetric;

};


void HMetric::ShowPropertiesDilog(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//MetricDialog Dlg();
	//Dlg.DoModal();	
};


Logic* LOutPin::Get(bool copy){
	Logic *log = new Logic();
	ImageIndex *index1, *index2;
	log->l = (dynamic_cast<HMetric*>(m_plug))->compare;//(index1, index2);
	ImageInputPin *ipin = (ImageInputPin *)(dynamic_cast<HMetric*>(m_plug))->inPins[1];
	log->path = ipin->img->path;

	return log;
};


UINT HMetric::DoModal(){
//	MetricDialog Dlg();
//	Dlg.DoModal();
	return 0;
};
void ImageInputPin::Querry(){
	if(m_linkedPin==NULL)
		return;
	ImageOutPin *pin = (ImageOutPin *)m_linkedPin;
	img = pin->Get(true);
	return;
};
Counter ImageInputPin::QueryLinkedPinNext(){
	if(m_linkedPin==NULL)
		return Counter();
	ImageOutPin *pin = (ImageOutPin *)m_linkedPin;
	return pin->QueryNew();
};

Counter HMetric::Next(){
	StartTiming();

	ImageInputPin *pin1, *pin2;

	pin1 = (ImageInputPin *) (inPins[0]);
	pin2 = (ImageInputPin *) (inPins[1]);

	if (!pin1->Linked() || !pin2->Linked()){
		AfxMessageBox ("Connect all pins!");
	};

	StopTiming();
	Counter count1 = pin1->QueryLinkedPinNext();
	Counter count2 = pin2->QueryLinkedPinNext();
	if (count1.count <=0 || count2.count <=0)
		return Counter();

	pin1->Querry();
	pin2->Querry();
	
	ContinueTiming();
	ImageIndex *index1 =NULL;
	ImageIndex *index2 =NULL;

	index1 = HMetric::GetIndex(pin1->img->pBitmap); 
	index2 = HMetric::GetIndex(pin2->img->pBitmap);
	
	compare = Compare(index1, index2);
	if (index1)
		delete index1;
	if (index2)
		delete index2;
	EndTiming();

	tagMSG message;
	while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		};

	if(count1.count>count2.count)
		return count1;
	return count2;
};

HMetric::HMetric(Core *pCore): MetricPlugin(pCore){	
	inPins.push_back(new ImageInputPin(this));
	inPins.push_back(new ImageInputPin(this));

	outPins.push_back(new LOutPin(this));
};
ImageIndex* HMetric::GetIndex(CxImage *bitmap){
	MetricData *pIndex = new MetricData;

	int x,y;

	if(bitmap->GetType()!=2){
		for(int i=0;i<16;i++)
			pIndex->H[i]=0;
	}else{
		ColorHandler myCol;
		unsigned int counter=0;
		double k = (double) 16/360;
		RGBQUAD col;
		for (y=0;y<bitmap->GetHeight();y++){
			double rowMedian = 0;
			for (x=0;x<bitmap->GetWidth();x++){
				col= bitmap->GetPixelColor(x,y, false);

				ColorHandler::HSV myHSV = myCol.RGBtoHSV(ColorHandler::_RGB(col.rgbRed, col.rgbGreen, col.rgbBlue));
				int idx = myHSV.Hue * k;
				pIndex->H[idx]+=1;
				counter++;
			};
		};
	};
	
	double max = pIndex->H[0];
	for(int i=0;i<16;i++){
		if(max<pIndex->H[i])
			max=pIndex->H[i];
	};
	for(int i=0;i<16;i++){
		pIndex->H[i] = (double) pIndex->H[i]/max;
	}

	return pIndex;
};

double HMetric::Compare(ImageIndex* index1, ImageIndex* index2){	
	MetricData *ind1, *ind2;
	ind1 = (MetricData*) index1;
	ind2 = (MetricData*) index2;

	double distanse = 0.0;
	double summ =0;
	for(int i=0;i<16;i++){
		summ+= pow((ind1->H[i] - ind2->H[i]),2);
	};
	return sqrt(summ);	
};