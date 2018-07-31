#pragma once

#include "../core/core/core.h"

#include "afxcmn.h"
// CAddElementD dialog

class CAddElementD : public CDialog
{
	DECLARE_DYNAMIC(CAddElementD)

public:
	CAddElementD(Core* core, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddElementD();
	PluginEntry* GetPlgE(){return m_plge;};
	
// Dialog Data
	enum { IDD = IDD_ADDELEMENT };
private:
	PluginEntry * m_plge;
	Core* core;
	bool isActive;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl SelectPluginsCtrl;

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	afx_msg void OnBnClickedOk();
};
