#pragma once

#include "../../CxImage/CxImage/xImage.h"
#include <string>
#include <boost/timer.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

using namespace std;

class Image;
class CoreData;
class Measure;

typedef boost::shared_ptr<CxImage> PBitmap;
typedef boost::shared_ptr<Image> PImage;
typedef boost::shared_ptr<Measure> PMeasure;
typedef boost::shared_ptr<CoreData> PData;



enum CORE_DATA_TYPE {
  CORE_DATA_METRIC=1,
  CORE_DATA_LOGIC=2,
  CORE_DATA_IMAGE=3,
  CORE_DATA_MEASURE=4,
  CORE_DATA_NONE=5,
};

class CoreData{
public:
	boost::timer timer;
	CORE_DATA_TYPE DATATYPE;
	virtual CORE_DATA_TYPE GetDataType(){return DATATYPE;};
};

class Measure: public CoreData{
public:
	Measure(){timer.restart();};
	vector<double> values;
};
class Image: public CoreData{
public:
	string path;	
	PBitmap pBitmap;

	Image::Image()
	{
		timer.restart();
		DATATYPE = CORE_DATA_IMAGE;
	};

	Image::Image(const CxImage &bitmap)
	{
		DATATYPE = CORE_DATA_IMAGE;
		timer.restart();
		pBitmap = PBitmap(new CxImage(bitmap, true, false,false));
	};

	Image(Image &img){
		DATATYPE = CORE_DATA_IMAGE;
		timer.restart();
		pBitmap = PBitmap(new CxImage(*(img.pBitmap.get()), true, false,false));
		path= img.path;
		timer=img.timer;
	};
	~Image(void);
};

 

class Logic: public CoreData{
public:
	Logic(){
		DATATYPE = CORE_DATA_LOGIC;
	};
	~Logic(){};
	double l;
	string path;
	bool operator> (const Logic& log){
		return log.l<l;
	};
	bool operator< (const Logic& log){
		return log.l>l;
	};
	bool operator>= (const Logic& log){
		return log.l<=l;
	};
	bool operator<= (const Logic& log){
		return log.l>=l;
	};
	bool operator== (const Logic& log){
		return log.l==l;
	};
};