// CBIRQEditorDoc.h : interface of the CCBIRQEditorDoc class
//
#include "cbirqelements.h"
#include "../core/core/core.h"



#pragma once

class CCBIRQEditorDoc : public CDocument
{
public: // create from serialization only
	CCBIRQEditorDoc();
	DECLARE_DYNCREATE(CCBIRQEditorDoc)

	Core *core;

public:
	void RunScript();
	CCBIRElements *CBIRElements;

	CString script;
	int z;
	virtual BOOL OnNewDocument();
	int a;

	virtual void Serialize(CArchive& ar);

public:
	virtual ~CCBIRQEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


