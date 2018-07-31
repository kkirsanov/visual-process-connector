// Input_us.h : main header file for the Input_us DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Blocksock.h"


// CInput_usApp
// See Input_us.cpp for the implementation of this class
//

class CInput_usApp : public CWinApp
{
public:
	CInput_usApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};



class US: public Plugin {
	friend class SimpleOutMeasurePin<US>;
private:
	PMeasure value;
public:
	US(Core *pCore);
	~US();
	UINT DoModal();

	Counter Next();
	CBlockingSocket sockClient;
	bool connected;

	virtual	void Serialize(CArchive& ar){

		Plugin::Serialize(ar);

		if (ar.IsStoring()){				
		}else{
		}
	};


};