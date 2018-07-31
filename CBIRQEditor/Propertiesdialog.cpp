
// Propertiesdialog.cpp : implementation file
//


#include "stdafx.h"
#include "CBIRQEditor.h"
#include "Propertiesdialog.h"
#include "TimingDlg.h"
#include "TGD.h"
#include <windows.h>

// Propertiesdialog dialog
// using undocumented functions and structures

#define SystemBasicInformation		0
#define	SystemPerformanceInformation	2
#define SystemTimeInformation		3

IMPLEMENT_DYNAMIC(Propertiesdialog, CDialog)

Propertiesdialog::Propertiesdialog(CCBIRQEditorDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(Propertiesdialog::IDD, pParent)
{
	this->pDoc = pDoc;
	pOldElement=NULL;
	for (int i=0;i<=173;i++)
		_CPULoad.push_back(0);
}


BOOL Propertiesdialog::OnInitDialog(){
	BOOL z= CDialog::OnInitDialog();
	timerE = SetTimer(1,100, 0);
	CPULoadBitmap.Create(170,50,24);

	//creating InPins List;
	InPinCtrl.InsertColumn(0,"No.", 0,35); 
	InPinCtrl.InsertColumn(1,"Type", 0,60); 
	InPinCtrl.InsertColumn(2,"Status", 0,70); 

	OutPinCtrl.InsertColumn(0,"No.", 0,35); 
	OutPinCtrl.InsertColumn(1,"Type", 0,60); 
	return z;
}
int Propertiesdialog::OnDestroy(){	
	CDialog::OnDestroy();
	return 0;
}
void Propertiesdialog::FillView(CCBIRQElement *elem){	
	currentModuleName = CString(elem->plugin->GetName().c_str());;
	SetDlgItemText(IDC_DESCEDIT, elem->description);

	int t = 1000*elem->plugin->GetTime();
	char str[20];
	itoa(t, str, 10);

	currentModuleDuration = CString(str) + CString("ms");
	
	InPinCtrl.DeleteAllItems();
	int counter = 0;
	for(vector<InPin*>::iterator it = elem->plugin->inPins.begin();
		it!=elem->plugin->inPins.end(); it++){
		counter++;
		char s[50];
		itoa(counter,s,10);
		int item = InPinCtrl.InsertItem(0,s);
		
		CString type;
		switch((*it)->GetDataType()){
			case CORE_DATA_IMAGE:
				type = "Image";
			break;
			case CORE_DATA_LOGIC:
				type = "Logic";
			break;
			case CORE_DATA_METRIC:
				type = "Metric";
			break;
			default:
				type = "Data";
		};
		InPinCtrl.SetItem(item, 1,  LVIF_TEXT, type, 1, 0,0,0);

		CString status ("Linked");
		if(!(*it)->GetLink())
			status ="Unlinked";
		InPinCtrl.SetItem(item, 2,  LVIF_TEXT, status, 1, 0,0,0);
	};

	OutPinCtrl.DeleteAllItems();
	counter = 0;
	for(vector<OutPin*>::iterator it = elem->plugin->outPins.begin();
		it!=elem->plugin->outPins.end(); it++){
		counter++;
		char s[50];
		itoa(counter,s,10);
		int item = OutPinCtrl.InsertItem(0,s);
		
		CString type;
		switch((*it)->GetDataType()){
			case CORE_DATA_IMAGE:
				type = "Image";
			break;
			case CORE_DATA_LOGIC:
				type = "Logic";
			break;
			case CORE_DATA_METRIC:
				type = "Metric";
			break;
			default:
				type = "Data";
		};
		OutPinCtrl.SetItem(item, 1,  LVIF_TEXT, type, 1, 0,0,0);		
	};

	Invalidate(FALSE);
	//GetParent()->GetParent()->Invalidate();
	//InvalidateRect(CRect(0,0,300,100));
};

void Propertiesdialog::PaintLoad(){
	
	vector<int>::iterator begin,end;
	begin = _CPULoad.begin();
	end = _CPULoad.end();
	int x=0;
	CPULoadBitmap.Clear();
	for(vector<int>::iterator it = begin; it!=end;it++){
		RGBQUAD c;
		c.rgbBlue=0;
		c.rgbRed=0;
		c.rgbGreen=150;

		CPULoadBitmap.DrawLine(x,x,0,(*it)/2, c);
		x++;
	};
};
void Propertiesdialog::Renew(){

	for(int i=0;i<170-1;i++)
		_CPULoad[i]=_CPULoad[i+1];

	_CPULoad[169]=CpuUsage.GetCpuUsage();
	
	
	BOOL z =pDoc->IsKindOf(RUNTIME_CLASS(CCBIRQEditorDoc));
	if(!z)
		return;
	CCBIRElements* pElems= pDoc->CBIRElements;

	if (pElems== NULL)
		return;
	int element  = pElems->GetActive();
	if (element!=0){
		CCBIRQElement* pElem = pElems->GetByID(element);
		if(pElem!=NULL){
			//compare wthi previos
			//if equal then do nothing
			//else - init controls for it
			if ((pElem!=pOldElement)){
				FillView(pElem);
				pOldElement = pElem;
			}
		}
	}
	
}
Propertiesdialog::~Propertiesdialog()
{

}

void Propertiesdialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DESCEDIT, DescCtrl);
	DDX_Control(pDX, IDC_INLIST, InPinCtrl);
	DDX_Control(pDX, IDC_OUTLIST, OutPinCtrl);
}

