// CBIRQEditorView.cpp : implementation of the CCBIRQEditorView class
//

#include "stdafx.h"
#include "CBIRQEditor.h"

#include "CBIRQEditorDoc.h"
#include "CBIRQEditorView.h"
#include "scriptdialog.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCBIRQEditorView

IMPLEMENT_DYNCREATE(CCBIRQEditorView, CView)

BEGIN_MESSAGE_MAP(CCBIRQEditorView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_RUNBTN, &CCBIRQEditorView::OnRunbtn)
	ON_COMMAND(ID_SCRIPTBTN, &CCBIRQEditorView::OnScriptbtn)
	ON_COMMAND(32777, &CCBIRQEditorView::OnAddBtn)
	
END_MESSAGE_MAP()

// CCBIRQEditorView construction/destruction

CCBIRQEditorView::CCBIRQEditorView()
{
	

}

CCBIRQEditorView::~CCBIRQEditorView()
{
}

BOOL CCBIRQEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCBIRQEditorView drawing

afx_msg void CCBIRQEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	vector<list<CCBIRQLink*>::iterator> dellist;//delete deque
	CCBIRQEditorDoc* pDoc = GetDocument();
	UINT id =pDoc->CBIRElements->GetActive();
	CString msg;

	switch(nChar){
		//case VK_:
		case VK_SUBTRACT:
		case 189://-			
			if(id>=1){
				msg = CString("Действительно сбросить связи модуля ");
				msg = msg+ pDoc->CBIRElements->GetByID(id)->GetName();
				msg += CString("?");

				if(IDYES == AfxMessageBox(msg, MB_YESNO)){
					//disconnect all pins  and delete all links for this element
					
					//scan for links
					for(list<CCBIRQLink*>::iterator it = pDoc->CBIRElements->links.begin(); it!=pDoc->CBIRElements->links.end(); it++){
						if ((*it)->end ==id){
							(*it)->endPin->UnLink();
							dellist.push_back(it);
							delete (*it);
						}
						if ((*it)->start ==id){
							(*it)->endPin->UnLink();
							dellist.push_back(it);
							delete (*it);
						}
					};
					//kill!!!
					for(vector<list<CCBIRQLink*>::iterator>::iterator delIT = dellist.begin(); delIT!=
						dellist.end(); delIT++){
							pDoc->CBIRElements->links.erase(*delIT);
					}
				}
				Invalidate();
			}
		break;
		case VK_NUMPAD1:
		case 49:
			if(id>=1)
				pDoc->CBIRElements->GetByID(id)->plugin->group= 1;		
			Invalidate();
			break;
		case VK_NUMPAD2:
		case 50:
			if(id>=1)pDoc->CBIRElements->GetByID(id)->plugin->group= 2;
			Invalidate();
			break;
		case VK_NUMPAD3:
		case 51:
			if(id>=1)pDoc->CBIRElements->GetByID(id)->plugin->group= 3;
			Invalidate();
			break;
		case VK_NUMPAD4:
		case 52:
			if(id>=1)pDoc->CBIRElements->GetByID(id)->plugin->group= 4;
			Invalidate();
			break;
		case VK_NUMPAD5:
		case 53:
			if(id>=1)pDoc->CBIRElements->GetByID(id)->plugin->group= 5;
			Invalidate();
			break;
		case VK_NUMPAD6:
		case 54:
			if(id>=1)pDoc->CBIRElements->GetByID(id)->plugin->group= 6;
			Invalidate();
			break;
		case VK_NUMPAD7:
		case 55:
			if(id>=1)pDoc->CBIRElements->GetByID(id)->plugin->group= 7;
			Invalidate();
			break;
		case VK_NUMPAD8:
		case 56:
			if(id>=1)pDoc->CBIRElements->GetByID(id)->plugin->group= 8;
			Invalidate();
			break;
		case VK_NUMPAD9:
		case 57:
			if(id>=1)pDoc->CBIRElements->GetByID(id)->plugin->group= 9;
			Invalidate();
			break;		

		case VK_DELETE:
			if(id>=1){
				msg = CString("Действительно удалить блок ");
				msg = msg+ pDoc->CBIRElements->GetByID(id)->GetName();
				msg += CString("?");

				if(IDYES == AfxMessageBox(msg, MB_YESNO)){
					//disconnect all pins  and delete all links for this element


					//vector<list<CCBIRQLink*>::iterator> dellist;//delete deque
					//scan for links
					for(list<CCBIRQLink*>::iterator it = pDoc->CBIRElements->links.begin(); it!=pDoc->CBIRElements->links.end(); it++){
						if ((*it)->end ==id){
							(*it)->endPin->UnLink();
							dellist.push_back(it);
							delete (*it);
						}
						if ((*it)->start ==id){
							(*it)->endPin->UnLink();
							dellist.push_back(it);
							delete (*it);
						}
					};
					//kill!!!
					for(vector<list<CCBIRQLink*>::iterator>::iterator delIT = dellist.begin(); delIT!=
						dellist.end(); delIT++){
							pDoc->CBIRElements->links.erase(*delIT);
					}
					//delete element					
					pDoc->CBIRElements->Remove(id);
					Invalidate();

				}
			};
		break;
	}
};
void CCBIRQEditorView::OnDraw(CDC* dc)
{
	CMemDC pDC(dc);
    CCBIRQEditorDoc* pDoc = GetDocument();

	pDoc->CBIRElements->draw(pDC);
}

