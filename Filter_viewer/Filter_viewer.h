// Filter_viewer.h : main header file for the Filter_viewer DLL
//

//#include "ImageVD.h"


#pragma once

#ifndef H_IMV
#define H_IMV


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define PLUGIN_API __declspec(dllexport)
// CFilter_viewerApp
// See Filter_viewer.cpp for the implementation of this class
//

class CFilter_viewerApp : public CWinApp
{
public:
	CFilter_viewerApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class ImageViewer: public virtual Plugin {
	friend class SimpleOutImagePin<ImageViewer>;
	private:
		PImage Bitmap;
		PImage image;
		CString fileName;
		bool IsDialog;
	public:
		virtual
		void Serialize(CArchive& ar){
			Plugin::Serialize(ar);
		}
		CDialog* dlg;
		ImageViewer(Core *pCore);
		virtual ~ImageViewer();
		UINT DoModal();

		Counter Next();
};

#endif