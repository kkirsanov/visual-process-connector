// filter_comlex_mi.h : main header file for the filter_comlex_mi DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include <fstream>;


// Cfilter_comlex_miApp
// See filter_comlex_mi.cpp for the implementation of this class
//

class Cfilter_comlex_miApp : public CWinApp
{
public:
	Cfilter_comlex_miApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};



class ComplexFilter: public Plugin {
	friend class SimpleOutImagePin<ComplexFilter>;
private:
	PBitmap Bitmap;
	PImage image;
	PMeasure value;
	//ofstream logger;
public:
	virtual	void Serialize(CArchive& ar){
			Plugin::Serialize(ar);
	}
	CDialog* dlg;
	ComplexFilter(Core *pCore);
	virtual ~ComplexFilter();
	UINT DoModal();
	Counter Next();
};
