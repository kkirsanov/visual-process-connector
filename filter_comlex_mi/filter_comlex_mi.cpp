// filter_comlex_mi.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_comlex_mi.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cfilter_comlex_miApp

BEGIN_MESSAGE_MAP(Cfilter_comlex_miApp, CWinApp)
END_MESSAGE_MAP()

// Cfilter_comlex_miApp construction

Cfilter_comlex_miApp::Cfilter_comlex_miApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only Cfilter_comlex_miApp object

Cfilter_comlex_miApp theApp;


// Cfilter_comlex_miApp initialization

BOOL Cfilter_comlex_miApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	ComplexFilter *p = new ComplexFilter(pCore);
//	Ñomplex *pIG = new Ñomplex(pCore);
	return p;
}

ComplexFilter::ComplexFilter(Core *pCore): Plugin(pCore){	
	image = PImage(new Image());
	pluginType = CORE_PLG_FILTER;
	name = "US+Image";
	description = "US+Image";

	outPins.push_back(new SimpleOutImagePin<ComplexFilter>(this));
	dlg = 0;	
	inPins.push_back(new MeasureInputPin(this));
	inPins.push_back(new ImageInputPin(this));
	value = PMeasure(new Measure());
	//logger.open("c:\\1.log", ios_base::out);
}

ComplexFilter::~ComplexFilter(){
	delete outPins.at(0);
	delete inPins.at(0);
	delete inPins.at(1);
};

UINT ComplexFilter::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	boost::mutex::scoped_lock scoped_lock(mutex);

	return MB_OK;
};

Counter ComplexFilter::Next(){
	StartTiming();	
	ImageInputPin *pin = (ImageInputPin *) (inPins[1]);
	MeasureInputPin *pin2 = (MeasureInputPin *) (inPins[0]);
	if (!pin->IsLinked() ||!pin2->IsLinked()){
		EndTiming();
		AfxMessageBox ("Connect all pins!");
		IsWorking = false;
		return Counter();
	};
	Counter count1;

	pCore->AddPinRequest(this, pin);
	pCore->AddPinRequest(this, pin2);
	pCore->RunPinRequest(this);


	image = boost::shared_dynamic_cast<Image>(pin->Get());
	value = boost::shared_dynamic_cast<Measure>(pin2->Get());

	count1 = pin->counter;
	//combine
	RGBQUAD col;
	col.rgbBlue=255;
	col.rgbRed=255;
	col.rgbGreen=255;

	vector<int> x;
	vector<int> y;

	x.resize(15);
	y.resize(15);

	int MAXRadius = __min(image->pBitmap->GetWidth(),image->pBitmap->GetHeight()) /2; //ïîëîâèíà èêðàíà
	double distance = value->values[0]; //in cm

	//scale distance to MAXRadius
	double k = 250/*maxint*/ / MAXRadius;

	double rDistance = distance /k;
 
	double radius =  MAXRadius - rDistance;

	int ii=0;
	for(double i=0;i<2*3.1415+1;i+= 2*3.1415/10){
		x.at(ii) = cos(i)*radius;
		y.at(ii) = sin(i)*radius;

		ii++;
	}

	int xc = image->pBitmap->GetWidth()/2;
	int yc = image->pBitmap->GetHeight()/2;
	
	int xl = xc-value->values[0]/2;
	int xr = xc+value->values[0]/2;

	for(int i =0;i<9;i++){
		image->pBitmap->DrawLine(xc + x.at(i),xc + x.at(i+1),yc + y.at(i),yc + y.at(i+1), col);	
	}
	if (value->values.size()>1){
		//logger << value->values[1] << endl;
		double angle = value->values[1];
		//double radius = MAXRadius;
		//180 3.1415
		int x2 = cos(3.1415*angle/180)*MAXRadius;
		int y2 = sin(3.1415*angle/180)*MAXRadius;

		image->pBitmap->DrawLine(xc, x2+xc, yc ,y2+yc, col);
		//image->pBitmap->DrawLine(xc, cos((3.1415/180)*angle)*radius, yc ,sin((3.1415/180)*angle)*radius, col2);
	}
	//image->pBitmap->DrawLine(xl,xr,yc,yc, col);

	EndTiming();	

	return count1;
};
