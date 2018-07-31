// filter_resize.h : main header file for the filter_resize DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cfilter_resizeApp
// See filter_resize.cpp for the implementation of this class
//

class Cfilter_resizeApp : public CWinApp
{
public:
	Cfilter_resizeApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class Resizer: public virtual Plugin {
friend class SimpleOutImagePin<Resizer>;
private:

	PImage image;
	PBitmap tempBMP;
public:
	int sizeX, sizeY, qual;
	Resizer(Core *pCore);
	~Resizer();
	Counter counter;	
	UINT DoModal();
	Counter Next();
	void Serialize(CArchive& ar);
};