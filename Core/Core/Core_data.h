#pragma once

#include "image.h"
#include <vector>


static enum CORE_DIRECTION {
  CORE_DIRECTION_INPUT=0,	  
  CORE_DIRECTION_OUTPUT=1,
  CORE_DIRECTION_NONE=2
};
static enum CORE_PLUGIN_TYPE {
	CORE_PLG_METRIC=0,
	CORE_PLG_INPUT=1,
	CORE_PLG_OUT=2,
	CORE_PLG_LOGIC=3,	
	CORE_PLG_FILTER=4,
	CORE_PLG_NONE=5
};



class Counter{
public:
	Counter(){isInfinity=false;count=0;};
	bool isInfinity;
	bool isUnknown;
	unsigned int count;
};

class Plugin;

class Pin;
class InPin;
class OutPin;

class Pin{
public:
	Counter counter;
	Pin(Plugin *plug){m_plug = plug;};
	virtual ~Pin(){;};
	virtual CORE_DIRECTION GetDirection(){return CORE_DIRECTION_NONE;};
	virtual CORE_DATA_TYPE GetDataType(){return CORE_DATA_NONE;};
	virtual boost::shared_ptr<CoreData> Get(bool copy = false)=0;
	Plugin *m_plug;
};


class InPin: public Pin{
public:
	OutPin* pLinkedPin;
	InPin(Plugin *plug):Pin(plug){ pLinkedPin=NULL;};	

	CORE_DIRECTION GetDirection(){return CORE_DIRECTION_INPUT;};
	OutPin *GetLink(){return pLinkedPin;};
	bool IsLinked (){if(pLinkedPin)return true; return false;};

	void Link(OutPin* pin){
			pLinkedPin = pin;
	};
	virtual void UnLink(){pLinkedPin = NULL;};
	virtual boost::shared_ptr<CoreData> Get(bool copy = false)=0;
	virtual ~InPin(){UnLink();};	
};