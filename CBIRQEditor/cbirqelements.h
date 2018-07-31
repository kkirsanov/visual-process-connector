#pragma once
#include "stdafx.h"
#include "AddElementD.h"
#include "CBIRQLink.h"

#include <afxtempl.h>
#include <iostream>
#include <string>
#include <list>


using namespace std;


class CCBIRElements{
public:
	//

	void scriptLink(string param1, string param2){
		CCBIRQElement *pElem1 = GetByDescription(param1);
		CCBIRQElement *pElem2 = GetByDescription(param2);

		if (pElem1&&pElem2){
			//check for StartPin
			int endpinnum =0;
			int startpinnum =0;

			if (pElem1->plugin->outPins.size()<=0)
				return;
			
			OutPin *StartPin= pElem1->plugin->outPins.at(0);
			//scan for same inPins of the same type as Output;
			CORE_DATA_TYPE pinType=StartPin->GetDataType();

			int size = pElem2->plugin->inPins.size();
			if(size==0)
				return;

			InPin *endPin= NULL;
			for(int i=0;i<size;i++){
				if(pinType==pElem2->plugin->inPins.at(i)->GetDataType()
					&& ! pElem2->plugin->inPins.at(i)->GetLink())
				{
					endPin = pElem2->plugin->inPins.at(i);

					endpinnum=i;
					break;
				}
			}
			if (endPin==NULL)
				return;
			//Check for Link doubling

			list<CCBIRQLink*>::iterator it = links.begin();
			for(;it!=links.end(); it++){
				CCBIRQLink *myLink = *it;

				if ((myLink->startPin==StartPin)&&(myLink->endPin==endPin)){
					return;
				}
			};
			CCBIRQLink *link = new CCBIRQLink();		
			link->start=pElem1->ID;
			link->end=pElem2->ID;
			link->startPin=StartPin;
			link->endPin=endPin;
			link->endpinnum=endpinnum;
			link->startpinnum=startpinnum;
			
			links.push_back(link);

			endPin->Link(startPin);
			endPin = NULL;
			startPin = NULL;

		}

	}
	void scriptUnlink(string param){

		vector<list<CCBIRQLink*>::iterator> dellist;//delete deque

		CCBIRQElement *pElem = GetByDescription(param);
		if (pElem){
			//scan for links
			int id = pElem->ID;
	
			for(list<CCBIRQLink*>::iterator it = links.begin(); it!=links.end(); it++){
				if ((*it)->end ==id){
					(*it)->endPin->UnLink();
					dellist.push_back(it);
					delete (*it);
				}
				if ((*it)->start ==id){
					(*it)->endPin->UnLink();
					dellist.push_back(it);
					delete (*it);
				}
			};
			//kill!!!
			for(vector<list<CCBIRQLink*>::iterator>::iterator delIT = dellist.begin(); delIT!=
				dellist.end(); delIT++){
					links.erase(*delIT);
			}
		};
	};

	void scriptShow(string param){
		CCBIRQElement *pElem = GetByDescription(param);
		if (pElem)
			pElem->plugin->DoModal();
	};
	void scriptRun(string param){

		CCBIRQElement *pElem = GetByDescription(param);

		if(pElem && !pElem->plugin->IsWorking){
			pElem->isRun = 1;			
			pElem->plugin->Next();
			pElem->isRun = 0;
		};

	};
	std::list<CCBIRQLink*> links;

	CRgn *m_region;		//changed region;
	CPoint m_point;		//current mouse location
	UINT ID;
	UINT m_CurrentID;
	UINT linkStart;
	bool m_isSnap;		//snap to greed
	UINT m_stepSize;	//greed step length
	bool isLDraging;

	Core*	core;

	OutPin *startPin;
	InPin *endPin;
	int startpinnum, endpinnum;

	list<CCBIRQElement*> elements;
	

	CCBIRQElement* GetByID(UINT ID);
	CCBIRQElement* GetByDescription(string name);
	
