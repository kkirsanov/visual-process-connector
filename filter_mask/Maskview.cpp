// Maskview.cpp : implementation file
//

#include "stdafx.h"
#include "filter_mask.h"
#include "Maskview.h"


// Maskview dialog

IMPLEMENT_DYNAMIC(Maskview, CDialog)

Maskview::Maskview(MaskPalette *pmp,Mask &m1, Mask &m2, Mask &m3,CString &ColorModel, CWnd* pParent /*=NULL*/)
	: CDialog(Maskview::IDD, pParent),
	pMaskPalette(pmp),
	pM1(&m1),
	pM2(&m2),
	pM3(&m3),
	pColorModel(&ColorModel)

{

}

Maskview::~Maskview()
{
	for(vector<CEdit*>::iterator it = M1.begin();it!=M1.end();it++){
		delete *it;
	};

	for(vector<CEdit*>::iterator it = M2.begin();it!=M2.end();it++){
		delete *it;
	};

	for(vector<CEdit*>::iterator it = M3.begin();it!=M3.end();it++){
		delete *it;
	};
}

void Maskview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CPS, CPS);
	DDX_Control(pDX, IDC_LIST3, FilterSelect);
	DDX_Control(pDX, IDC_LIST4, M1Size);
	DDX_Control(pDX, IDC_LIST5, M2Size);
	DDX_Control(pDX, IDC_LIST6, M3Size);
}

