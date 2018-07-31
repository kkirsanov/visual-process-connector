// Input_Audio.h : main header file for the Input_Audio DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CInput_AudioApp
// See Input_Audio.cpp for the implementation of this class
//

class CInput_AudioApp : public CWinApp
{
public:
	CInput_AudioApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

class CInput_imageApp : public CWinApp
{
public:
	CInput_imageApp();

	// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class AudioIn: public Plugin {
	friend class SimpleOutMeasurePin<AudioIn>;
private:
	PMeasure value;
public:
	AudioIn(Core *pCore);
	~AudioIn();
	UINT DoModal();

	Counter Next();
	virtual	void Serialize(CArchive& ar){

		Plugin::Serialize(ar);

		if (ar.IsStoring()){				
		}else{
		}
	};


};