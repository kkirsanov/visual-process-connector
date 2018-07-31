#pragma once

#include "Resource.h"
// DirReaderD dialog

class DirReaderD : public CDialog
{
	DECLARE_DYNAMIC(DirReaderD)

public:
	DirReaderD(CWnd* pParent = NULL);   // standard constructor
	virtual ~DirReaderD();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
