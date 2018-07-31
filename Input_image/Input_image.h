// Input_image.h : main header file for the Input_image DLL
//

#pragma once



#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CInput_imageApp
// See Input_image.cpp for the implementation of this class

#define PLUGIN_API __declspec(dllexport)

class CInput_imageApp : public CWinApp
{
public:
	CInput_imageApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class ImageGetter: public virtual Plugin {
friend class SimpleOutImagePin<ImageGetter>;
private:
	PImage image;
	string fileName;
public:
	ImageGetter(Core *pCore);
	~ImageGetter();
	UINT DoModal();

	Counter Next();
	virtual
	void Serialize(CArchive& ar){
		
		Plugin::Serialize(ar);

		if (ar.IsStoring()){
			CString st(fileName.c_str());
			ar<<st;
		}else{
			CString st;
			ar>>st;
			fileName = CString(st.GetBuffer());
			bool isOk=true;
			CFile f;
			int reult =  f.Open(st, CFile::modeRead | CFile::shareDenyNone);
			if(reult==0){			
				isOk==false;
			}
			if ((st== CString("")) && isOk)
			{
				int res = AfxMessageBox("Файл не обнаружен. Выбрать?", MB_YESNO);
				if(IDYES == res){//Ok			
					this->DoModal();
				}
			}else{
				image->pBitmap = boost::shared_ptr<CxImage>(new CxImage());
				image->pBitmap->Load(fileName.c_str());
				image->path=fileName;
			}
		}
	};
};