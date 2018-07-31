// StereoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filter_stereo.h"
#include "StereoDlg.h"


// StereoDlg dialog

IMPLEMENT_DYNAMIC(StereoDlg, CDialog)

StereoDlg::StereoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(StereoDlg::IDD, pParent)
{
	pImg1=NULL;
	pImg2=NULL;

}

StereoDlg::~StereoDlg()
{
}

void StereoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, SLD);
}


BEGIN_MESSAGE_MAP(StereoDlg, CDialog)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &StereoDlg::OnNMReleasedcaptureSlider1)
	ON_BN_CLICKED(IDC_BUTTON1, &StereoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// StereoDlg message handlers

void StereoDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
class Wavelet1D{
public:
	Wavelet1D(const vector<double> &data);
	int m_totalsize;
	vector<double> wavelet;
	static void restore(const vector<double> &src, vector<double> &dest, int step);
	static vector<double> CalculateMove(const vector<double> &data1, const vector<double> &data2, int step);
	static void fillStart(const vector<double> &src, vector<double> &dest, int step);
	static void ApplayMove(vector<double> &data,  const vector<double> &move, int step, double koeff);
	double val;
};
double logbase(int a, int base){
	return log((double)a) / log((double)base);
};

Wavelet1D::Wavelet1D(const vector<double> &data){
	m_totalsize= logbase(data.size(),2);
	if(m_totalsize<=1)
		throw "Too Small!";

	wavelet.assign(data.begin(), data.end());

	int toX=0;
	int counter=0;

	int step=m_totalsize-1;
	//int steps=2*step+1;

	for(;step>=0;){
		toX=pow((double)2, (double)step);
		vector<double> tmpVec;
		tmpVec.resize(toX*2);
		counter=0;

		for(int x=0;x<toX;x++){
			double median = (double)(wavelet[x*2] + wavelet[x*2 +1]) /2;
			double kof = median - wavelet[x*2];
			tmpVec[toX+counter] = kof;/**(1/(sqrt(pow((double)2, step))));//normalize*/
			tmpVec[x]=median;
			counter++;
		}
		for(int i=0;i<toX*2;i++)
			wavelet[i]=tmpVec[i];
		tmpVec.clear();

		step--;
	}
	val=wavelet[0];
};

void Wavelet1D::restore(const vector<double> &src, vector<double> &dest, int step){
	int totalsize= logbase(src.size(),2);

	int startX = 0;
	int endX = pow((double)2, step);

	vector<double> tmpVec;
	//tmpVec.resize(endX*2);

	int counter =0;
	for(int x=0; x<endX;x++){
		double val1 = src[x]-src[endX+x];
		double val2 = src[x]+src[endX+x];
		tmpVec.push_back(val1);
		tmpVec.push_back(val2);
	}

	for(int i=0;i<endX*2;i++)
		dest[i]=tmpVec[i];
}

vector<double> Wavelet1D::CalculateMove(const vector<double> &data1, const vector<double> &data2, int step){
	vector<double> moveData;
	int startX = 0;
	int endX = pow((double)2, step);

	for(int x=startX;x<endX;x++){
		double x1 = data1[x];
		double x2 = data2[x];
		double a1=data1[endX+x];
		double a2=data2[endX+x];

		double newA =x1-x2+a1;
		double delataA = a2-newA;

		//moveData.push_back(delataA);
		moveData.push_back(delataA);
	}

	return moveData;
}
void Wavelet1D::fillStart(const vector<double> &src, vector<double> &dest, int step){
	int startX = pow((double)2, step);

	for(int x=startX;x<startX*2;x++)
		dest[x]=src[x];
}

void Wavelet1D::ApplayMove(vector<double> &data, const vector<double> &move, int step, double koeff){
	int startX = pow((double)2, step);
	int endX = pow((double)2, step+1);
	int counter = 0;
	for(int x=startX;x<endX;x++){
		//data[x] =(1-koeff)*data[x] + (koeff)*(data[x] - move[logbase(x,2)]);
		data[x] =(koeff)*data[x] + (1-koeff)*(data[x] - move[counter++]);
	}
	//1 склоняет к 1, 0 к 2, 0.5 - по середине;
}


void StereoDlg::OnBnClickedButton1()
{
	//run

	for(int y=y;y<=pImg2->GetHeight();y++){
		vector<double> im1, im2;

		pImg1->Colorize(0,0);
		pImg2->Colorize(0,0);
		for(int x=0;x<=128;x++){
			im1.push_back(pImg1->GetPixelColor(x,y).rgbBlue);
			im2.push_back(pImg1->GetPixelColor(x,y).rgbBlue);
		}
	}
}
