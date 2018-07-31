// SortViewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Logic_sort.h"
#include "SortViewDialog.h"


// CSortViewDialog dialog

IMPLEMENT_DYNAMIC(CSortViewDialog, CDialog)

CSortViewDialog::CSortViewDialog(CBIR::Core *pCore, vector<Logic> *logics, CWnd* pParent /*=NULL*/)
	: CDialog(CSortViewDialog::IDD, pParent)
{
	CSortViewDialog::pCore = pCore;
	CSortViewDialog::logics = logics;
	bitmap=NULL;

}

CSortViewDialog::~CSortViewDialog()
{
}

void CSortViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, picList);
}

BOOL CSortViewDialog::OnInitDialog(){
	CDialog::OnInitDialog();

	vector<Logic>::iterator it;
	for(it = logics->begin();it != logics->end(); it++){
		picList.AddString( (*it).path);
	};
	bitmap=NULL;

	return TRUE;
};


BEGIN_MESSAGE_MAP(CSortViewDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_LIST1, &CSortViewDialog::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CSortViewDialog message handlers

void CSortViewDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(bitmap)
		bitmap->Draw(dc,0,0);
}

int CSortViewDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	
}

//void CSortViewDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialog::OnActivate(nState, pWndOther, bMinimized);
//}

void CSortViewDialog::OnLbnSelchangeList1()
{
	
	if (bitmap)
		delete bitmap;
	int z = picList.GetCurSel();
	CString path = (*logics)[z].path;
	bitmap = pCore->getImageFromFile(false, path);
	int maxsize = 350;				 
	double cof;
	if (bitmap->GetWidth() > bitmap->GetHeight()){
		cof = (double)maxsize / bitmap->GetWidth();
	}else{
		cof = (double)maxsize / bitmap->GetHeight();
	};
	if (cof<1)
		bitmap->Resample(bitmap->GetWidth()*cof, bitmap->GetHeight()*cof );

	Invalidate();
}