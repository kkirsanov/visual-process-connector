// filter_stereo.h : main header file for the filter_stereo DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cfilter_stereoApp
// See filter_stereo.cpp for the implementation of this class
//

class Cfilter_stereoApp : public CWinApp
{
public:
	Cfilter_stereoApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class ColorFilter: public virtual Plugin {
	friend class SimpleOutImagePin<ColorFilter>;
private:

	Image* image1;
	Image* image2;
	CxImage tempBMP1;
	CxImage tempBMP2;
public:
	ColorFilter(Core *pCore);
	~ColorFilter();
	Counter counter;	
	UINT DoModal();
	Counter Next();
	void Serialize(CArchive& ar);
};