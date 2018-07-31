// Filter_network.h : main header file for the Filter_network DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "NetWork.h"

// CFilter_networkApp
// See Filter_network.cpp for the implementation of this class
//
#define PLUGIN_API __declspec(dllexport)


class CFilter_networkApp : public CWinApp
{
public:
	CFilter_networkApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class NetFilter: public Plugin {
	private:
		friend class SimpleOutImagePin<NetFilter>;
		friend class SimpleOutMeasurePin<NetFilter>;
	
		PBitmap Bitmap;
		PBitmap tempBMP;
		
		DWORD remoteIP;

		//AfxBeginThread(
	public:
		bool isStopped;
		bool isSended;
		CWinThread *pThread;
		Counter counter;
		PImage image;

		UINT remotePort;
		UINT localPort;
		Server *server;

		NetFilter(Core *pCore);
		~NetFilter();
		UINT DoModal();
		long GetCount(){return 1;};
		Counter Next();
		void Serialize(CArchive& ar);

};