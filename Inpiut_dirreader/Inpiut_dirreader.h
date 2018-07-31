// Inpiut_dirreader.h : main header file for the Inpiut_dirreader DLL
//

#pragma once

using namespace std;

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define PLUGIN_API __declspec(dllexport)

// CInpiut_dirreaderApp
// See Inpiut_dirreader.cpp for the implementation of this class
//

class CInpiut_dirreaderApp : public CWinApp
{
public:
	CInpiut_dirreaderApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class DirReader: public Plugin {
	friend class SimpleOutImagePin<DirReader>;

	private:
		int currentNum, totalNum;
		PBitmap Bitmap;
		PImage image;
		CArray<CString> fileNames;
		int counter;
		void ReadFiles(LPCTSTR pstr);
		CString workPath;
	public:
		DirReader(Core *pCore);
		~DirReader();
		UINT DoModal();
		long GetCount();
		Counter Next();

		virtual void Serialize(CArchive& ar);

};