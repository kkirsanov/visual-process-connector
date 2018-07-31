// CBIRQEditorView.h : interface of the CCBIRQEditorView class
//


#pragma once
#include "CBIRQEditorDoc.h"

class CCBIRQEditorView : public CView
{
protected: // create from serialization only
	CCBIRQEditorView();
	DECLARE_DYNCREATE(CCBIRQEditorView)

// Attributes
public:
	CCBIRQEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCBIRQEditorView();
	CPoint mPosition;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void CCBIRQEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);//OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags,  CPoint point);	
	afx_msg void OnLButtonUp(UINT nFlags,  CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags,  CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags,  CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags,  CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags,  CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
public:
	afx_msg void OnRunbtn();
	afx_msg void OnAddBtn();
	afx_msg void OnScriptbtn();
public:
	afx_msg void OnHelpAddmodule();
};

#ifndef _DEBUG  // debug version in CBIRQEditorView.cpp
inline CCBIRQEditorDoc* CCBIRQEditorView::GetDocument() const
   { return reinterpret_cast<CCBIRQEditorDoc*>(m_pDocument); }
#endif

