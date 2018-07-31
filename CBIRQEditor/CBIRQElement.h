#pragma once


class CCBIRQElement{
private:
	CPoint position;
	int size;
	Core* pCore;
public:
	UINT ID;//Future
	CString description;
	int group;
	int isRun;

	CCBIRQElement(Core* pcore, CPoint point = CPoint(0,0), UINT ID = 0);
	~CCBIRQElement(){
		if (plugin){
			delete plugin;
		}
	};
	CPoint getPosition(){return position;};	
	void setPosition(CPoint newPosition){position = newPosition;};
	UINT getID(){return ID;};
	CString GetName(){
		return CString(plugin->GetName().c_str());
	}

	Plugin *plugin;

	bool updateState(CPoint point);	
	//paint itself on DC
	void draw (CDC *pDC, bool state, Pin *pPin);
	void DrawInCone(CDC *pDc,int x, int y, CBrush &brush);
	void DrawOutCone(CDC *pDc,int x, int y, CBrush &brush);

	//input count
	int inCount;
	//output count
	int outCount;

	void CCBIRQElement::Serialize(CArchive& ar){
		if (ar.IsStoring())
		{
			ar << ID;
			ar << group;
			ar << size;
			ar << position;
			ar << inCount;
			ar << outCount;
			ar << description;

			ar << plugin->GetDataType();
			CString st(plugin->GetName().c_str());
			ar << st;
			//store Plugin Date
			plugin->Serialize(ar);

			ar << plugin->group;
			ar << plugin->groupData;
		}else{
			ar >> ID;
			ar >> group;
			ar >> size;
			ar >> position;
			ar >> inCount;
			ar >> outCount;
			ar >> description;

			CString plugName;
			//search for plugin
			CORE_PLUGIN_TYPE plugType = CORE_PLUGIN_TYPE::CORE_PLG_NONE;
			int tmp;
			ar >> tmp;
			//search for plugin with name as @plugName and create it`s
			PluginVector::iterator it, itEnd;
			
			it=pCore->plugins.begin();
			itEnd=pCore->plugins.end();

			ar >> plugName;

			for(;it!=itEnd;it++){
				
				if ((*it)->GetName() == string(plugName.GetBuffer())){
					plugin = (*it)->CreatePlugin();
					//restore Plugin Data
					plugin->Serialize(ar);
					break;
				}
				
			};
			ar>> plugin->group;
			ar>> plugin->groupData;
		}
	}

private:
	void DrawPins(CDC *pDC, Pin *pPin);
};
