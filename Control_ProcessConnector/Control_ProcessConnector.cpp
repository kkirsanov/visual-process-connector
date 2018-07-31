// Control_ProcessConnector.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Control_ProcessConnector.h"
#include "PathSelector.h"

#include <stdio.h>
#include <conio.h>
#include <process.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CControl_ProcessConnectorApp

BEGIN_MESSAGE_MAP(CControl_ProcessConnectorApp, CWinApp)
END_MESSAGE_MAP()


// CControl_ProcessConnectorApp construction

CControl_ProcessConnectorApp::CControl_ProcessConnectorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CControl_ProcessConnectorApp object

CControl_ProcessConnectorApp theApp;


// CControl_ProcessConnectorApp initialization

BOOL CControl_ProcessConnectorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	Control *pIG = new Control(pCore);
    return pIG;
}


void Control::Serialize(CArchive &ar){
	if (ar.IsStoring())
	{
		ar<<path;
	}else{
		ar>>path;
	};
}
Counter Control::Next(){
	//_execl( path.GetBuffer(), path.GetBuffer(), "_execl", "two", NULL );
    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line). 
        path.GetBuffer(), // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return Counter();
    }

    // Wait until child process exits.
    //WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    //CloseHandle( pi.hProcess );
    //CloseHandle( pi.hThread );


	return Counter();
}
Control::Control(Core *pCore): Plugin(pCore){	
	
	outPins.push_back(new SimpleOutMeasurePin<Control>(this));	
	inPins.push_back(new MeasureInputPin(this));
	value = PMeasure(new Measure());
	ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
	name = "Process Connector";
	description = "Process Connector";
	pluginType = CORE_PLG_FILTER;
};


UINT Control::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	PathSelector Dlg(&path);
	Dlg.DoModal();
	return 0;
};