void CCBIRQEditorView::OnLButtonDblClk(UINT nFlags, CPoint point){
	CCBIRQEditorDoc* pDoc = GetDocument();	
	
	if(pDoc->CBIRElements->isActive()){
		UINT id =pDoc->CBIRElements->GetActive();
		if(id>=1){
			pDoc->CBIRElements->doModal(id);
		};
	}else{
		pDoc->CBIRElements->addDialog(point);
		//pDoc->CBIRElements->add(point);
	};
	
	Invalidate();
};

void CCBIRQEditorView::OnRButtonDblClk(UINT nFlags, CPoint point){
	
	CCBIRQEditorDoc* pDoc = GetDocument();
	if(pDoc->CBIRElements->isActive()){
		UINT id =pDoc->CBIRElements->GetActive();

		if(id>=1 && !pDoc->CBIRElements->GetByID(id)->plugin->IsWorking){
			pDoc->CBIRElements->GetByID(id)->isRun = 1;			
			pDoc->CBIRElements->GetByID(id)->plugin->Next();
			pDoc->CBIRElements->GetByID(id)->isRun = 0;
		};
	}else{
	};
	Invalidate();
};

void CCBIRQEditorView::OnMouseMove(UINT nFlags, CPoint point){
	CCBIRQEditorDoc* pDoc = GetDocument();
	if (pDoc->CBIRElements->linkStart || pDoc->CBIRElements->isLDraging)
		if(pDoc->CBIRElements->updateState(point))
			Invalidate();//if some changed

	return;
};
void CCBIRQEditorView::OnLButtonDown(UINT nFlags, CPoint point){
	CCBIRQEditorDoc* pDoc = GetDocument();

	if(pDoc->CBIRElements->updateState(point))
		Invalidate();//if some changed

	pDoc->CBIRElements->startLDraging();
};
void CCBIRQEditorView::OnLButtonUp(UINT nFlags, CPoint point){
	CCBIRQEditorDoc* pDoc = GetDocument();	
	pDoc->CBIRElements->stopLDraging();
	Invalidate();
};

void CCBIRQEditorView::OnRButtonDown(UINT nFlags, CPoint point){
	CCBIRQEditorDoc* pDoc = GetDocument();	
	if(pDoc->CBIRElements->updateState(point))
		Invalidate();//if some changed
	pDoc->CBIRElements->startLinking();
};
void CCBIRQEditorView::OnRButtonUp(UINT nFlags, CPoint point){
	CCBIRQEditorDoc* pDoc = GetDocument();	
	pDoc->CBIRElements->stopLinking();
	Invalidate();
};

// CCBIRQEditorView printing

BOOL CCBIRQEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CCBIRQEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{}
void CCBIRQEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{}

// CCBIRQEditorView diagnostics

#ifdef _DEBUG
void CCBIRQEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CCBIRQEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCBIRQEditorDoc* CCBIRQEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCBIRQEditorDoc)));
	return (CCBIRQEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CCBIRQEditorView message handlers

void CCBIRQEditorView::OnRunbtn()
{
	AfxMessageBox("Run!");
	CCBIRQEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
}

void CCBIRQEditorView::OnScriptbtn()
{	
	CCBIRQEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString str = pDoc->script;
	CScriptDialog *dlg = new CScriptDialog(str);
	INT_PTR z = dlg->DoModal();	
	if (z==2 || z==1){
		str = dlg->script;
		pDoc->script = str;
	}
	if (z == 1){
		pDoc->RunScript();
	}
	Invalidate();
}

void CCBIRQEditorView::OnAddBtn()
{
	//AfxMessageBox("shalom!");
	CCBIRQEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Диалог для открытия DLL
	char szFilters[]=	"Module (*.dll;*.o)||";
	CFileDialog fileDlg (TRUE, "", "*.dll;*.o",
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);


	if( fileDlg.DoModal ()==IDOK ){
		CString pathName = fileDlg.GetPathName();
		string str(pathName.GetBuffer());
		pDoc->core->AddModule(str);
		//Вызов функции ядра загрузки DLL	
	}
	
	Invalidate();
}


BOOL CCBIRQEditorView::OnEraseBkgnd(CDC *pDC)
{
//	return CView::OnEraseBkgnd(pDC);	
	return FALSE;
}


void CCBIRQEditorView::OnHelpAddmodule()
{
	AfxMessageBox("Alasdair");
}