	~CCBIRElements(){
		for(list<CCBIRQLink*>::iterator it = links.begin(); it!=links.end(); it++){
			delete (*it);
		};
		
		for(list<CCBIRQElement*>::iterator it = elements.begin(); it!=elements.end(); it++){
			if ((*it)->plugin){
				if((*it)->plugin->inPins.size()>=1)
					(*it)->plugin->inPins.at(0)->UnLink();
				if((*it)->plugin->inPins.size()>=2)
					(*it)->plugin->inPins.at(1)->UnLink();
				if((*it)->plugin->inPins.size()>=3)
					(*it)->plugin->inPins.at(2)->UnLink();
			}
		}

		for(list<CCBIRQElement*>::iterator it = elements.begin(); it!=elements.end(); it++){
			(*it)->plugin->Stop();
		}
		
		for(list<CCBIRQElement*>::iterator it = elements.begin(); it!=elements.end(); it++){
			delete (*it);
		}
	};
	CCBIRElements(Core* core){
		CCBIRElements::core=core;ID=0;m_CurrentID=0;isLDraging=false;m_isSnap=false;m_stepSize=30;linkStart=0;
		startPin=NULL;
		endPin=0;
	};
	void Remove(UINT id){
		CCBIRQElement* elem = GetByID(id);
		elements.remove(elem);
		delete elem;
	};
	void add(CPoint point);
	void add(CCBIRQElement* element);
	bool updateState(CPoint point);
	void draw (CDC *pDC);
	void startLinking();
	void doModal(UINT id){
		GetByID(id)->plugin->DoModal();
	};
	bool isActive(){if(m_CurrentID!=0)return true; else return false;};
	UINT GetActive(){if(m_CurrentID!=0)return m_CurrentID; return 0;};
	void stopLinking();
	void startLDraging(){if(m_CurrentID!=0)isLDraging=true;};
	void stopLDraging(){if(m_CurrentID)isLDraging=false;};
	bool getLDraging(){return isLDraging;};

	void addDialog(CPoint point);

	void CCBIRElements::Serialize(CArchive &ar){
		if (ar.IsStoring())
		{
			ar << ID;
			ar << m_isSnap;
			ar << m_stepSize;
		}
		else
		{
			ar >> ID;
			ar >> m_isSnap;
			ar >> m_stepSize;
		}
		//serializing elements
		//srialize count
		if (ar.IsStoring())
		{
			int count= elements.size();
			ar << count;

			list<CCBIRQElement*>::iterator elIT = elements.begin();
			for(;elIT!=elements.end();elIT++){
				(*elIT)->Serialize(ar);
			};

		}else{
			int count;
			ar >> count;
			for(;count>0;count--){
				CCBIRQElement *elem = new CCBIRQElement(core);
				elem->Serialize(ar);
				elements.push_back(elem);
			}

		}


		//serializing links
		if (ar.IsStoring())
		{
			int count= links.size();
			ar << count;

			list<CCBIRQLink*>::iterator liIT = links.begin();
			//links.Serialize(ar);
			for(;liIT!=links.end();liIT++){
				//(*liIT)->Serialize(ar);
				(*liIT)->Serialize(ar);
			};

		}else{
			int count;
			ar >> count;
			for(;count>0;count--){
				CCBIRQLink *link = new CCBIRQLink();
				link->Serialize(ar);
				links.push_back(link);
				CCBIRQElement *el_start, *el_end;
				el_start = GetByID(link->start);
				el_end = GetByID(link->end);
				//el_start->plugin->Link(el_end->plugin);

				pair<OutPin*, InPin*> p = el_start->plugin->Link(link->startpinnum, el_end->plugin, link->endpinnum);
				link->startPin = p.first;
				link->endPin = p.second;
				if (p.first== NULL || p.second==NULL){
					links.pop_back();
				}
			}
		}
	};
};