

#pragma once

// CTabCtrlView.h : header file
//

#include "afxcmn.h"

class CTabCtrlView;

class CViewTabCtl : public CTabCtrl
{
// Construction
public:
        CViewTabCtl();

// Attributes
public:

// Operations
public:
        
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CViewTabCtl)
        //}}AFX_VIRTUAL

// Implementation
public:
        void SetView(CTabCtrlView* pView);
        virtual void RecalcLayout(CRect& rect, CWnd* wnd);
        virtual ~CViewTabCtl();
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

        // Generated message map functions
protected:
        CTabCtrlView* m_pView;
        CString m_sGrayFont;
        CString m_sSelFont;
        CDC m_dc;
        CFont m_selFont;
        virtual BOOL HandleTabs(int sel);
        //{{AFX_MSG(CViewTabCtl)
        afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCTabCtrlView window

// The actual tab control
// Wrapper Window Handles Interfacing to MFC doc View Model
// and resizing of the Tab Ctrl
class CTabCtrlView : public CWnd
{
// Construction
public:
        CTabCtrlView();

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCTabCtrlView)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual BOOL HandleTabs(int sel);
        void SetTab(int Tab);
        virtual void SetView();
        void RecalcLayout();
        CView* GetActiveView();
        BOOL CreateStatic(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE, UINT nID = AFX_IDW_PANE_FIRST);
        virtual CView* CreateView(CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext );
        virtual ~CTabCtrlView();

        // Generated message map functions
protected:
        virtual void InitTabs(CTabCtrlView* pView);
        CViewTabCtl m_TabCtl;
        CView* m_ActiveView;
        
        BOOL CreateCommon(CWnd* pParentWnd, SIZE sizeMin, DWORD dwStyle, UINT nID);
        //{{AFX_MSG(CCTabCtrlView)
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnPaint();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CViewTabCtl window