// Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Console.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../core/core/core.h"

// The one and only application object

CWinApp theApp;
using namespace std;
Core *pCore;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		//create core
		pCore = new Core();
	}

	return nRetCode;
}
