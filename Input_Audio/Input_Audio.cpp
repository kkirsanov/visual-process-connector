// Input_Audio.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Input_Audio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CInput_AudioApp, CWinApp)
END_MESSAGE_MAP()


// CInput_AudioApp construction

CInput_AudioApp::CInput_AudioApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CInput_AudioApp object

CInput_AudioApp theApp;


// CInput_AudioApp initialization

BOOL CInput_AudioApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isInputPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	

	AudioIn *pIG = new AudioIn(pCore);
	return pIG;
}


AudioIn::AudioIn(Core *pCore): Plugin(pCore){
	value = PMeasure(new Measure());
	pluginType = CORE_PLUGIN_TYPE::CORE_PLG_INPUT;

	SimpleOutMeasurePin<AudioIn> *pPin = new SimpleOutMeasurePin<AudioIn>(this);
	outPins.push_back(pPin);
	name = "Audio Input";
	description = "Audio Input";
}

AudioIn::~AudioIn(){
	delete outPins.at(0);
};
UINT AudioIn::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
/*
	CImageDialog* dlg = new CImageDialog(image, pCore);
	INT_PTR z =dlg->DoModal();
	if(z==1 && (dlg->FileName!="")){
		CString st = dlg->FileName.GetBuffer();
		fileName = string(st.GetBuffer());

		image->pBitmap = new CxImage();//pCore->getImageFromFile(false, fileName);
		image->pBitmap->Load(fileName.c_str());

		image->path=fileName;
	}
	delete dlg;
	*/
	return IDOK;
};
Counter AudioIn::Next(){
	StartTiming();	
	value = PMeasure(new Measure());
	value->values.push_back(100);

	EndTiming();
	return Counter();
};
