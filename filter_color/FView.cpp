// FView.cpp : implementation file
//

#include "stdafx.h"
#include "filter_color.h"
#include "FView.h"


// CFView dialog

IMPLEMENT_DYNAMIC(CFView, CDialog)

void CFView::GeneratePalette(int val){
	int r,g,b;
	r=CR.GetPos();
	g=CG.GetPos();
	b=CB.GetPos();
	ColorDistance metric;

	palette  = PBitmap(new CxImage(255,255,24));

	ColorHandler myCol;
	RGBQUAD col;
	int z = CMVAL.GetPos();

	for(int h=0; h<255;h++){
		for(int s=0; s<255;s++){
			ColorHandler::_RGB RGB = myCol.HSVtoRGB(h,s,val);

			col.rgbRed=RGB.Red; //Мистика какая то! Не трогать! Так надо.
			col.rgbGreen=RGB.Green;
			col.rgbBlue=RGB.Blue;
;
		
			double d = metric.distanceHSV(col.rgbRed, col.rgbGreen, col.rgbBlue,r,g,b,mH,mS,mV);
			double d2=(double) z/1000;
			if(d<=d2){
				//col.Blue = 255;
				//col.Red = 255;
				//col.Green = 255;
			}else{
				col.rgbBlue = 0;
				col.rgbRed = 0;
				col.rgbGreen = 0;
			};

			//Да. Но в отличие от психотронных генераторов есть уйма косвенных доказательств НЛО.
			
			//palette

			palette->SetPixelColor(h,255-s,col);
			//gflSetColorAt(palette,h,255-s, &col);
		};
	}
};
void CFView::UpdateImage(){
	if(preview_Bitmap.get()){
		ColorDistance metric;
		RGBQUAD col;
		int r,g,b;
		r=CR.GetPos();
		g=CG.GetPos();
		b=CB.GetPos();
		int z = CMVAL.GetPos();

		for (int x = 0; x<preview_Bitmap->GetWidth();x++)
			for (int y = 0; y<preview_Bitmap->GetHeight();y++){				
				col = scaledBitmap->GetPixelColor(x,y);
				
				double d = metric.distanceHSV(col.rgbRed, col.rgbGreen, col.rgbBlue,r,g,b,mH,mS,mV);
				double d2=(double) z/1000;
				if(d<=d2){
					//col.Blue = 255;
					//col.Red = 255;
					//col.Green = 255;
				}else{
					col.rgbBlue = 0;
					col.rgbRed = 0;
					col.rgbGreen = 0;
				};
				preview_Bitmap->SetPixelColor(x,y,col ,false);
			}
	};
}
CFView::CFView(PBitmap bmp, int H, int S, int V,  int _mH, int _mS, int _mV, int _val, Core *core, CWnd* pParent /*= NULL*/)
	: CDialog(CFView::IDD, pParent)
{
	_h=H;
	_s=S;
	_v=V;

	mH=_mH;
	mS=_mS;
	mV=_mV;
	val=_val;
	
	bitmap = bmp;
	if(bitmap.get()){
		scaledBitmap = PBitmap(new CxImage(*bitmap));

		int maxsize = 356;				 
		double cof;

		if (scaledBitmap->GetWidth() > scaledBitmap->GetHeight()){
			cof = (double)maxsize / bitmap->GetWidth();
		}else{
			cof = (double)maxsize / bitmap->GetHeight();
		};
		if (cof<1)
			scaledBitmap->Resample(scaledBitmap->GetWidth()*cof, scaledBitmap->GetHeight()*cof);

		preview_Bitmap = PBitmap(new CxImage(*scaledBitmap));
	};
	CFView::pCore = core;
}

CFView::~CFView()
{
}

void CFView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_R, CR);
	DDX_Control(pDX, IDC_G, CG);
	DDX_Control(pDX, IDC_B, CB);
	DDX_Control(pDX, IDC_H, CH);
	DDX_Control(pDX, IDC_S, CS);
	DDX_Control(pDX, IDC_V, CV);
	DDX_Control(pDX, IDC_PC, PC);
	DDX_Control(pDX, IDC_SLIDER1, CMH);
	DDX_Control(pDX, IDC_SLIDER3, CMS);
	DDX_Control(pDX, IDC_SLIDER2, CMV);
	DDX_Control(pDX, IDC_SLIDER4, CMVAL);
}


