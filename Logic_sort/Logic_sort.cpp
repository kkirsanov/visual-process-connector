// Logic_sort.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Logic_sort.h"
#include <algorithm>
#include "SortViewDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(CLogic_sortApp, CWinApp)
END_MESSAGE_MAP()


// CLogic_sortApp construction

CLogic_sortApp::CLogic_sortApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CLogic_sortApp object

CLogic_sortApp theApp;


// CLogic_sortApp initialization

BOOL CLogic_sortApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isMetricPlugin	=	TRUE;

extern "C" PLUGIN_API CBIR::Plugin* init(CBIR::Core *pCore)
{
	SimpleSort *pSort = new SimpleSort(pCore);
    return pSort;
}


Counter SimpleSort::Next(){
	SLIPin *pin = (SLIPin *)inPins[0];
	if (pin->Linked()){
		logics.clear();
		while(pin->QueryLinkedPinNext().count>=1){
			pin->Querry();
			logics.push_back(*(pin->pLogic));
		};
		StartTiming();
		sort(logics.begin(), logics.end());
		EndTiming();
		Counter c;
		return c;
	}
	return Counter();
}
SimpleSort::SimpleSort(Core *pCore): LogicPlugin(pCore){	
	inPins.push_back(new SLIPin(this));
	outPins.push_back(new SLOPin(this));
};

UINT SimpleSort::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CSortViewDialog Dlg(pCore, &logics);
	Dlg.DoModal();
	return 0;
}
Logic * SLOPin::Get(bool copy){
	return NULL;
};
void SLIPin::Querry(){
	if(m_linkedPin==NULL)
		return;
	LogicOutPin *pin = (LogicOutPin *)m_linkedPin;
	pLogic = pin->Get(true);
	return;
};

Counter SLIPin::QueryLinkedPinNext(){
	if(m_linkedPin==NULL)
		return Counter();
	LogicOutPin *pin = (LogicOutPin *)m_linkedPin;
	return pin->QueryNew();
};