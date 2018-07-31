// PanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filter_panorama.h"
#include "PanDlg.h"


// PanDlg dialog

IMPLEMENT_DYNAMIC(PanDlg, CDialog)

PanDlg::PanDlg(int s, int t, vector<Image *> *images,Core *core, CWnd* pParent /*=NULL*/)
: CDialog(PanDlg::IDD, pParent)
	, summCount(s)
	, timeStep(t)	
{
	pImages = images;
	pCore = core;
}

PanDlg::~PanDlg()
{
}

void PanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, sSpin);
	DDX_Control(pDX, IDC_SPIN2, tSpin);
	DDX_Control(pDX, IDC_EDIT1, samplesSpin);
	DDX_Control(pDX, IDC_EDIT2, TimeSpin);
}

BEGIN_MESSAGE_MAP(PanDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &PanDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



void PanDlg::OnBnClickedButton1()
{
	summCount= GetDlgItemInt(IDC_EDIT1);
	timeStep = GetDlgItemInt(IDC_EDIT2);
	OnOK();
}

void PanDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (pImages->empty())
		return;

	int y=180;
	int x=0;
	int xStep =pImages->at(0)->pBitmap->GetWidth();
	int height =pImages->at(0)->pBitmap->GetHeight();
	
	int size = pImages->size();
	
	CRect rect;
	GetWindowRect(rect);

	double divFactor = (double)(rect.right -rect.left)/(pImages->at(0)->pBitmap->GetWidth() * size);

	if (divFactor>1)
		divFactor=1;

	xStep =pImages->at(0)->pBitmap->GetWidth()*divFactor;
	height =pImages->at(0)->pBitmap->GetHeight()*divFactor;

	for(vector<Image*>::iterator it= pImages->begin();it!=pImages->end(); it++){
		CxImage scaledBitmap;

		(*it)->pBitmap->Resample(xStep,height, 1, &scaledBitmap);
		scaledBitmap.Draw(dc,x,y);

		//dc.Rectangle(x,y,x+xStep,y+height);
		x+=xStep;
		//break;
	}

	CxImage scaledBitmap;
	pImages->at(Active)->pBitmap->Resample(290,180, 1, &scaledBitmap);
	scaledBitmap.Draw(dc, 170, 0);
	
	x+=xStep;

}
void PanDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int size = pImages->size();
	if (size>1){
		CRect rect;		
		GetWindowRect(rect);

		int w = rect.right-rect.left;
		double w2 = (double)point.x/w;
		int old = Active;
		Active = w2*size;

		if (Active!=old){
			InvalidateRect(CRect(150,0,150+320,200),FALSE);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}
