// metric_wavelet.h : main header file for the metric_wavelet DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "math.h"		// main symbols


// Cmetric_waveletApp
// See metric_wavelet.cpp for the implementation of this class
//

#define PLUGIN_API __declspec(dllexport)


typedef vector<double> vec;
typedef vector<vec> multivec;

typedef vector<int> vecI;
typedef vector<vecI> multivecI;


class Cmetric_waveletApp : public CWinApp
{
public:
	Cmetric_waveletApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


using namespace CBIR;

class Wavelet2D;

	class Wavelet: public ImageIndex{
	public:
		Wavelet(CxImage *bmp);
		~Wavelet(){if(wavelet!=NULL)delete wavelet;};
		Wavelet2D *wavelet;
	};


	class WaveletMetric : public virtual MetricPlugin{
		friend class ImageInputPin;
		friend class LOutPin;
	private:
		double compare;

		class MetricData : public CBIR::ImageIndex{
		public:
			multivecI data;
			~MetricData(){data.clear();};
		};

	public:
		Counter Next();
		WaveletMetric(Core *pCore);
		CString	GetName(){return CString("Wavelet Color");};
		CString	GetDescription(){return CString("Wavelet color (RGBHSV)");};
		void ShowPropertiesDilog();
		UINT DoModal();
		ImageIndex* GetIndex(CxImage *bitmap);
		double Compare(ImageIndex* index1, ImageIndex* index2);

		~WaveletMetric(){};
	};


	class LOutPin: public LogicOutPin{
	public:
		LOutPin(Plugin* plug):LogicOutPin(plug){};
		Counter QueryNew(){return m_plug->Next();};
		~LOutPin(){};
		Logic* Get(bool copy =false);
		bool isWorking(){
			return m_plug->IsWorking;
		};
	};

	double logbase(int a, int base){
		return log((double)a) / log((double)base);
	};


	class Wavelet2D{
	public:
		Wavelet2D(const multivec &data);
		~Wavelet2D(){};

		int m_totalsize;
		multivec dat;
		double val;
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



class ColorHandler 
{
   // Handle conversions between RGB and HSV    
   // (and Color types, as well).
public:
   struct _RGB
   {
      // All values are between 0 and 255.
      int Red;
      int Green;
      int Blue;

      _RGB(int R, int G, int B) 
      {
         Red = R;
         Green = G;
         Blue = B;
      }

  };

   struct HSV
   {
      // All values are between 0 and 255.
      int Hue;
      int Saturation;
      int value;

      HSV(int H, int S, int V) 
      {
         Hue = H;
         Saturation = S;
         value = V;
      }
   };

   static _RGB HSVtoRGB(int H, int S, int V) 
   {
      // H, S, and V must all be between 0 and 255.
      return HSVtoRGB(HSV(H, S, V));
   }

   static _RGB HSVtoRGB(HSV& hsv) 
   {
      // HSV contains values scaled as in the color wheel:
      // that is, all from 0 to 255. 

      // for ( this code to work, HSV.Hue needs
      // to be scaled from 0 to 360 (it//s the angle of the selected
      // point within the circle). HSV.Saturation and HSV.value must be 
      // scaled to be between 0 and 1.

      double h;
      double s;
      double v;

      double r = 0;
      double g = 0;
      double b = 0;

      // Scale Hue to be between 0 and 360. Saturation
      // and value scale to be between 0 and 1.
      h = (double)((int)((double)hsv.Hue / 255 * 360) % 360);
      s = (double) hsv.Saturation / 255;
      v = (double) hsv.value / 255;

      if ( s == 0 ) 
      {
         // If s is 0, all colors are the same.
         // This is some flavor of gray.
         r = v;
         g = v;
         b = v;
      } 
      else 
      {
         double p;
         double q;
         double t;

         double fractionalSector;
         int sectorNumber;
         double sectorPos;

         // The color wheel consists of 6 sectors.
         // Figure out which sector you//re in.
         sectorPos = h / 60;
         sectorNumber = (int)(floor(sectorPos));

         // get the fractional part of the sector.
         // That is, how many degrees into the sector
         // are you?
         fractionalSector = sectorPos - sectorNumber;

         // Calculate values for the three axes
         // of the color. 
         p = v * (1 - s);
         q = v * (1 - (s * fractionalSector));
         t = v * (1 - (s * (1 - fractionalSector)));

         // Assign the fractional colors to r, g, and b
         // based on the sector the angle is in.
         switch (sectorNumber) 
         {
         case 0:
            r = v;
            g = t;
            b = p;
            break;

         case 1:
            r = q;
            g = v;
            b = p;
            break;

         case 2:
            r = p;
            g = v;
            b = t;
            break;

         case 3:
            r = p;
            g = q;
            b = v;
            break;

         case 4:
            r = t;
            g = p;
            b = v;
            break;

         case 5:
            r = v;
            g = p;
            b = q;
            break;
         }
      }
      // return an RGB structure, with values scaled
      // to be between 0 and 255.
      return _RGB((int)(r * 255), (int)(g * 255), (int)(b * 255));
   }

   static HSV RGBtoHSV(_RGB RGB) 
   {
      // In this function, R, G, and B values must be scaled 
      // to be between 0 and 1.
      // HSV.Hue will be a value between 0 and 360, and 
      // HSV.Saturation and value are between 0 and 1.
      // The code must scale these to be between 0 and 255 for
      // the purposes of this application.

      double min;
      double max;
      double delta;

      double r = (double) RGB.Red / 255;
      double g = (double) RGB.Green / 255;
      double b = (double) RGB.Blue / 255;

      double h;
      double s;
      double v;

      min = min(min(r, g), b);
      max = max(max(r, g), b);
      v = max;
      delta = max - min;
      if ( max == 0 || delta == 0 ) 
      {
         // R, G, and B must be 0, or all the same.
         // In this case, S is 0, and H is undefined.
         // Using H = 0 is as good as any...
         s = 0;
         h = 0;
      } 
      else 
      {
         s = delta / max;
         if ( r == max ) 
         {
            // Between Yellow and Magenta
            h = (g - b) / delta;
         } 
         else if ( g == max ) 
         {
            // Between Cyan and Yellow
            h = 2 + (b - r) / delta;
         } 
         else 
         {
            // Between Magenta and Cyan
            h = 4 + (r - g) / delta;
         }

      }
      // Scale h to be between 0 and 360. 
      // This may require adding 360, if the value
      // is negative.
      h *= 60;
      if ( h < 0 ) 
      {
         h += 360;
      }

      // Scale to the requirements of this 
      // application. All values are between 0 and 255.
      return HSV((int)(h / 360 * 255), (int)(s * 255), (int)(v * 255));
   };
};