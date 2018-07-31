// Filter_network.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Filter_network.h"
#include "NetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFilter_networkApp

BEGIN_MESSAGE_MAP(CFilter_networkApp, CWinApp)
END_MESSAGE_MAP()


// CFilter_networkApp construction

CFilter_networkApp::CFilter_networkApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFilter_networkApp object

CFilter_networkApp theApp;


// CFilter_networkApp initialization

BOOL CFilter_networkApp::InitInstance()
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
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	NetFilter *pIG = new NetFilter(pCore);
    return pIG;

}



NetFilter::NetFilter(Core *pCore): Plugin(pCore){	

	isStopped = false;
	remotePort = 3001;
	localPort  = 3001;
	pThread = NULL;
	remoteIP=0;

	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);
	inPins.push_back(new MeasureInputPin(this));

	server=NULL;

	pluginType = CORE_PLG_FILTER;
	name = "Network";
	description = "Network";
}
NetFilter::~NetFilter(){
	if (server!=NULL)
		delete server;
	//if (!pThread)
		//pThread->
		
};
UINT NetFilter::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CNetDlg* dlg = new CNetDlg( &remoteIP, &remotePort, &localPort, NULL);
	if(dlg->DoModal()==IDOK){
		
	};
	delete dlg;
	return MB_OK;
};
UINT proc(LPVOID Param)
{
	NetFilter *p = (NetFilter*)Param;
	if (p->isStopped)
		return 0;
	if (p->server){
		p->IsWorking = true;

		p->isSended=false;
		p->server->Listen();
		p->server->Start(p->image, p->counter);
		if (p->isStopped)
			return 0;
		delete p->server;
		p->server = NULL;
		p->isSended=true;
		p->IsWorking=false;
		p->Next();
	}
	return 0;
};
Counter NetFilter::Next(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	/*
	проверить подключение
	Если подключены по входному пину, то забрать данные из него и отдать в сеть

	Если не подключены то произвести подключение по сети, забрать данные из неё и проверить
	тип данных.
	Если тип изображение, то создать такой пин и вернуть в него данныевернуть в выходной
	*/
	counter = Counter();

	if (inPins[0]->GetLink()){
		StartTiming();
		//есть подключение - Забрать изображение из пина
		if (server==NULL){
			

			isSended = false;
			server = new Server(remoteIP, localPort);
			ImageInputPin * pin = (ImageInputPin *) (inPins[0]);
			

			Counter count1;
			pCore->AddPinRequest(this, pin);
			pCore->RunPinRequest(this);
			counter = pin->counter;

			image = boost::shared_dynamic_cast<Image>(pin->Get());;
			
			//отправить в сеть
			if (!isStopped){
				pThread = AfxBeginThread(proc, this);
				EndTiming();
				while (!isSended){
					Sleep(10);
					tagMSG message;
					while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
							::TranslateMessage(&message);
							::DispatchMessage(&message);
						};
				}
			};
		}else{
			
			//Сервер есть, нужно просто обновить изображение
			if(server->working){
				StartTiming();
				ImageInputPin * pin = (ImageInputPin *) (inPins[0]);
				
				pCore->AddPinRequest(this, pin);
				pCore->RunPinRequest(this);
				counter = pin->counter;
				
				image = boost::shared_dynamic_cast<Image>(pin->Get());;

			}
		}
	}else{
		StartTiming();
		//нет подключения - забрать данные из сети
		Server myServer(remoteIP, remotePort);
		int type;
		void *data = myServer.Recive(type, counter);
		if (type!=0){
			if (type==1){//image				
				image = PImage((Image*)data);
			};

			//проверить количесво созданных выходных пинов
			if (outPins.size()==0){
				//если 0 - создать новый
				outPins.push_back(new SimpleOutImagePin<NetFilter>(this));
			}else{
				if (outPins.at(0)->GetDataType()!= CORE_DATA_IMAGE){
					//delete Pin
					outPins.clear();
					//create new
					outPins.push_back(new SimpleOutImagePin<NetFilter>(this));
	
				}
			}
		}
		
	}

	EndTiming();
	tagMSG message;
	while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		};
	return counter;
};

void NetFilter::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<<remoteIP;
		ar<<remotePort;
		ar<<localPort;
	}else{
		ar>>remoteIP;
		ar>>remotePort;
		ar>>localPort;
	}
};