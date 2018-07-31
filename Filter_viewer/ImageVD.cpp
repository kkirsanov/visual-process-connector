// ImageVD.cpp : implementation file
//

#include "stdafx.h"
#include "Filter_viewer.h"
#include "ImageVD.h"


// CImageVD dialog

IMPLEMENT_DYNAMIC(CImageVD, CDialog)

BOOL CImageVD::OnInitDialog(){
	BOOL z = CDialog::OnInitDialog();
	if(pbitmap.get()!=NULL){

		SCROLLINFO horz;

		int h =pbitmap->GetHeight();
		int w =pbitmap->GetWidth();

		CString str;
		str.Format("Размер: %d x %d",w,h);
		HSlider.SetRange(0, w-sizeX);
		VSlider.SetRange(0, h-sizeY);
		
		DataList.AddString(str);
		
	}
	return z;
}
CImageVD::CImageVD(PBitmap bitmap,bool* IsDialog,Core *core, CWnd* pParent /*=NULL*/)
	: CDialog(CImageVD::IDD, pParent)
{
	pIsD = IsDialog;
	*pIsD=true;
	pbitmap = bitmap;
	FirstStart =0;
	CImageVD::pCore = core;
	sizeX=400;
	sizeY=300;
	flag=0;
	IsP = false;
}


CImageVD::~CImageVD()
{
	*pIsD=false;
}

void CImageVD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, DataList);
	DDX_Control(pDX, IDC_SLIDER1, HSlider);
	DDX_Control(pDX, IDC_SLIDER2, VSlider);
}


BEGIN_MESSAGE_MAP(CImageVD, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CImageVD::OnBnClickedButton1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CImageVD::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, &CImageVD::OnNMReleasedcaptureSlider2)

	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CImageVD message handlers

void CImageVD::OnPaint()
{	
	CPaintDC dc(this); // device context for painting
	boost::mutex::scoped_lock lock(fillMutex);

	int startX=HSlider.GetPos();
	int startY=VSlider.GetPos();
	int toX = -1;
	int toY = -1;

	if(pbitmap.get()){
		pbitmap->Draw(dc,-startX,-startY,toX,toY, CRect(0,0,sizeX,sizeY));
	};

	if (!IsP)
		CDialog::OnPaint();

}
void CImageVD::OnBnClickedButton1()
{

	CString fformat = _T("");
	fformat += _T("All files - (*.*)|*.*|");
	CFileDialog fileDlg (TRUE, _T("Jpeg"), _T("*.jpg"), OFN_CREATEPROMPT| OFN_HIDEREADONLY, fformat);
	if( fileDlg.DoModal ()==IDOK ){
		pbitmap->Save(fileDlg.GetPathName(), CXIMAGE_FORMAT_JPG);			
	};
}

void CImageVD::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	Invalidate();
	*pResult = 0;
}

void CImageVD::OnNMReleasedcaptureSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	Invalidate();
	*pResult = 0;
}

void CImageVD::OnClose()
{
	flag=1;
	*pIsD=false;
	CDialog::OnClose();
	delete this;
}
