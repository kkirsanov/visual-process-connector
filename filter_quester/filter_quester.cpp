// filter_quester.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "filter_quester.h"
#include "QV.h"
#include <boost/thread/xtime.hpp>
#include <boost/thread/condition.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Cfilter_questerApp, CWinApp)
END_MESSAGE_MAP()


// Cfilter_questerApp construction

Cfilter_questerApp::Cfilter_questerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cfilter_questerApp object

Cfilter_questerApp theApp;


// Cfilter_questerApp initialization

BOOL Cfilter_questerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	Quester *pIG = new Quester(pCore);
    return pIG;

}

Quester::Quester(Core *pCore): Plugin(pCore){	
	sumCount=100;
	timeSpace = 1000/25;
	CanGo=false;
	ImageInputPin *pin2 = new ImageInputPin(this);
	name = "Clock";
	description = "Clock";
	Plugin::pluginType = CORE_PLG_LOGIC;
	inPins.push_back(pin2);
	th=0;
}

Quester::~Quester(){
	CanGo=false;
	//if(th)
		
};

UINT Quester::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	QV * dlg = new QV(sumCount, timeSpace);	
	if(dlg->DoModal()==IDOK){
		sumCount = dlg->summCount;
		timeSpace =dlg->timeStep;
	}
	delete dlg;
	
	return MB_OK;
};
void Quester::trueNext(){
	ImageInputPin *pin;
	pin = (ImageInputPin *) (inPins[0]);
	if (!pin->GetLink()){
		CanGo=true;
		AfxMessageBox ("Connect all pins!");
	};

	boost::xtime xt;

	//get 10 images
	if (sumCount>0){
		for(int i=0; i<sumCount; i++){
			if(CanGo)
				return;//zzzzzzzzzzzzzz		

			if (!pin->GetLink())
				return ;//zzzzzzzzzzzzzz
			pCore->AddPinRequest(this, pin);
			pCore->RunPinRequest(this);
			
			if(timeSpace){
				//Sleep(timeSpace);
				xtime_get(&xt, boost::TIME_UTC);
				xt.nsec += (long)timeSpace*1000000;
				boost::thread::sleep(xt);
			}
			
			//Beep( 750, 300 );
			tagMSG message;
			
			while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			};
			
			
		};
	}else{
		while(1){
			if (!pin->GetLink())
				CanGo=true;
			if(CanGo)
				return ;//zzzzzzzzzzzzzz
			
			pCore->AddPinRequest(this, pin);
			pCore->RunPinRequest(this);

			if(timeSpace){
				//Sleep(timeSpace);
				xtime_get(&xt, boost::TIME_UTC);
				xt.nsec += (long)timeSpace*1000000;
				boost::thread::sleep(xt);
			}
			
			tagMSG message;
			
			if (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			};
			

		};
	}
	CanGo =true;
	
}
/*
UINT proc(LPVOID Param){
	Quester *pQ = (Quester *)Param;
	pQ->trueNext();
	return 0;
};
*/
class QuesterThread{
public:
	QuesterThread(Quester *pQ){
		p = pQ;
	};
	Quester *p;
	void operator()(){
		p->trueNext();
	}
	
};

Counter Quester::Next(){
	
//	th= AfxBeginThread(proc, this);
	
	QuesterThread *qt = new QuesterThread (this);

	boost::thread::thread(*qt);
	
	do{
		Sleep(10);
		tagMSG message;
		
		while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			};
		
	}while(!CanGo);
	
	//trueNext();
	CanGo=false;
	return Counter();//zzzzzzzzzzzzzz
};

void Quester::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<< Quester::timeSpace;
		ar<< Quester::sumCount;
	}else{
		ar>> Quester::timeSpace;
		ar>> Quester::sumCount;
	}
};

