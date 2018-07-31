// system_sensor.h : main header file for the system_sensor DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Csystem_sensorApp
// See system_sensor.cpp for the implementation of this class
//

class Csystem_sensorApp : public CWinApp
{
public:
	Csystem_sensorApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#define PLUGIN_API __declspec(dllexport)


class SystemSensor: public Plugin {
	friend class SimpleOutMeasurePin<SystemSensor>;
private:
	PMeasure value;
public:
	SystemSensor(Core *pCore);
	~SystemSensor();
	UINT DoModal();

	Counter Next();
	bool connected;

	virtual	void Serialize(CArchive& ar){
		Plugin::Serialize(ar);

		if (ar.IsStoring()){				
		}else{
		}
	};

};
