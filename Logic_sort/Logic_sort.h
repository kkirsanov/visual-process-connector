// Logic_sort.h : main header file for the Logic_sort DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#define PLUGIN_API __declspec(dllexport)

using namespace CBIR;
// CLogic_sortApp
// See Logic_sort.cpp for the implementation of this class
//

class CLogic_sortApp : public CWinApp
{
public:
	CLogic_sortApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

using namespace CBIR;


class SimpleSort : public virtual LogicPlugin{		
	friend class SLIPin;
private:
	vector<Logic> logics;
public:			
	SimpleSort(Core *pCore);
	~SimpleSort(){};		
	CString	GetName(){return CString("Simple sort");};
	CString	GetDescription(){return CString("Simple sort");};
	UINT DoModal();
	Counter Next();
};

class SLIPin: public CBIR::LogicInPin{
public:
	SLIPin(Plugin* plug):LogicInPin(plug){};
	~SLIPin(){};
	void Querry();
	Counter QueryLinkedPinNext();
	Logic* pLogic;

};

class SLOPin: public CBIR::LogicOutPin{
public:
	SLOPin(Plugin* plug):LogicOutPin(plug){};
	Counter QueryNew(){return m_plug->Next();};
	~SLOPin(){};
	Logic* Get(bool copy =false);
private:
	SimpleSort* m_plug;
};
