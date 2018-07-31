// Input_image.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Input_image.h"
#include "ImageDialog.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool fileExists(const CString& fileName) 
{ 
	std::fstream fin; fin.open(fileName,std::ios::in); if( fin.is_open() ) { fin.close(); return true; } fin.close(); return false; 
} 


BEGIN_MESSAGE_MAP(CInput_imageApp, CWinApp)
END_MESSAGE_MAP()


// CInput_imageApp construction

CInput_imageApp::CInput_imageApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CInput_imageApp object

CInput_imageApp theApp;


// CInput_imageApp initialization

BOOL CInput_imageApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isInputPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	 
	ImageGetter *pIG = new ImageGetter(pCore);
    return pIG;
}


ImageGetter::ImageGetter(Core *pCore): Plugin(pCore){	

	image = PImage(new Image());
	pluginType = CORE_PLUGIN_TYPE::CORE_PLG_INPUT;

	SimpleOutImagePin<ImageGetter> *pPin = new SimpleOutImagePin<ImageGetter>(this);
	outPins.push_back(pPin);
	name = "Image Input";
	description = "Image Input";
}

ImageGetter::~ImageGetter(){
	delete outPins.at(0);
};
UINT ImageGetter::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CImageDialog* dlg = new CImageDialog(image, pCore);
	INT_PTR z =dlg->DoModal();
	if(z==1 && (dlg->FileName!="")){
		CString st = dlg->FileName.GetBuffer();
		fileName = string(st.GetBuffer());
		
		image->pBitmap = PBitmap(new CxImage());
		image->pBitmap->Load(fileName.c_str());

		image->path=fileName;
	}
	delete dlg;
	return IDOK;
};
Counter ImageGetter::Next(){
	StartTiming();	
	if (image->pBitmap){
		Counter c;
		c.count=1;
		c.isInfinity=true;
		EndTiming();
		return c;
	}
	EndTiming();
	return Counter();
};
