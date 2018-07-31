// TGD.cpp : implementation file
//

#include "stdafx.h"
#include "CBIRQEditor.h"
#include "TGD.h"
#include "resource.h"

#include <algorithm>
#include <numeric>
#include <functional>





// TGD dialog

IMPLEMENT_DYNAMIC(TGD, CDialog)

TGD::TGD(Plugin *pPlug, CWnd* pParent /*=NULL*/)
	: CDialog(TGD::IDD, pParent)
{
	view_start=0;
	view_finish=1000;

	pPlugin = pPlug;
	mosuseX=10;
	maxval = 1000;
	view_y = 100;

	boost::mutex::scoped_lock lock (pPlugin->timeMutex);
	timings.assign(pPlugin->times.begin(), pPlugin->times.end());
	lock.unlock();

	median = std::accumulate (timings.begin(), timings.end(), 0.0) / timings.size();
	
	if (timings.empty())
		throw "No dada";
	sort(timings.begin(), timings.end());

	maxVal = timings.back();
	minVal = timings.front();
	view_finish = maxVal;
	view_start = minVal;

	if (view_finish<view_start+5)
		view_finish=view_start+5;

	Prepare();

}

void TGD::Prepare(){
	boost::mutex::scoped_lock lock (pPlugin->timeMutex);
	timings.assign(pPlugin->times.begin(), pPlugin->times.end());
	lock.unlock();

	int maxX=350;
	int maxY=130;

	Bitmap = PBitmap(new CxImage());
	Bitmap->Create(maxX, maxY, 24);

	if (timings.empty())
		return;

	median = std::accumulate(timings.begin(), timings.end(), 0.0)/ timings.size();

	sort(timings.begin(), timings.end());
	
	maxVal = timings.back();
	minVal = timings.front();

	
/*  //normalize
	for(deque<double>::iterator it = timings.begin();it!=timings.end();it++)
		*it/=maxVal;
*/
	counts.resize(maxX);
	fill(counts.begin(), counts.end(), 0);

	//ferquency 
	for(deque<double>::iterator it = timings.begin();it!=timings.end();it++){
		//clip
		if (*it < view_start)
			continue;
		if (*it >view_finish)
			continue;
		double diff = view_finish-view_start;
		double pos = (*it - view_start);
		int nVal = (double)maxX* (pos/diff);
		if (nVal>=maxX)
			nVal=maxX-1;
		counts[nVal]++;
	}
	//normalize
	vector<int>::iterator maxIt = std::max_element(counts.begin(), counts.end());
	mulK = (double)maxY/(*maxIt);

	for(vector<int>::iterator it = counts.begin();it!=counts.end();it++){
		*it= (mulK* *it);
	};
//paint
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

	/*
	for(deque<double>::iterator it = timings.begin();it!=timings.end();it++){
		unsigned res = 0;
		//clip front and back
		if (*it
		= (maxX* *it);
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
*/
}
TGD::~TGD()
{
}

void TGD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TGD, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON5, &TGD::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &TGD::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TGD::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &TGD::OnBnClickedButton3)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// TGD message handlers

void TGD::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((point.x>=20) && (point.x<Bitmap->GetWidth()+20)){
		double pixSize = ((double)view_finish-view_start)/ Bitmap->GetWidth();

		curval = (double)(point.x -20+1) / (Bitmap->GetWidth());
		curval*= view_finish;
		curval = view_start+ pixSize* (point.x -20+1) ;//* (double)(point.x -20+1);
		curStr = boost::lexical_cast<string>(curval);
		if (curStr.size()>6)
			curStr.resize(6);
		curStr+=" ms";
		mosuseX = point.x;
		//Invalidate();
		InvalidateRect(CRect(0,Bitmap->GetHeight()+10,600,Bitmap->GetHeight()+40));
	}

	CDialog::OnMouseMove(nFlags, point);
}
string GetmsStr(double &dbl){
	string st = boost::lexical_cast<string>(dbl);
	if (st.size()>6)
		st.resize(6);
	st+= " ms";
	return st;
}
void TGD::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.SetBkColor(RGB(212,208,200));

	Bitmap->Draw(dc, 20, 10);
	//draw tringle
	int x = mosuseX+20;
	int y = Bitmap->GetHeight()+10;

	dc.MoveTo(x-20, y);
	dc.LineTo(x+10-20,y+10);
	dc.LineTo(x-10-20,y+10);
	dc.LineTo(x-20,y);
	dc.TextOut(x-10-20,y+16, curStr.c_str(), curStr.size());
 
	string st = GetmsStr(view_finish) ;
	dc.TextOut(25+Bitmap->GetWidth(),10+Bitmap->GetHeight(), st.c_str(), st.size());

	st = GetmsStr(view_start);
	dc.TextOut(25,10+Bitmap->GetHeight(), st.c_str(), st.size());
	
	st = GetmsStr(minVal);
	st ="min: "+st;
	dc.TextOut(25+Bitmap->GetWidth(),30, st.c_str(), st.size());

	st = GetmsStr(maxVal);
	st ="max: "+st;
	dc.TextOut(25+Bitmap->GetWidth(),50, st.c_str(), st.size());

	st = GetmsStr(median);
	st ="M: "+st;
	dc.TextOut(25+Bitmap->GetWidth(),70, st.c_str(), st.size());


}
void TGD::OnBnClickedButton5()
{
	double diff = view_finish-view_start;
	view_finish += diff/3;

	Prepare();
	Invalidate();
}

void TGD::OnBnClickedButton1()
{

	double diff = view_finish-view_start;
	view_finish -= diff/3;
	if (view_finish<= view_start+2)
		view_finish = view_start+2;
	
	Prepare();
	Invalidate();
}

void TGD::OnBnClickedButton2()
{
	double diff = view_finish-view_start;
	if (view_start-diff/6 < 0){
		view_start = 0;
		view_finish=diff;
	}else{
		view_start-=diff/6;
		view_finish-=diff/6;
	}

	Prepare();
	Invalidate();
}

void TGD::OnBnClickedButton3()
{
	
	double diff = view_finish-view_start;
	view_start+=diff/6;
	view_finish+=diff/6;

	Prepare();
	Invalidate();
}

void TGD::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	Prepare();
	InvalidateRect(CRect(0,0,350,140));
	CDialog::OnTimer(nIDEvent);
}
