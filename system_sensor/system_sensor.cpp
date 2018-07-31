// system_sensor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "system_sensor.h"
#include <boost/foreach.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Csystem_sensorApp, CWinApp)
END_MESSAGE_MAP()


// Csystem_sensorApp construction

Csystem_sensorApp::Csystem_sensorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Csystem_sensorApp object

Csystem_sensorApp theApp;


// Csystem_sensorApp initialization

BOOL Csystem_sensorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isInputPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	

	SystemSensor *pSystemSensor = new SystemSensor(pCore);
	return pSystemSensor;
}



SystemSensor::SystemSensor(Core *pCore): Plugin(pCore){

	value = PMeasure(new Measure());
	pluginType = CORE_PLUGIN_TYPE::CORE_PLG_INPUT;

	outPins.push_back(new SimpleOutMeasurePin<SystemSensor>(this));
	
	inPins.push_back(new MeasureInputPin(this));
	inPins.push_back(new MeasureInputPin(this));
	

	name = "_Sensor";
	description = "_Sensor";
	connected = false;
}

SystemSensor::~SystemSensor(){
	BOOST_FOREACH(OutPin *pin, outPins){
		delete pin;
	}
	BOOST_FOREACH(InPin *pin, inPins){
		delete pin;
	}
	
};
UINT SystemSensor::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return IDOK;
};
Counter SystemSensor::Next(){
	StartTiming();
	value = PMeasure(new Measure());

	EndTiming();
	return Counter();
};