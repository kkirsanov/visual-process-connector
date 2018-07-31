#pragma once


// CPropertiesView view
#include "CBIRQEditorDoc.h"
#include "Propertiesdialog.h"

class CPropertiesView : public CView
{
	DECLARE_DYNCREATE(CPropertiesView)

protected:
	CPropertiesView();           // protected constructor used by dynamic creation
	virtual ~CPropertiesView();
	CCBIRQEditorDoc* m_pDocument;
	int OnCreate(LPCREATESTRUCT lpcs);
	void OnDestroy();
	void OnPaint(){
	};
	
	void OnClose(){		
		d->CloseWindow();
		delete d;
		
		CView::OnClose();
	};

public:
	Propertiesdialog *d;
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


