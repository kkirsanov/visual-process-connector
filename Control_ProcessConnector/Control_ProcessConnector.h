// Control_ProcessConnector.h : main header file for the Control_ProcessConnector DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


class CControl_ProcessConnectorApp : public CWinApp
{
public:
	CControl_ProcessConnectorApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

class Control : public  Plugin{
friend class SimpleOutMeasurePin<Control>;
private:
	CString path;
public:	
	PMeasure value;
	Control(Core *pCore);
	~Control(){
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );		
		delete outPins.at(0);
		delete inPins.at(0);
	};
	void Serialize(CArchive& ar);
	UINT DoModal();
	Counter Next();
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
};