BEGIN_MESSAGE_MAP(CFView, CDialog)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_R, &CFView::OnNMReleasedcaptureR)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_B, &CFView::OnNMReleasedcaptureB)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_G, &CFView::OnNMReleasedcaptureG)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_R, &CFView::OnNMCustomdrawR)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_H, &CFView::OnNMCustomdrawH)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_G, &CFView::OnNMCustomdrawG)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_B, &CFView::OnNMCustomdrawB)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_H, &CFView::OnNMReleasedcaptureH)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_S, &CFView::OnNMReleasedcaptureS)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_V, &CFView::OnNMReleasedcaptureV)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDOK, &CFView::OnBnClickedOk)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CFView::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, &CFView::OnNMReleasedcaptureSlider3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, &CFView::OnNMReleasedcaptureSlider2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER4, &CFView::OnNMReleasedcaptureSlider4)
END_MESSAGE_MAP()


// CFView message handlers

int CFView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here

	return 0;
}
void
CFView::UpdateColors(int col){
	if(!preview_Bitmap.get())
		return;

	if (col==0){
		int r,g,b;
		char z[50];
		r= CR.GetPos();
		g= CG.GetPos();
		b= CB.GetPos();
		itoa(r,z, 10);
		SetDlgItemText(IDC_STATICR, CString(z));
		itoa(g,z, 10);
		SetDlgItemText(IDC_STATICG, CString(z));
		itoa(b,z, 10);
		SetDlgItemText(IDC_STATICB, CString(z));

		//convert RGB to HSV
		ColorHandler myCol;

		ColorHandler::HSV HSV = myCol.RGBtoHSV(ColorHandler::_RGB(r, g, b));
	
		_h=HSV.Hue;
		_s=HSV.Saturation;
		_v=HSV.value;

		itoa(_h,z, 10);
		SetDlgItemText(IDC_STATICH, CString(z));
		itoa(_s,z, 10);
		SetDlgItemText(IDC_STATICS, CString(z));
		itoa(_v,z, 10);
		SetDlgItemText(IDC_STATICV, CString(z));
		CH.SetPos(_h);
		CS.SetPos(_s);
		CV.SetPos(_v);
		
	}else{
		int r,g,b;
		char z[50];
		_h=CH.GetPos();
		_s=CS.GetPos();
		_v=CV.GetPos();

		itoa(_h,z, 10);
		SetDlgItemText(IDC_STATICH, CString(z));
		itoa(_s,z,10);
		SetDlgItemText(IDC_STATICS, CString(z));
		itoa(_v,z,10);
		SetDlgItemText(IDC_STATICV, CString(z));

		//convert RGB to HSV
		ColorHandler myCol;

		ColorHandler::_RGB RGB = myCol.HSVtoRGB(_h,_s,_v);
		
		r=RGB.Red;
		g=RGB.Green;
		b=RGB.Blue;

		itoa(r,z, 10);
		SetDlgItemText(IDC_STATICR, CString(z));
		itoa(g,z, 10);
		SetDlgItemText(IDC_STATICG, CString(z));
		itoa(b,z, 10);
		SetDlgItemText(IDC_STATICB, CString(z));
		CR.SetPos(r);
		CG.SetPos(g);
		CB.SetPos(b);
		
	}
	
	mH = CMH.GetPos();
	mS = CMS.GetPos();
	mV = CMV.GetPos();
	val= CMVAL.GetPos();

	UpdateImage();
	Invalidate(false);
}

void CFView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

}
BOOL CFView::OnInitDialog(){
	BOOL z = CDialog::OnInitDialog();
	CR.SetRange(0, 255);
	CG.SetRange(0, 255);
	CB.SetRange(0, 255);
	CH.SetRange(0, 255);
	CS.SetRange(0, 255);
	CV.SetRange(0, 255);
	CV.SetPos(128);

	CMH.SetRange(0,100);
	CMS.SetRange(0,100);
	CMV.SetRange(0,100);
	CMVAL.SetRange(0,1000);
	CMH.SetPos(mH);
	CMS.SetPos(mS);
	CMV.SetPos(mV);
	CMVAL.SetPos(val);
	
	
	CH.SetPos(_h);
	CS.SetPos(_s);
	CV.SetPos(_v);

	UpdateColors(1);

	return z;
}

