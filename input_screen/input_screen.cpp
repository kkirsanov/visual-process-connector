// input_screen.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "input_screen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(Cinput_screenApp, CWinApp)
END_MESSAGE_MAP()


// Cinput_screenApp construction

Cinput_screenApp::Cinput_screenApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cinput_screenApp object

Cinput_screenApp theApp;


// Cinput_screenApp initialization

BOOL Cinput_screenApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isInputPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	ScreenReader *pSR = new ScreenReader(pCore);
	return pSR;
}

ScreenReader::ScreenReader(Core *pCore): Plugin(pCore){	
	image = PImage(new Image());
	outPins.push_back(new SimpleOutImagePin<ScreenReader>(this));

	pluginType = CORE_PLUGIN_TYPE::CORE_PLG_INPUT;

	name = "Screen Reader";
	description = "Screen Reader";
};

ScreenReader::~ScreenReader(){	
	delete outPins.at(0);
	outPins.clear();
};


void ScreenReader::Serialize(CArchive &ar){

};
UINT ScreenReader::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
};


long ScreenReader::GetCount(){
	return 1;
};

Counter ScreenReader::Next(){	

	StartTiming();
	Counter c;
	c.count=1;
	int _x = GetSystemMetrics(SM_CXSCREEN);
	int _y = GetSystemMetrics(SM_CYSCREEN);

	HDC hDDC = GetDC(0);
	HDC hCDC = CreateCompatibleDC(hDDC);
	HBITMAP hBmp = CreateCompatibleBitmap(hDDC,_x,_y);
	SelectObject(hCDC, hBmp);
	BitBlt(hCDC,0,0,_x,_y,hDDC,0,0,SRCCOPY|CAPTUREBLT);

	ReleaseDC(0, hDDC);
	DeleteDC(hCDC);

	Bitmap  = PBitmap(new CxImage());
	Bitmap->CreateFromHBITMAP(hBmp);
	image->pBitmap= Bitmap;

	DeleteObject(hBmp);
	EndTiming();

	return c;
};