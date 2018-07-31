// Input_us.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Input_us.h"
#include <boost/lexical_cast.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CInput_usApp

BEGIN_MESSAGE_MAP(CInput_usApp, CWinApp)
END_MESSAGE_MAP()


// CInput_usApp construction

CInput_usApp::CInput_usApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CInput_usApp object

CInput_usApp theApp;


// CInput_usApp initialization

BOOL CInput_usApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}


extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isInputPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	

	US *pUS = new US(pCore);
	return pUS;
}


US::US(Core *pCore): Plugin(pCore){

	value = PMeasure(new Measure());
	pluginType = CORE_PLUGIN_TYPE::CORE_PLG_INPUT;

	SimpleOutMeasurePin<US> *pPin = new SimpleOutMeasurePin<US>(this);
	outPins.push_back(pPin);

	inPins.push_back(new MeasureInputPin(this));


	name = "US Input";
	description = "US Input";
	connected = false;
}

US::~US(){
	delete outPins.at(0);
};
UINT US::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxSocketInit();
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
Counter US::Next(){
	StartTiming();
	//char *getrange = "SRF GETRANGE\r";
	char *getrange = "SRF GETANGRNG\r";

	if (!connected){
		//192.168.252.13
		//127.000.000.001
		CSockAddr addr("192.168.252.13", 23);
		//CSockAddr addr("192.168.253.003", 23);
		sockClient.Create();
		connected=true;
		try{
			sockClient.Connect(addr);
			
			string str;
			int i=0;
			while(i<100){
				char ch;
				sockClient.Receive(&ch,1,2);
				if(ch!='\r'){
					str.push_back(ch);
				}else{
					break;
				}
				i++;
			};

			if (strcmp(str.c_str(), "SRF READY")==0)
				connected = true;
		}catch(CBlockingSocketException *ex){
			sockClient.Cleanup();
			char err[50];
			//ex->GetErrorMessage(err,1);
			//AfxMessageBox(err);
			connected = false;
			value = PMeasure(new Measure());
			value->values.push_back(1);
			value->values.push_back(0);
		}

	}

	if (connected){

		value = PMeasure(new Measure());

		char *data = new char[50];
		try{
			int res = sockClient.Send(getrange, strlen(getrange), 10);
			Sleep(10);
			sockClient.Receive(data,20,10);
		}catch(CBlockingSocketException *ex){
			connected=false;
			value->values.push_back(1);
			sockClient.Cleanup();
			EndTiming();
			delete[] data;
			return Counter();
		};
		
		string str;
		int i=0;
		while(i<30){
			if(data[i]!='\r'){
				str.push_back(data[i]);
			}else{
				break;
			}
			i++;					
		}
		//RNG:ANG 038:0023
		if (str.length()>6){
			string sval1 = str.substr(8,3);
			string sval2 = str.substr(12,4);
			try{
				int val1 = boost::lexical_cast<int>(sval1);
				int val2 = boost::lexical_cast<int>(sval2);
				value->values.push_back(val1);
				value->values.push_back(val2);				
			}catch(...){

			}
		}

		/*
		if (str.length()>6){
			string newS = str.substr(6,7);
			value->value = atoi(newS.c_str());
			if (value->value >250)
				value->value =250;

			if (value->value <0)
				value->value =1;
		}
		*/
		delete[] data;
	}
	EndTiming();
	return Counter();
};