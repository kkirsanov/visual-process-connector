#pragma once

#include "resource.h"
// CPictureViewr dialog

using namespace CBIR;
class CPictureViewr : public CDialog
{
	DECLARE_DYNAMIC(CPictureViewr)

public:
	CPictureViewr(GFL_BITMAP *img,CBIR::Core  *core, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPictureViewr();

// Dialog Data
	enum { IDD = IDD_PICSHOW };

private:
GFL_BITMAP *m_img;
CBIR::Core *m_core;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
