// filter_summator.h : main header file for the filter_summator DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define PLUGIN_API __declspec(dllexport)


// Cfilter_summatorApp
// See filter_summator.cpp for the implementation of this class
//

class Cfilter_summatorApp : public CWinApp
{
public:
	Cfilter_summatorApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


using namespace CBIR;


class Summator: public virtual FilterPlugin {
	friend class SImagePin;
	private:
		CxImage *Bitmap;
		Image* image;
		CString fileName;
	public:
		Summator(Core *pCore);
		~Summator();
		int sumCount;
		int timeSpace; //milliseconds
		UINT DoModal();
		long GetCount(){return 1;};
		Counter Next();
		void Serialize(CArchive& ar);

		CString	GetName(){return CString("Summator");};
		CString	GetDescription(){return CString("Summator");};
};

class SImagePin: public CBIR::ImageOutPin{
	public:
		SImagePin(Plugin* plug);
		~SImagePin(){};
		CString GetName(){return CString("Image");};
		Counter QueryNew(){return m_plug->Next();};
		Image* Get(bool copy = false);
	};

class ImageInputPin: public CBIR::ImageInPin{
public:
	ImageInputPin(Plugin* plug):ImageInPin(plug){
		img=NULL;
	};
	~ImageInputPin(){};
	Image *img;
	void Querry();
};
