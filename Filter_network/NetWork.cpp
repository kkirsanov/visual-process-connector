#include "StdAfx.h"
#include "NetWork.h"

Server::Server(DWORD ip, UINT port)
{
	Server::port=port;
	Server::ip=ip;
	WSADATA wsd;
	VERIFY(WSAStartup(0x0101, &wsd) == 0);
	TRACE("WSAStartup -- min version = %x\n", wsd.wVersion);
	working= FALSE;
}

void *Server::Recive(int &type, Counter &counter){
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	type=0;
	void* pResult=NULL;
	
	try{
		CSockAddr addr(ip, port);
		CBlockingSocket sockClient;
		sockClient.Create();
		sockClient.Connect(addr);
	
		//if conneced - recive Type
		int dataLength=0;
		int dataType=0;

		int recived=0;
		int *pInt = new int[10];
		
		int data[7];
		while(recived<sizeof(int)*7){
			int count = sockClient.Receive((char*)pInt, sizeof(int)*7 - recived, 200);
			memcpy(&(data[recived]), pInt,count);
			recived+= count;
		};
		type=data[0];
		int X=data[1];
		int Y=data[2];
		int size=data[3];
		counter.count=data[4];
		counter.isInfinity=data[5];
		counter.isUnknown=data[6];

		recived=0;
		if (X<1)
			return NULL;
		if (X>10000)
			return NULL;

		Image *image = new Image();
		BYTE *data2 = new BYTE[size];

		BYTE *pData = new BYTE[5086];
		while(recived<size){		
			int count= sockClient.Receive((char*)pData, 4000, 100);
			if (count==0) break;
			memcpy(&(data2[recived]), pData, count);			
			recived+=count;			
		}
		delete[] pData;
		image->pBitmap = boost::shared_ptr<CxImage>(new CxImage());
		image->pBitmap->Decode(data2, size,CXIMAGE_FORMAT_BMP);

		delete[] data2;

		return image;
	}catch(CBlockingSocketException *e){
		e->ReportError();
		e->Delete();
	}
	return NULL;
	
}
void Server::Listen(){
	CSockAddr addr(INADDR_ANY, port);
	try{	
		sockServer.Create();
		sockServer.Bind(addr);
		sockServer.Listen();
		CSockAddr addr(INADDR_ANY, port);
		working= TRUE;
		BOOL z =sockServer.Accept(sockRecv, addr); // Use another CSocket to accept the connection	
	}catch(CBlockingSocketException *e){
		e->ReportError();
		e->Delete();
		return;
	}
};
int Server::Start(PImage pImage,Counter counter, UINT port){
	if(port==0){
		port=Server::port;
	}
	BOOL z=FALSE;
	
	BYTE* buf = NULL;
	long size;
	bool z2 = pImage->pBitmap->Encode(buf, size, CXIMAGE_FORMAT_BMP);
	//AfxMessageBox(pImage->pBitmap->GetLastError());

	int data[7];
	data[0]=1;
	data[1]=pImage->pBitmap->GetWidth();
	data[2]=pImage->pBitmap->GetHeight();
	data[3]=size;
	data[4]=counter.count;
	data[5]=counter.isInfinity;
	data[6]=counter.isUnknown;
	
	try{
		sockRecv.Send((char*)data, sizeof(int)*7,310);
		sockRecv.Send((char*)buf, size,31);
		pImage->pBitmap->FreeMemory((void*)buf);

	}catch(CBlockingSocketException *e){
		working=FALSE;
		e->ReportError();
		e->Delete();
	}

	sockRecv.Close();
	sockServer.Close();
	working=FALSE;

	return 0;	
}
Server::~Server(void)
{
	sockServer.Close();
}
