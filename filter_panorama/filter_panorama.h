// filter_panorama.h : main header file for the filter_panorama DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#define PLUGIN_API __declspec(dllexport)

// Cfilter_panoramaApp
// See filter_panorama.cpp for the implementation of this class
//

class Cfilter_panoramaApp : public CWinApp
{
public:
	Cfilter_panoramaApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

class Panorama: public virtual FilterPlugin {
	friend class SimpleOutImagePin<Resizer>;
	private:
		CxImage *Bitmap;
		Image* image;
		CString fileName;
	public:
		vector<Image *> images;
		Panorama(Core *pCore);
		~Panorama();
		int sumCount;
		int timeSpace; //milliseconds
		UINT DoModal();
		long GetCount(){return 1;};
		Counter Next();
		void Serialize(CArchive& ar);

		CString	GetName(){return CString("Panorama");};
		CString	GetDescription(){return CString("Panorama");};
};