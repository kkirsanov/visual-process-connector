// CTabCtrlView.cpp : implementation file
//

// core headers
#include "afx.h"
#include "afxplex_.h"
#include "afxcoll.h"
#include "afxcmn.h"

#include "stdafx.h"

#include "CTabCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CX_BORDER 1
#define CY_BORDER 1

#define MAX_COLORS 10

// The following colors give a Tab Ctrl an OS/2 look
COLORREF colorRef[MAX_COLORS] =
{
        RGB(0,225,255),
        RGB(0,240,190),
        RGB(128,128,255),
        RGB(240,200,175),
        RGB(240,240,150),
        RGB(175,130,175),
        RGB(240,140,0),
        RGB(255,200,0),
        RGB(255,160,120),
        RGB(255,200, 175)
};


/////////////////////////////////////////////////////////////////////////////
// CTabCtrlView

CTabCtrlView::CTabCtrlView()
{
}

CTabCtrlView::~CTabCtrlView()
{
}


BEGIN_MESSAGE_MAP(CTabCtrlView, CWnd)
        //{{AFX_MSG_MAP(CTabCtrlView)
        ON_WM_SIZE()
        ON_WM_ERASEBKGND()
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlView message handlers

BOOL CTabCtrlView::CreateStatic(CWnd * pParentWnd, DWORD dwStyle, UINT nID)
{
        ASSERT(pParentWnd != NULL);
        ASSERT(dwStyle & WS_CHILD);
        ASSERT(!(dwStyle & SPLS_DYNAMIC_SPLIT));

        // create with zero minimum pane size
        if (!CreateCommon(pParentWnd, CSize(0, 0), dwStyle, nID))
                return FALSE;

        // all panes must be created with explicit calls to CreateView
        return TRUE;
}





BOOL CTabCtrlView::CreateCommon(CWnd * pParentWnd, SIZE sizeMin, DWORD dwStyle, UINT nID)
{
        ASSERT(pParentWnd != NULL);
        ASSERT(sizeMin.cx >= 0 && sizeMin.cy >= 0);
        ASSERT(dwStyle & WS_CHILD);
        ASSERT(nID != 0);

        // create with the same wnd-class as MDI-Frame (no erase bkgnd)
        if (!CreateEx(0, NULL, NULL, dwStyle, 0, 0, 0, 0,
          pParentWnd->m_hWnd, (HMENU)nID, NULL))
                return FALSE;       // create invisible

        //Create the Tab Control
        CRect rect;
        GetClientRect(rect);
        CImageList pImageList;


        m_TabCtl.Create(WS_VISIBLE | WS_CHILD | TCS_OWNERDRAWFIXED, rect, this, nID);

        //Overide this function to provide your Tabs
        InitTabs(this);
        return TRUE;
}

CView* CTabCtrlView::CreateView(CRuntimeClass * pViewClass, SIZE sizeInit, CCreateContext * pContext)
{
#ifdef _DEBUG
        ASSERT_VALID(this);
        ASSERT(pViewClass != NULL);
        ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
        ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

        BOOL bSendInitialUpdate = FALSE;

        CCreateContext contextT;
        if (pContext == NULL)
        {
                // if no context specified, generate one from the currently selected
                //  client if possible
                CView* pOldView = NULL;
                if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
                {
                        // set info about last pane
                        ASSERT(contextT.m_pCurrentFrame == NULL);
                        contextT.m_pLastView = pOldView;
                        contextT.m_pCurrentDoc = pOldView->GetDocument();
                        if (contextT.m_pCurrentDoc != NULL)
                                contextT.m_pNewDocTemplate =
                                  contextT.m_pCurrentDoc->GetDocTemplate();
                }
                pContext = &contextT;
                bSendInitialUpdate = TRUE;
        }

        CWnd* pWnd;
        TRY
        {
                pWnd = (CWnd*)pViewClass->CreateObject();
                if (pWnd == NULL)
                        AfxThrowMemoryException();
        }
        CATCH_ALL(e)
        {
                TRACE0("Out of memory creating a splitter pane.\n");
                // Note: DELETE_EXCEPTION(e) not required
                return (CView*) NULL;
        }
        END_CATCH_ALL

        ASSERT_KINDOF(CWnd, pWnd);
        ASSERT(pWnd->m_hWnd == NULL);       // not yet created

        DWORD dwStyle = AFX_WS_DEFAULT_VIEW;

        // Create with the right size (wrong position)
        CRect rect(CPoint(0,0), sizeInit);
        if (!pWnd->Create(NULL, NULL, dwStyle,
                rect, this, 0, pContext))
        {
                TRACE0("Warning: couldn't create client pane for splitter.\n");
                        // pWnd will be cleaned up by PostNcDestroy
                return (CView*) NULL;
        }

        // send initial notification message
        if (bSendInitialUpdate);
//              pWnd->SendMessage(WM_INITIALUPDATE);
        m_ActiveView = (CView*) pWnd;
        return m_ActiveView;
}