void Propertiesdialog::OnPaint(){	
	CDialog::OnPaint();
	DescCtrl.Invalidate();
	
	CFont font;
	VERIFY(font.CreateFont(
	   16,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_BOLD,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   "Arial"));                 // lpszFacename


	// Do something with the font just created...
	CClientDC dc(this);  

	CFont* def_font = dc.SelectObject(&font);
	dc.SetBkColor(RGB(212,208,200));

	dc.TextOut(10,25,currentModuleName);
	dc.TextOut(155,45,currentModuleDuration);

	dc.SelectObject(def_font);
	// Done with the font.  Delete the font object.
	font.DeleteObject(); 

	//PaintLoad();	

	int minX = 10;
	int maxX = 180;
	int minY = 430;
	int maxY = minY+50;

	PaintLoad();	
	CPULoadBitmap.Draw(dc, minX, minY);
}
BEGIN_MESSAGE_MAP(Propertiesdialog, CDialog)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_DESCEDIT, &Propertiesdialog::OnEnChangeDescedit)
	ON_BN_CLICKED(IDC_BUTTON1, &Propertiesdialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Propertiesdialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Propertiesdialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_ADDNEWELEMENTBTN, &Propertiesdialog::OnBnClickedAddnewelementbtn)
	ON_BN_CLICKED(IDC_BUTTON4, &Propertiesdialog::OnBnClickedButton4)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNTIMING, &Propertiesdialog::OnBnClickedBtntiming)
END_MESSAGE_MAP()


// Propertiesdialog message handlers

void Propertiesdialog::OnEnChangeDescedit()
{
}

void Propertiesdialog::OnBnClickedButton1()
{
	if (pOldElement!=NULL){
		char str[200];
		GetDlgItemText(IDC_DESCEDIT, str, 30);
		pOldElement->description = CString(str);
		GetParent()->GetParent()->Invalidate();
	}
}

void Propertiesdialog::OnBnClickedButton2()
{
	if (pOldElement!=NULL)
		pOldElement->plugin->DoModal();
}

void Propertiesdialog::OnBnClickedButton3()
{
	if (pOldElement!=NULL)
		pOldElement->plugin->Next();
	
}

void Propertiesdialog::OnBnClickedAddnewelementbtn()
{
	pDoc->CBIRElements->addDialog(CPoint(200,200));
	GetParent()->GetParent()->Invalidate();
}

void Propertiesdialog::OnBnClickedButton4()
{
	OnKeyDown(VK_SUBTRACT,0,0);
}

void Propertiesdialog::OnTimer(UINT_PTR nIDEvent)
{

	Renew();
	Invalidate(0);
	CDialog::OnTimer(nIDEvent);
}

void Propertiesdialog::OnBnClickedBtntiming()
{
	if (pOldElement!=NULL){
		try{
			TGD dlg(pOldElement->plugin);
			dlg.DoModal();
		}catch(const char *ex){
			AfxMessageBox("No Data");
		}
	}
}
