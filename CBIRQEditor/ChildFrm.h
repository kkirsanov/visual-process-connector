#pragma once

#include "CBIRQEditorView.h"
#include "PropertiesView.h"
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();
	CSplitterWnd splitter;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext *pContext);
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
