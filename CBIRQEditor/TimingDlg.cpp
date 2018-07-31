// TimingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "CBIRQEditor.h"
#include "TimingDlg.h"
#include <boost/lexical_cast.hpp>


// TimingDlg dialog

IMPLEMENT_DYNAMIC(TimingDlg, CDialog)

TimingDlg::TimingDlg(Plugin *pPlug, CWnd* pParent /*=NULL*/)
	: CDialog(TimingDlg::IDD, pParent)
{
	pPlugin = pPlug;
	mosuseX=10;
	boost::mutex::scoped_lock lock (pPlugin->timeMutex);
	timings.assign(pPlugin->times.begin(), pPlugin->times.end());
	lock.unlock();

	int maxX=350;
	int maxY=130;

	Bitmap = PBitmap(new CxImage());
	Bitmap->Create(maxX, maxY, 24);

	if (timings.empty())
		return;

	sort(timings.begin(), timings.end());
	maxval = timings.back();
	for(deque<double>::iterator it = timings.begin();it!=timings.end();it++){
		*it/=maxval;
	}
	counts.resize(maxX);
	fill(counts.begin(), counts.end(), 0);

	for(deque<double>::iterator it = timings.begin();it!=timings.end();it++){
		unsigned res = (maxX* *it);
		if (res>=maxX)
			res =maxX-1; 
		counts[res]++;
	};
	vector<int>::iterator maxIt = std::max_element(counts.begin(), counts.end());
	mulK = (double)maxY/(*maxIt);

	for(vector<int>::iterator it = counts.begin();it!=counts.end();it++){
		*it= (mulK* *it);
	};
	RGBQUAD colBlack, colWhite;
	colBlack.rgbBlue=0;
	colBlack.rgbRed=0;
	colBlack.rgbGreen=0;

	colWhite.rgbBlue=255;
	colWhite.rgbRed=255;
	colWhite.rgbGreen=255;

	int i=0;
	for(vector<int>::iterator it = counts.begin();it!=counts.end();it++){
		Bitmap->DrawLine(i,i,0, Bitmap->GetHeight()-1, colWhite);
		Bitmap->DrawLine(i,i,0, *it, colBlack);
		
		i++;
		if (i>Bitmap->GetWidth())
			break;
	};
}

TimingDlg::~TimingDlg()
{
}
void TimingDlg::OnMouseMove(UINT nFlags, CPoint point){
	if ((point.x>=20) && (point.x<Bitmap->GetWidth()+20)){
		curval = (double)(point.x -20+1) / (Bitmap->GetWidth());
		curval*= maxval;
		curStr = boost::lexical_cast<string>(curval);
		curStr.resize(6);
		curStr+=" ms";
		//SetDlgItemText(IDC_STIMESTAT, st.c_str());
		mosuseX = point.x;
		Invalidate();
	}
}
void TimingDlg::OnPaint(){	
	CDialog::OnPaint();
/*
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
*/
	// Do something with the font just created...
	CClientDC dc(this);  

//	CFont* def_font = dc.SelectObject(&font);
	dc.SetBkColor(RGB(212,208,200));
	
	//dc.TextOut(10,25,currentModuleName);
	//dc.TextOut(155,45,currentModuleDuration);
	Bitmap->Draw(dc, 20, 10);
	//draw tringle
	int x = mosuseX+20;
	int y = Bitmap->GetHeight()+10;

	dc.MoveTo(x-20, y);
	dc.LineTo(x+10-20,y+10);
	dc.LineTo(x-10-20,y+10);
	dc.LineTo(x-20,y);
	dc.TextOut(x-30-20,y+16, curStr.c_str(), curStr.size());

	//dc.SelectObject(def_font);
	// Done with the font.  Delete the font object.
	//font.DeleteObject(); 
	string st = boost::lexical_cast<string>(maxval);
	st.resize(6);
	st+= " ms";

	dc.TextOut(25+Bitmap->GetWidth(),10+Bitmap->GetHeight(), st.c_str(), st.size());

	return ;
}

void TimingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TimingDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// TimingDlg message handlers