BOOL Maskview::OnInitDialog(){
	BOOL z = CDialog::OnInitDialog();
	CPS.AddString("HSL");
	CPS.AddString("RGB");
	CPS.AddString("XYZ");
	CPS.AddString("YIQ");
	CPS.AddString("YUV");
	
	SetDlgItemInt(IDC_Divisor1, pM1->divisor);
	SetDlgItemInt(IDC_Divisor2, pM2->divisor);
	SetDlgItemInt(IDC_Divisor3, pM3->divisor);

	SetDlgItemInt(IDC_BAIS1, pM1->bais);
	SetDlgItemInt(IDC_BAIS2, pM2->bais);
	SetDlgItemInt(IDC_BAIS3, pM3->bais);

	if (*pColorModel==CString ("HSL"))
		CPS.SetCurSel(0);
	if (*pColorModel==CString ("RGB"))
		CPS.SetCurSel(1);
	if (*pColorModel==CString ("XYZ"))
		CPS.SetCurSel(2);
	if (*pColorModel==CString ("YIQ"))
		CPS.SetCurSel(3);
	if (*pColorModel==CString ("YUV"))
		CPS.SetCurSel(4);

	for(vector<Mask*>::iterator it = pMaskPalette->begin(); it!=pMaskPalette->end();it++){
		FilterSelect.AddString(CString((*it)->name.c_str()));
	}	


	M1Size.AddString("3x3");
	M1Size.AddString("5x5");
	M1Size.AddString("7x7");
	M1Size.AddString("Disable");
	M1Size.SetCurSel(3);

	if(pM1->mask.size()==3)
		M1Size.SetCurSel(0);
	if(pM1->mask.size()==5)
		M1Size.SetCurSel(1);
	if(pM1->mask.size()==7)
		M1Size.SetCurSel(2);

	M2Size.AddString("3x3");
	M2Size.AddString("5x5");
	M2Size.AddString("7x7");
	M2Size.AddString("Disable");	
	M2Size.SetCurSel(3);

	if(pM2->mask.size()==3)
		M2Size.SetCurSel(0);
	if(pM2->mask.size()==5)
		M2Size.SetCurSel(1);
	if(pM2->mask.size()==7)
		M2Size.SetCurSel(2);

	M3Size.AddString("3x3");
	M3Size.AddString("5x5");
	M3Size.AddString("7x7");
	M3Size.AddString("Disable");
	M3Size.SetCurSel(3);

	if(pM3->mask.size()==3)
		M3Size.SetCurSel(0);
	if(pM3->mask.size()==5)
		M3Size.SetCurSel(1);
	if(pM3->mask.size()==7)
		M3Size.SetCurSel(2);

	for (int i=0;i<7*7;i++){
		M1.push_back(new CEdit());
		M2.push_back(new CEdit());
		M3.push_back(new CEdit());
	}

	int x,y;
	for (int i=0;i<7*7;i++){
		x=i%7;
		y=i/7;
		int xStep=25;
		int yStep=17;
		int newX = x*xStep;
		int newY = y*yStep;
		M1.at(i)->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(newX,newY, newX+xStep, newY+yStep),this , 4000+i);

		newX+=7*xStep+20;
		M2.at(i)->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(newX,newY, newX+xStep, newY+yStep),this , 4100+i);

		newX+=7*xStep+20;
		M3.at(i)->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(newX,newY, newX+xStep, newY+yStep),this , 4200+i);

	}

	//fill values for 1		
	int size = pM1->mask.size();
	if (size==3)M1Size.SetCurSel(0);
	if (size==5)M1Size.SetCurSel(1);
	if (size==7)M1Size.SetCurSel(2);
	M1Size.UpdateData();


	//Заполнить поле 1
	int startX=7/2-size/2;
	int startY=7/2-size/2;
	int endX=7/2+size/2;
	int endY=7/2+size/2;

	for(int y=0;y<7;y++){
		for(int x=0;x<7;x++){
			if (size==0) break;
			if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
				CString z;
				int _x = x-startX;
				int _y = y-startY;
				
				int val = pM1->mask.at(_x).at(_y);
				z.Format("%d", val);
				M1.at(x+y*7)->SetWindowTextA(z);
			}else{
				M1.at(x+y*7)->SetWindowText("");
			}
		}
	};
	
	
	size = pM2->mask.size();
	if (size==3)M2Size.SetCurSel(0);
	if (size==5)M2Size.SetCurSel(1);
	if (size==7)M2Size.SetCurSel(2);
	M2Size.UpdateData();

	//Заполнить поле 2
	startX=7/2-size/2;
	startY=7/2-size/2;
	endX=7/2+size/2;
	endY=7/2+size/2;

	for(int y=0;y<7;y++){
		for(int x=0;x<7;x++){
			if (size==0) break;
			if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
				CString z;
				int _x = x-startX;
				int _y = y-startY;
				
				int val = pM2->mask.at(_x).at(_y);
				z.Format("%d", val);
				M2.at(x+y*7)->SetWindowTextA(z);
			}else{
				M2.at(x+y*7)->SetWindowText("");
			}
		}
	};

	size = pM3->mask.size();
	if (size==3)M3Size.SetCurSel(0);
	if (size==5)M3Size.SetCurSel(1);
	if (size==7)M3Size.SetCurSel(2);
	M3Size.UpdateData();

	//Заполнить поле 2
	startX=7/2-size/2;
	startY=7/2-size/2;
	endX=7/2+size/2;
	endY=7/2+size/2;

	for(int y=0;y<7;y++){
		for(int x=0;x<7;x++){
			if (size==0) break;
			if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
				CString z;
				int _x = x-startX;
				int _y = y-startY;
				
				int val = pM3->mask.at(_x).at(_y);
				z.Format("%d", val);
				M3.at(x+y*7)->SetWindowTextA(z);
			}else{
				M3.at(x+y*7)->SetWindowText("");
			}
		}
	};

	OnLbnSelchangeList4(); //upadeSize;
	OnLbnSelchangeList5(); //upadeSize;
	OnLbnSelchangeList6(); //upadeSize;
	return z;
}

BEGIN_MESSAGE_MAP(Maskview, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST4, &Maskview::OnLbnSelchangeList4)
	ON_BN_CLICKED(IDC_BUTTON1, &Maskview::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Maskview::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST5, &Maskview::OnLbnSelchangeList5)
	ON_LBN_SELCHANGE(IDC_LIST6, &Maskview::OnLbnSelchangeList6)
	ON_BN_CLICKED(IDC_BUTTON3, &Maskview::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &Maskview::OnBnClickedOk)
END_MESSAGE_MAP()


// Maskview message handlers

void Maskview::OnLbnSelchangeList4()
{
	
	if(M1Size.GetCurSel()==0){//3
		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( x>=2 && x<=4 && y>=2 && y<=4){
					M1.at(x+y*7)->EnableWindow();
				}else{
					M1.at(x+y*7)->EnableWindow(false);
				}
			}
		};
	};
	if(M1Size.GetCurSel()==1){//5
		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( x>=1 && x<=5 && y>=1 && y<=5){
					M1.at(x+y*7)->EnableWindow();
				}else{
					M1.at(x+y*7)->EnableWindow(false);
				}
			}
		};
	};
	if(M1Size.GetCurSel()==2){//7
		for(int y=0;y<7;y++)
			for(int x=0;x<7;x++)
				M1.at(x+y*7)->EnableWindow();
	};
	if(M1Size.GetCurSel()==3){//disable
		for(int y=0;y<7;y++)
			for(int x=0;x<7;x++)
				M1.at(x+y*7)->EnableWindow(false);
	};
}

