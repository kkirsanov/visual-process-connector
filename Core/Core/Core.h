#pragma once

//#define  BOOST_SERIALIZATION_DYN_LINK
//#define  BOOST_ALL_DYN_LINK
const int TIMEBUFFSIZE = 900;

#define PLUGIN_API __declspec(dllexport)

#pragma warning (disable: 4996)
#pragma warning (disable: 4018)
#pragma warning (disable: 4267)

#ifndef CORE
#define CORE

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <deque>

#include <afxwin.h>

#include "Core_Data.h"


#include <boost/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/random.hpp>

//#include <clocale>

class PluginEntry;

string GetCurTimeString();


typedef std::vector<PluginEntry*> PluginVector ;
typedef PluginVector::iterator PluginVectorIt;


class Core{
private:
	boost::mt11213b rn;
	boost::mutex randomMutex;
	boost::mutex workMapMutex;
public:
	int GetRandom(){
		boost::mutex::scoped_lock scoped_lock(randomMutex);
		return rand();//rn();
	}
	Core(string pluginPath=string("./"));
	void LogIt(string str);
	boost::mutex IsWritingMutex;
	
	map<Plugin*, vector<InPin*>>  WorkMap;
	
	ofstream Log;

	void AddPinRequest(Plugin* pPlg, InPin* pPin);
	void RunPinRequest(Plugin* pPlg);

	
	void AddModule(string path);

	//Counter QueryParalaelPin
	vector<PluginEntry*> plugins;

	~Core(){
		Log.close();
	};
};



class Plugin{
	
protected:
	string name;
	string description;
	CORE_PLUGIN_TYPE pluginType;
public:
	virtual void Stop(){std::cout << "Stopping " << name << endl;};
	boost::mutex mutex;
	boost::mutex timeMutex;

	bool IsWorking;
	Plugin(Core* pCore);
	string	GetName(){return name;};
	string	GetDescription(){return description;};

	virtual unsigned int DoModal(){return 0;};
	CORE_PLUGIN_TYPE	GetType(){return pluginType;};
	CORE_PLUGIN_TYPE	GetDataType(){return pluginType;};

	virtual ~Plugin(){};
	void Link(Plugin* plg);
	virtual pair<OutPin*, InPin*> Link(int pin, Plugin* plg, int remotePin);
	virtual Counter Next(){return Counter();};
	
	vector<InPin*> inPins;
	vector<OutPin*> outPins;

	double GetTime(){
		return perfDiffMs;
	};
	int group;
	int groupData;
	virtual void Serialize(CArchive& ar){};
	double perfDiffMs;

	deque<double> times;
private:
	boost::timer timer;
protected:
	

	LARGE_INTEGER freq;
	LARGE_INTEGER perfCntPrev;
	LARGE_INTEGER perfCntNew;

	double resultTime;
	void StartTiming(){
		IsWorking = true;
		timer.restart();
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&perfCntPrev);
		
	};
	void EndTiming(){
		IsWorking = false;
		resultTime = timer.elapsed();
		QueryPerformanceCounter(&perfCntNew);
		perfDiffMs = (perfCntNew.QuadPart - perfCntPrev.QuadPart) * 1000.0 / freq.QuadPart;
		boost::mutex::scoped_lock lock(timeMutex);
		times.push_back(perfDiffMs);
		if (times.size()>TIMEBUFFSIZE)
			times.pop_front();

	};
	Core* pCore;
};

typedef	Plugin*	(FUNCTION_pPlugin_pCore)	(Core*); //Для импорта п адресу

class PluginEntry{
public:
	PluginEntry(FUNCTION_pPlugin_pCore *ifu, Core* core){initf = ifu;pCore = core;};
	string name;
	CORE_PLUGIN_TYPE type;
	~PluginEntry();
	virtual string GetName(){return name;};
	virtual CORE_PLUGIN_TYPE GetType(){return type;};

	Plugin* CreatePlugin(){
		return (initf)(pCore); //return the reuslt of Init function from module
	};
private:		
	FUNCTION_pPlugin_pCore *initf;
	Core *pCore;
};

class OutPin: public Pin{

public:
	OutPin(Plugin *plug):Pin(plug){};
	virtual ~OutPin(){;};
	virtual CORE_DIRECTION GetDirection(){return CORE_DIRECTION_OUTPUT;};
	virtual CORE_DATA_TYPE GetDataType(){return CORE_DATA_NONE;};
	
	virtual Counter RequestData(int group = 1, InPin* pin = NULL){
		boost::mutex::scoped_lock lock(m_plug->mutex);
		return m_plug->Next();
	};
	virtual boost::shared_ptr<CoreData> Get(bool copy = false)=0;
	boost::shared_ptr<CoreData> data;
};

template <class T>
class SimpleOutImagePin: public OutPin{
public:
	queue<PImage> images;
	SimpleOutImagePin(Plugin* plug):OutPin(plug){
	};	
	virtual ~SimpleOutImagePin(){};
	virtual CORE_DATA_TYPE GetDataType(){return CORE_DATA_IMAGE;};
	//virtual void Next(){};
	
	virtual PData Get(bool copy = false){
		boost::mutex::scoped_lock lock((dynamic_cast<T*>(m_plug))->mutex);
		PImage p = PImage((dynamic_cast<T*>(m_plug))->image);
		
		images.push(p);
		if (images.size()>10)
			images.pop();

		if(copy)
			return p;
		
		return PImage(new Image(*p));
	};
};


class ImageInputPin: public InPin{	
public:
	ImageInputPin(Plugin* plug):InPin(plug){
	};	
	virtual CORE_DATA_TYPE GetDataType(){return CORE_DATA_IMAGE;};
	
	virtual boost::shared_ptr<CoreData> Get(bool copy = false){
		if (pLinkedPin){
			boost::shared_ptr<CoreData> z = pLinkedPin->Get(copy);
			counter =pLinkedPin->counter;
			return z;
		};
		throw "No Linked Pin";
	};	
};

template <class T>
class SimpleOutMeasurePin: public OutPin{
public:	

	SimpleOutMeasurePin(Plugin* plug):OutPin(plug){
	};	
	virtual ~SimpleOutMeasurePin(){};
	virtual CORE_DATA_TYPE GetDataType(){return CORE_DATA_MEASURE;};
	//virtual void Next(){};
	virtual boost::shared_ptr<CoreData> Get(bool copy = false){
		boost::mutex::scoped_lock lock((dynamic_cast<T*>(m_plug))->mutex);
		boost::shared_ptr<Measure> p = (dynamic_cast<T*>(m_plug))->value;

		if(copy)
			return p;

		PMeasure pMeasure = PMeasure(new  Measure(*p));
		return pMeasure;
	};
};

class MeasureInputPin: public InPin{	
public:
	MeasureInputPin(Plugin* plug):InPin(plug){
	};	
	virtual CORE_DATA_TYPE GetDataType(){return CORE_DATA_MEASURE;};

	virtual boost::shared_ptr<CoreData> Get(bool copy = false){
		if (pLinkedPin){
			boost::shared_ptr<CoreData> z = pLinkedPin->Get(copy);
			counter =pLinkedPin->counter;
			return z;
		};
		throw "No Linked Pin";
	};
};

#endif