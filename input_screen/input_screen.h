// input_screen.h : main header file for the input_screen DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cinput_screenApp
// See input_screen.cpp for the implementation of this class
//

#define PLUGIN_API __declspec(dllexport)


class Cinput_screenApp : public CWinApp
{
public:
	Cinput_screenApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class ScreenReader: public Plugin {
	friend class SimpleOutImagePin<ScreenReader>;

private:
	PBitmap Bitmap;
	PImage image;
public:
	ScreenReader(Core *pCore);
	~ScreenReader();
	UINT DoModal();
	long GetCount();
	Counter Next();

	virtual void Serialize(CArchive& ar);

};