
#include "stdafx.h"
#include "CBIRQEditor.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}
BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext *pContext){
	BOOL z = splitter.CreateStatic(this, 1,2);
	
	splitter.CreateView(0,0,RUNTIME_CLASS(CPropertiesView),CSize(200,100),pContext);
	splitter.CreateView(0,1,RUNTIME_CLASS(CCBIRQEditorView),CSize(100,100),pContext);
	
	this->ShowWindow(SW_MAXIMIZE);

	return z;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers
