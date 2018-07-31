// index_wavelet.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "index_wavelet.h"
#include <boost/lexical_cast.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(Cindex_waveletApp, CWinApp)
END_MESSAGE_MAP()


// Cindex_waveletApp construction

Cindex_waveletApp::Cindex_waveletApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cindex_waveletApp object

Cindex_waveletApp theApp;


// Cindex_waveletApp initialization

BOOL Cindex_waveletApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isFilterPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
	WaveltExtractor *pIG = new WaveltExtractor(pCore);
	return pIG;
}

WaveltExtractor::WaveltExtractor(Core *pCore): Plugin(pCore){	

	pluginType = CORE_PLG_FILTER;
	name = "Wavelet Index";
	description = "Wavelet Index";

	outPins.push_back(new SimpleOutImagePin<WaveltExtractor>(this));
	InPin *pPin = new ImageInputPin(this);
	inPins.push_back(pPin);

}

WaveltExtractor::~WaveltExtractor(){

};
UINT WaveltExtractor::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return MB_OK;
};
Counter WaveltExtractor::Next(){
	StartTiming();	
	ImageInputPin *pin;
	pin = (ImageInputPin *) (inPins[0]);
	if (!pin->GetLink()){
		AfxMessageBox ("Connect all pins!");
		return Counter();
	};

	pCore->AddPinRequest(this, pin);
	pCore->RunPinRequest(this);
	Counter count;	

	image = boost::shared_dynamic_cast<Image>(pin->Get());

	//create result Image	
	tempBMP = PBitmap(new CxImage(*image->pBitmap));

	image->pBitmap =  tempBMP;

	Wavelet wav (tempBMP.get());

	try{
		string fname = boost::lexical_cast<string>(pin->counter.count);
		ofstream fs(fname.c_str());
	}catch(...){
		
	};

	EndTiming();	
	return pin->counter;
};


Wavelet2D::Wavelet2D(const multivec &data){
	m_totalsize= logbase(data.size(),2);
	if(m_totalsize<=1)
		throw "Too Small!";

	dat.assign(data.begin(), data.end());

	int fromX=0;
	int fromY=0;
	int toX=0;
	int toY=0;
	int counter=0;

	int step=m_totalsize-1;
	int stepx=step;
	int stepy=step;
	int steps=2*step+1;

	for(;step>=0;){
		if (stepx<step && stepy<step && stepx==stepy)
			step--;
		if(steps%2==1){
			toX=pow((double)2, (double)step);
			toY=pow((double)2, (double)step+1);
			for(int y=0;y<toY;y++){
				counter=0;
				vector<double> tmpVec;
				tmpVec.resize(toX*2);

				for(int x=0;x<toX;x++){
					double median = (double)(dat[y][x*2] + dat[y][x*2 +1]) /2;
					double kof = (double)(dat[y][x*2] - dat[y][x*2 +1]) /2;
					tmpVec[toX+counter] = kof*(1/(sqrt(pow((double)2, step))));//normalize
					tmpVec[x]=median;
					counter++;
				}
				for(int i=0;i<toX*2;i++){
					dat[y][i]=tmpVec[i];
				}
				tmpVec.clear();
			}
			stepx--;
		}else{
			counter=0;
			toX=pow((double)2, (double)step);
			toY=pow((double)2, (double)step);
			vector<double> tmpVec;
			tmpVec.resize(toY*2);

			for(int x=0;x<toX;x++){
				counter=0;
				for(int y=0;y<toY;y++){       
					double median = (double)(dat[y*2][x] + dat[y*2+1][x]) /2;
					double kof = (double)(dat[y*2][x] - dat[y*2+1][x]) /2;
					tmpVec[toY+counter] = kof*(1/(sqrt(pow((double)2, step))));//normalize
					tmpVec[y]=median;
					counter++;
				}
				for(int i=0;i<toY*2;i++){
					dat[i][x] = tmpVec[i];
				}
			}
			tmpVec.clear();
			stepy--;
		};
		steps--;
	};
	val=dat[0][0];
};

Wavelet::Wavelet(CxImage *bmp){
	//get image
	int h = bmp->GetWidth();

	int size = 4;
	multivec myVec;
	myVec.resize(h);
	for(multivec::iterator it = myVec.begin(); it!= myVec.end();it++){
		(*it).resize(h);
	};
	ColorHandler myCol;
	for(int y=0;y<h;y++){
		for(int x=0;x<h;x++){
			RGBQUAD col;
			col = bmp->GetPixelColor(x,y, false);
			ColorHandler::HSV myHSV = myCol.RGBtoHSV(ColorHandler::_RGB(col.rgbRed, col.rgbGreen,col.rgbBlue));
			//myVec[y][x]=myHSV.Hue;
			myVec[y][x]=myHSV.value;
		};
	}
	waveletV = boost::shared_ptr<Wavelet2D>(new Wavelet2D(myVec));

	for(int y=0;y<h;y++){
		for(int x=0;x<h;x++){
			RGBQUAD col;
			col = bmp->GetPixelColor(x,y, false);
			ColorHandler::HSV myHSV = myCol.RGBtoHSV(ColorHandler::_RGB(col.rgbRed, col.rgbGreen,col.rgbBlue));
			myVec[y][x]=myHSV.Hue;
		};
	}
	waveletH = boost::shared_ptr<Wavelet2D>(new Wavelet2D(myVec));

	myVec.clear();
};