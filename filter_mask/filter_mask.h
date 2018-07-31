// filter_mask.h : main header file for the filter_mask DLL
//

#pragma once

#include <math.h>
#include "mask.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define PLUGIN_API __declspec(dllexport)
// Cfilter_maskApp
// See filter_mask.cpp for the implementation of this class
//



class Cfilter_maskApp : public CWinApp
{
public:
	Cfilter_maskApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};



class MaskFilter: public Plugin {
friend class SimpleOutImagePin<MaskFilter>;
private:
	PBitmap Bitmap;
	PImage image;

	CString fileName;
	CString ColorModel;
	MaskPalette palette;
public:
	void ApplayMask(CxImage *pImg, Mask &window);
	MaskFilter(Core *pCore);
	~MaskFilter();
	Mask window1, window2, window3;

	UINT DoModal();
	long GetCount(){return 1;};
	Counter Next();
	void Serialize(CArchive& ar);

};