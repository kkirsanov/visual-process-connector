// Logview.cpp : implementation file
//

#include "stdafx.h"
#include "CBIRQEditor.h"
#include "Logview.h"


// CLogview dialog

IMPLEMENT_DYNAMIC(CLogview, CDialog)

CLogview::CLogview(CWnd* pParent /*=NULL*/)
	: CDialog(CLogview::IDD, pParent)
{

}

CLogview::~CLogview()
{
}

void CLogview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogview, CDialog)
END_MESSAGE_MAP()


// CLogview message handlers