void CTabCtrlView::OnSize(UINT nType, int cx, int cy) 
{
        if (nType != SIZE_MINIMIZED && cx > 0 && cy > 0)
                RecalcLayout();

        CWnd::OnSize(nType, cx, cy);
        return;
}

void CTabCtrlView::RecalcLayout()
{
        CWnd* pWnd = (CWnd*) GetActiveView();
        CRect rect;
        GetClientRect(&rect);
        m_TabCtl.RecalcLayout(rect, pWnd);
}

CView* CTabCtrlView::GetActiveView()
{
        return m_ActiveView;
}

BOOL CTabCtrlView::OnEraseBkgnd(CDC* pDC) 
{
        return FALSE;
}

void CTabCtrlView::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
}

void CTabCtrlView::SetView()
{
        //In most cases your main app window
        //should handle this. This is becuase
        //the doc view model expects the view
        //to be attached to your main frame
}

void CTabCtrlView::SetTab(int Tab)
{
        m_TabCtl.SetCurSel(Tab);
}

void CTabCtrlView::InitTabs(CTabCtrlView* pView)
{
        m_TabCtl.SetView(pView);
        return;
}

BOOL CTabCtrlView::HandleTabs(int sel)
{
        ASSERT(FALSE);
        return FALSE;
}






/////////////////////////////////////////////////////////////////////////////
// CViewTabCtl

CViewTabCtl::CViewTabCtl()
{
        m_sSelFont = _T("Helv");
        m_sGrayFont= _T("Helv");
}

CViewTabCtl::~CViewTabCtl()
{
}


BEGIN_MESSAGE_MAP(CViewTabCtl, CTabCtrl)
        //{{AFX_MSG_MAP(CViewTabCtl)
        ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTabCtl message handlers

void CViewTabCtl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
        // TODO: Add your control notification handler code here
        int nSel = GetCurSel();
        
        HandleTabs(nSel);
        
        *pResult = 0;
}

BOOL CViewTabCtl::HandleTabs(int sel)
{
        return m_pView->HandleTabs(sel);
}

void CViewTabCtl::RecalcLayout(CRect & rect, CWnd * wnd)
{
        SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);

        int ind = GetCurSel();
        AdjustRect(FALSE, &rect);
        wnd->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
}

void CViewTabCtl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
        int nSel = lpDrawItemStruct->itemID;
        ASSERT(nSel > -1);
        ASSERT(nSel <  GetItemCount()); 

        TC_ITEM item;
        char text[255];

        item.pszText = text;
        item.mask = TCIF_TEXT;
        GetItem(nSel, &item);

        if(!m_dc.Attach(lpDrawItemStruct->hDC))
                return;

        CRect rect = CRect(&(lpDrawItemStruct->rcItem));

        rect.NormalizeRect();
        rect.DeflateRect(CX_BORDER, CY_BORDER);

        CBrush brush(colorRef[nSel - (nSel / MAX_COLORS) * MAX_COLORS]);
        
        m_dc.FillRect(rect, &brush);

        
        COLORREF tcolor;
        if (nSel == GetCurSel())
        {
                m_selFont.DeleteObject();
                m_selFont.CreatePointFont(100, LPCTSTR(m_sGrayFont), &m_dc);
                m_dc.SelectObject(m_selFont);
                tcolor = RGB(0,0,0);
                
        }
        else
        {
                m_selFont.DeleteObject();
                m_selFont.CreatePointFont(80, LPCTSTR(m_sSelFont), &m_dc);
                m_dc.SelectObject(m_selFont);
                tcolor = GetSysColor(COLOR_3DSHADOW);
        }
        m_dc.SetBkMode(TRANSPARENT);
        m_dc.SetTextColor(tcolor);
        m_dc.DrawText(text, rect, DT_VCENTER|DT_CENTER);
        m_dc.Detach();
        return;
}

void CViewTabCtl::SetView(CTabCtrlView * pView)
{
        m_pView = pView;
        return;
}