void Maskview::OnBnClickedButton1()
{
	int sel = FilterSelect.GetCurSel();

	if (sel>=0){		
		int divisor = pMaskPalette->at(sel)->divisor;
		SetDlgItemInt(IDC_Divisor1, divisor);
		
		int bais = pMaskPalette->at(sel)->bais;
		SetDlgItemInt(IDC_BAIS1, bais);
		
		int size = pMaskPalette->at(sel)->mask.size();
		if (size==3)
			M1Size.SetCurSel(0);
		if (size==5)
			M1Size.SetCurSel(1);
		if (size==7)
			M1Size.SetCurSel(2);
		M1Size.UpdateData();
		OnLbnSelchangeList4(); //upadeSize;

		//Заполнить поле
		int startX=7/2-size/2;
		int startY=7/2-size/2;
		int endX=7/2+size/2;
		int endY=7/2+size/2;

		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
					CString z;
					int _x = x-startX;
					int _y = y-startY;
					
					int val = pMaskPalette->at(sel)->mask.at(_x).at(_y);
					z.Format("%d", val);
					M1.at(x+y*7)->SetWindowTextA(z);
				}else{
					M1.at(x+y*7)->SetWindowText("");
				}
			}
		};

	};
}

void Maskview::OnBnClickedButton2()
{
	int sel = FilterSelect.GetCurSel();

	if (sel>=0){		
		int divisor = pMaskPalette->at(sel)->divisor;
		SetDlgItemInt(IDC_Divisor2, divisor);
		
		int bais = pMaskPalette->at(sel)->bais;
		SetDlgItemInt(IDC_BAIS2, bais);
		
		int size = pMaskPalette->at(sel)->mask.size();
		if (size==3)
			M2Size.SetCurSel(0);
		if (size==5)
			M2Size.SetCurSel(1);
		if (size==7)
			M2Size.SetCurSel(2);
		M2Size.UpdateData();
		OnLbnSelchangeList5(); //upadeSize;

		//Заполнить поле
		int startX=7/2-size/2;
		int startY=7/2-size/2;
		int endX=7/2+size/2;
		int endY=7/2+size/2;

		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
					CString z;
					int _x = x-startX;
					int _y = y-startY;
					
					int val = pMaskPalette->at(sel)->mask.at(_x).at(_y);
					z.Format("%d", val);
					M2.at(x+y*7)->SetWindowTextA(z);
				}else{
					M2.at(x+y*7)->SetWindowText("");
				}
			}
		};

	};
}

void Maskview::OnLbnSelchangeList5()
{
	if(M2Size.GetCurSel()==0){//3
		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( x>=2 && x<=4 && y>=2 && y<=4){
					M2.at(x+y*7)->EnableWindow();
				}else{
					M2.at(x+y*7)->EnableWindow(false);
				}
			}
		};
	};
	if(M2Size.GetCurSel()==1){//5
		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( x>=1 && x<=5 && y>=1 && y<=5){
					M2.at(x+y*7)->EnableWindow();
				}else{
					M2.at(x+y*7)->EnableWindow(false);
				}
			}
		};
	};
	if(M2Size.GetCurSel()==2){//7
		for(int y=0;y<7;y++)
			for(int x=0;x<7;x++)
				M2.at(x+y*7)->EnableWindow();
	};
	if(M2Size.GetCurSel()==3){//disable
		for(int y=0;y<7;y++)
			for(int x=0;x<7;x++)
				M2.at(x+y*7)->EnableWindow(false);
	};
}

void Maskview::OnLbnSelchangeList6()
{
	if(M3Size.GetCurSel()==0){//3
		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( x>=2 && x<=4 && y>=2 && y<=4){
					M3.at(x+y*7)->EnableWindow();
				}else{
					M3.at(x+y*7)->EnableWindow(false);
				}
			}
		};
	};
	if(M3Size.GetCurSel()==1){//5
		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( x>=1 && x<=5 && y>=1 && y<=5){
					M3.at(x+y*7)->EnableWindow();
				}else{
					M3.at(x+y*7)->EnableWindow(false);
				}
			}
		};
	};
	if(M3Size.GetCurSel()==2){//7
		for(int y=0;y<7;y++)
			for(int x=0;x<7;x++)
				M3.at(x+y*7)->EnableWindow();
	};
	if(M3Size.GetCurSel()==3){//disable
		for(int y=0;y<7;y++)
			for(int x=0;x<7;x++)
				M3.at(x+y*7)->EnableWindow(false);
	}
}

void Maskview::OnBnClickedButton3()
{
	int sel = FilterSelect.GetCurSel();

	if (sel>=0){		
		int divisor = pMaskPalette->at(sel)->divisor;
		SetDlgItemInt(IDC_Divisor3, divisor);
		
		int bais = pMaskPalette->at(sel)->bais;
		SetDlgItemInt(IDC_BAIS3, bais);
		
		int size = pMaskPalette->at(sel)->mask.size();
		if (size==3)
			M3Size.SetCurSel(0);
		if (size==5)
			M3Size.SetCurSel(1);
		if (size==7)
			M3Size.SetCurSel(2);
		M3Size.UpdateData();
		OnLbnSelchangeList6(); //upadeSize;

		//Заполнить поле
		int startX=7/2-size/2;
		int startY=7/2-size/2;
		int endX=7/2+size/2;
		int endY=7/2+size/2;

		for(int y=0;y<7;y++){
			for(int x=0;x<7;x++){
				if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
					CString z;
					int _x = x-startX;
					int _y = y-startY;
					
					int val = pMaskPalette->at(sel)->mask.at(_x).at(_y);
					z.Format("%d", val);
					M3.at(x+y*7)->SetWindowTextA(z);
				}else{
					M3.at(x+y*7)->SetWindowText("");
				}
			}
		};
	};
}

void Maskview::OnBnClickedOk()
{
	int sel = CPS.GetCurSel();
	if (sel>=0)
		CPS.GetText(sel, *pColorModel);

	pM1->bais = GetDlgItemInt(IDC_BAIS1);
	pM2->bais = GetDlgItemInt(IDC_BAIS2);
	pM3->bais = GetDlgItemInt(IDC_BAIS3);

	pM1->divisor = GetDlgItemInt(IDC_Divisor1);
	pM2->divisor = GetDlgItemInt(IDC_Divisor2);
	pM3->divisor = GetDlgItemInt(IDC_Divisor3);

	sel=M1Size.GetCurSel();
	int size=0;
	if(sel==0)
		size=3;
	if(sel==1)
		size=5;
	if(sel==2)
		size=7;
	pM1->resize(size);


	int startX=7/2-size/2;
	int startY=7/2-size/2;
	int endX=7/2+size/2;
	int endY=7/2+size/2;

	for(int y=0;y<7;y++){
		for(int x=0;x<7;x++){
			if (size==0)break;
			if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
				CString z;
				int _x = x-startX;
				int _y = y-startY;				

				int val = GetDlgItemInt( M1.at(x+y*7)->GetDlgCtrlID());
				pM1->mask.at(_x).at(_y) = val;
			}
		}
	};

	sel=M2Size.GetCurSel();
	size=0;
	if(sel==0)
		size=3;
	if(sel==1)
		size=5;
	if(sel==2)
		size=7;
	pM2->resize(size);

	startX=7/2-size/2;
	startY=7/2-size/2;
	endX=7/2+size/2;
	endY=7/2+size/2;

	for(int y=0;y<7;y++){
		for(int x=0;x<7;x++){
			if (size==0)break;
			if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
				CString z;
				int _x = x-startX;
				int _y = y-startY;				

				int val = GetDlgItemInt( M2.at(x+y*7)->GetDlgCtrlID());
				pM2->mask.at(_x).at(_y) = val;
			}
		}
	};


	sel=M3Size.GetCurSel();
	size=0;
	if(sel==0)
		size=3;
	if(sel==1)
		size=5;
	if(sel==2)
		size=7;
	pM3->resize(size);

	startX=7/2-size/2;
	startY=7/2-size/2;
	endX=7/2+size/2;
	endY=7/2+size/2;

	for(int y=0;y<7;y++){
		for(int x=0;x<7;x++){
			if (size==0)break;
			if( (x>=startX && x<=endX) && (y>=startY && y<=endY)){
				CString z;
				int _x = x-startX;
				int _y = y-startY;				

				int val = GetDlgItemInt( M3.at(x+y*7)->GetDlgCtrlID());
				pM3->mask.at(_x).at(_y) = val;
			}
		}
	};

	OnOK();
}