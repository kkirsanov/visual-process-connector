// PropertiesView.cpp : implementation file
//

#include "stdafx.h"
#include "CBIRQEditor.h"
#include "PropertiesView.h"



// CPropertiesView

IMPLEMENT_DYNCREATE(CPropertiesView, CView)

CPropertiesView::CPropertiesView()
{

}

void CPropertiesView::OnDestroy(){	
	delete d;
	CView::OnDestroy();
}
int CPropertiesView::OnCreate(LPCREATESTRUCT lpcs){
	

	int z = CView::OnCreate(lpcs);	
	
	d = new Propertiesdialog((CCBIRQEditorDoc*)GetDocument(), this);
	d->Create(IDD_PROPD, this);
	d->SetParent(this);
	//d->SetOwner(this);
	d->ShowWindow(SW_SHOW);
	
	//RECT rect;
	//d->GetWindowRect(&rect);
	//GetParentFrame()->SetWindowPos(&CWnd::wndTop, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_NOMOVE);
	return z;
};

CPropertiesView::~CPropertiesView(){

}

BEGIN_MESSAGE_MAP(CPropertiesView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPropertiesView drawing

void CPropertiesView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CPropertiesView diagnostics

#ifdef _DEBUG
void CPropertiesView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropertiesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPropertiesView message handlers