void CFView::OnNMReleasedcaptureR(NMHDR *pNMHDR, LRESULT *pResult)
{	
	UpdateColors();	
	*pResult = 0;
}

void CFView::OnNMReleasedcaptureB(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateColors();	
	*pResult = 0;
}

void CFView::OnNMReleasedcaptureG(NMHDR *pNMHDR, LRESULT *pResult)
{		
	UpdateColors();
	*pResult = 0;
}

void CFView::OnNMCustomdrawR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}

void CFView::OnNMCustomdrawG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}

void CFView::OnNMCustomdrawB(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}
void CFView::OnNMCustomdrawH(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}
void CFView::OnNMReleasedcaptureH(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateColors(1);
	*pResult = 0;
}

void CFView::OnNMReleasedcaptureS(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateColors(1);
	*pResult = 0;
}

void CFView::OnNMReleasedcaptureV(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateColors(1);
	GeneratePalette(CV.GetPos());

	*pResult = 0;
}

void CFView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	GeneratePalette(CV.GetPos());
	if(preview_Bitmap.get())
		preview_Bitmap->Draw(dc,0,0);
	
	int h=0,s=0;	

	int x=374;
	int y=160;

	if (palette.get()!= NULL)
		palette->Draw(dc, x,y);


	dc.SetPixel(_h+x,_s+y, RGB(255-CR.GetPos(), 255-CG.GetPos(), 255-CB.GetPos()));
	dc.SetPixel(_h+x+1,_s+y, RGB(255-CR.GetPos(), 255-CG.GetPos(), 255-CB.GetPos()));
	dc.SetPixel(_h+x-1,_s+y, RGB(255-CR.GetPos(), 255-CG.GetPos(), 255-CB.GetPos()));
	dc.SetPixel(_h+x,_s+y+1, RGB(255-CR.GetPos(), 255-CG.GetPos(), 255-CB.GetPos()));
	dc.SetPixel(_h+x,_s+y-1, RGB(255-CR.GetPos(), 255-CG.GetPos(), 255-CB.GetPos()));

	CBrush z(RGB(CR.GetPos(),CG.GetPos(),CB.GetPos()));
	dc.FillRect(CRect(x-10,y-10,x,y),&z);

}
void CFView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (preview_Bitmap.get())
	if(point.x>1 && point.x<preview_Bitmap->GetWidth()-1 && point.y>1 && point.y<preview_Bitmap->GetHeight()-1){
		RGBQUAD col1 = scaledBitmap->GetPixelColor(point.x,scaledBitmap->GetHeight()- point.y);
		CString rg;
		rg.Format("%d, %d, %d", col1.rgbRed, col1.rgbGreen, col1.rgbBlue);
		SetWindowText(rg);
	}
}
void CFView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!preview_Bitmap.get())
		return;
	if(point.x>374 && point.x<374+255 && point.y>160 && point.y<160+255){
		_h=point.x-374;
		_s=point.y-160;

		CH.SetPos(_h);
		CS.SetPos(_s);
		CH.Invalidate();
		CS.Invalidate();
		UpdateColors(1);
	};
	if(point.x>1 && point.x<preview_Bitmap->GetWidth()-1 && point.y>1 && point.y<preview_Bitmap->GetHeight()-1){
		RGBQUAD col1;
		//palette->SetPixelColor(h,255-s,col);
		col1 = scaledBitmap->GetPixelColor(point.x,scaledBitmap->GetHeight()-point.y);

		CR.SetPos(col1.rgbRed);
		CG.SetPos(col1.rgbGreen);
		CB.SetPos(col1.rgbBlue);

		UpdateColors(0);
	};
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CFView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CFView::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateColors(1);
	*pResult = 0;
}

void CFView::OnNMReleasedcaptureSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateColors(1);
	*pResult = 0;
}

void CFView::OnNMReleasedcaptureSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateColors(1);
	*pResult = 0;
}

void CFView::OnNMReleasedcaptureSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateColors(1);
	*pResult = 0;
}
