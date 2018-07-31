// filter_quester.h : main header file for the filter_quester DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define PLUGIN_API __declspec(dllexport)
// Cfilter_questerApp
// See filter_quester.cpp for the implementation of this class
//

class Cfilter_questerApp : public CWinApp
{
public:
	Cfilter_questerApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

class Quester: public virtual Plugin{
	public:
		Quester(Core *pCore);
		~Quester();
		CWinThread *th;
		bool CanGo;
		int sumCount;
		int timeSpace; //milliseconds
		UINT DoModal();
		long GetCount(){return 1;};
		Counter Next();
		void trueNext();
		void Serialize(CArchive& ar);
	
};
