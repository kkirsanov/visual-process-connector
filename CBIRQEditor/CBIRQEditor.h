// CBIRQEditor.h : main header file for the CBIRQEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../core/core/core.h"


class CCBIRQEditorApp : public CWinApp
{
public:
	CCBIRQEditorApp();
	~CCBIRQEditorApp(){
		if (pCore){
			delete pCore;
		}
	}
	Core* pCore;
public:
	virtual BOOL InitInstance();
	//PB;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHelpPlugins();
public:
	afx_msg void OnHelpRuninterface();
};

extern CCBIRQEditorApp theApp;