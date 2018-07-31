#include "stdafx.h"
#include "cbirqelements.h"

//#include "Propertiesdialog.h"
#include <map>


bool CCBIRElements::updateState(CPoint point){
	bool ret = false;
	//int oldEl = m_CurrentID;
	double xScale = 2.1;

	m_point = point;
	if (isLDraging && (m_CurrentID!=0)){
		
		list<CCBIRQElement*>::iterator it = elements.begin();

		for(;it!=elements.end(); it++){
			CCBIRQElement *myElement = *it;
		
			if(myElement->getID()==m_CurrentID){
				if (m_isSnap){
					if ((point.x % m_stepSize)<=m_stepSize)
						point.x = point.x -(point.x % m_stepSize);

					if ((point.y % m_stepSize)<=m_stepSize)
						point.y = point.y -(point.y % m_stepSize);
				};
				myElement->setPosition(point);
				ret = true;//linkStart
				break;
			};
		};
	}else{
		list<CCBIRQElement*>::iterator it = elements.begin();

		UINT oldID =m_CurrentID;
		m_CurrentID = 0;
		for(;it!=elements.end(); it++){
			CCBIRQElement *myElement = *it;
			if(myElement->updateState(point)){
				m_CurrentID = myElement->getID();
				break;
			}
		};
		if (m_CurrentID!=oldID)
			ret = true;
	};

	if (linkStart)
		ret=true;

	return ret;
};

CCBIRQElement *CCBIRElements::GetByID(UINT ID){

	list<CCBIRQElement*>::iterator it = elements.begin();
	for(;it!=elements.end(); it++){
		CCBIRQElement *myElement = *it;
		if (myElement->getID()==ID)
			return myElement;
	};
	return NULL;
}
CCBIRQElement* CCBIRElements::GetByDescription(string name){
	list<CCBIRQElement*>::iterator it = elements.begin();
	for(;it!=elements.end(); it++)
	{
		CCBIRQElement *myElement = *it;
		if (0==myElement->description.Compare(CString(name.c_str())))
			return myElement;
	};
	return NULL;
}

void CCBIRElements::draw(CDC* pDC){
	
	list<CCBIRQElement*>::iterator it = elements.begin();
	for(;it!=elements.end(); it++){
		CCBIRQElement *myElement = *it;
		if (myElement->getID()==m_CurrentID){
			myElement->draw(pDC, true, startPin);
		}else{
			myElement->draw(pDC, false, startPin);
		};
	};
	if (linkStart!=0){
		CCBIRQElement *myElement = GetByID(linkStart);
		pDC->MoveTo(myElement->getPosition());
		pDC->LineTo(m_point);
	}

	list<CCBIRQLink*>::iterator it2 = links.begin();
	for(;it2!=links.end(); it2++)
	{
		CCBIRQLink *myLink = *it2;
		CCBIRQElement *sElement = GetByID(myLink->start);
		CCBIRQElement *eElement = GetByID(myLink->end);

		int x=0;
		int y=0;
		x=sElement->getPosition().x-40/2 + myLink->startpinnum*15*2 +5;
		y=sElement->getPosition().y-40+5;

		pDC->MoveTo(x+15/2,y-15/2);

		POINT points[4];
		
		POINT p;
		p.x=x+15/2;
		p.y=y-15/2;

		points[0] = p;
		p.y-=30;
		points[1] = p;


		x=eElement->getPosition().x-40/2 + myLink->endpinnum*15*2+5;
		y=eElement->getPosition().y+40-20;

		CPen myPen;
		CPen *oldPen;
		myPen.CreatePen(PS_SOLID,1, RGB(0,0,0));

		oldPen = (CPen*)pDC->SelectObject(myPen);

		p.x=x+15/2;
		p.y=y+15/2;
		points[3] = p;

		p.y+=30;
		points[2] = p;

		int cX = min (points[0].x, points[3].x) + (max(points[0].x, points[3].x) - min (points[0].x, points[3].x))/2 ;
		int cY = min (points[0].y, points[3].y) + (max(points[0].y, points[3].y) - min (points[0].y, points[3].y))/2 ;

		pDC->PolyBezier(points, 4);
		pDC->Ellipse(cX-15, cY-15, cX+15, cY+15);
		//pDC->TextOut(cX-10,cY-8, myLink->Description);


	};
};

void CCBIRElements::add(CPoint point){
	ID++;
	//CCBIRQElement *newElement = new CCBIRQElement(point, ID);
	//elements.InsertAfter(elements.GetTailPosition(), *newElement);
	elements.push_back(new CCBIRQElement(core, point, ID));

	//newElement->spawnDialog();
};

void CCBIRElements::add(CCBIRQElement *element){
	if (element->getID()>=ID)
		ID = element->getID()+1;
	elements.push_back(element);
};
void CCBIRElements::startLinking(){
	if (m_CurrentID==0)
		return;
	if (m_CurrentID!=0)
		linkStart =	m_CurrentID;
	CCBIRQElement *el = GetByID(m_CurrentID);
	int size = el->plugin->outPins.size();

	if(size==0){
		linkStart = 0;
		startPin =NULL;
		endPin =NULL;
		return;
	};
	startpinnum = 0;

	startPin = el->plugin->outPins[0];
	//for(int i=0;i<size;i++){
	//	el->plugin->outPins[i]
	//}
};
void CCBIRElements::stopLinking(){	
	
	if ((linkStart!=m_CurrentID)&&(m_CurrentID!=0)&&(linkStart!=0)){
		//check for StartPin
		if(startPin==NULL)
			return;
		//scan for same inPins of the same type as Output;
		CORE_DATA_TYPE pinType=startPin->GetDataType();

		CCBIRQElement *el = GetByID(m_CurrentID);

		int size = el->plugin->inPins.size();
		
		if(size==0)
			return;
		endPin= NULL;
		for(int i=0;i<size;i++){
			if(pinType==el->plugin->inPins[i]->GetDataType()
				&& ! el->plugin->inPins[i]->GetLink())
			{
				endPin = el->plugin->inPins[i];
				endpinnum=i;
				break;
			}
		}
		if (endPin==NULL)
			return;
		//Cheeck for Link doubling

		list<CCBIRQLink*>::iterator it = links.begin();
		for(;it!=links.end(); it++){
			CCBIRQLink *myLink = *it;

			if ((myLink->start==linkStart)&&(myLink->end==m_CurrentID)){
				linkStart = 0;
				return;
			}
		};
		CCBIRQLink *link = new CCBIRQLink();		
		link->start=linkStart;
		link->end=m_CurrentID;
		link->startPin=startPin;
		link->endpinnum=endpinnum;
		link->startpinnum=startpinnum;
		link->endPin=endPin;
		links.push_back(link);

		endPin->Link(startPin);
		endPin = NULL;
		startPin = NULL;

	}
	linkStart = 0;
	endPin = NULL;
	startPin = NULL;
};


/*
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
*/
void CCBIRElements::addDialog(CPoint point){
	if (isActive())
	{	
		//Do something with current element
	}else{//Spawn an Add Dialog.
		//core->showName();
		ID++;
		CAddElementD dlg(core);
		dlg.DoModal();

		PluginEntry *plge= dlg.GetPlgE();
		if(plge!=NULL){
			CCBIRQElement *newElement = new CCBIRQElement(core, point, ID);
			newElement->plugin = plge->CreatePlugin();
			elements.push_back(newElement);
		}
	}
};

void CCBIRQElement::DrawPins(CDC *pDC, Pin *pPin=NULL){
	int size =this->plugin->inPins.size();
	int x= position.x-this->size/2;
	int y= position.y;
	int s=15;	


	std::map<CORE_DATA_TYPE, COLORREF> types;
	types[CORE_DATA_METRIC] = RGB(0,0,255);	
	types[CORE_DATA_IMAGE] = RGB(0,255,0);
	types[CORE_DATA_LOGIC] = RGB(128,128,128);
	types[CORE_DATA_NONE] = RGB(255,0,0);

	CBrush* pOldBrush;
	CPen* pOldPen;

	CPen pen;
	CBrush brush;

	pen.CreatePen(PS_SOLID | PS_COSMETIC, 1, RGB(0, 0, 0));
	brush.CreateSolidBrush(RGB(255, 255, 255));

	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = pDC->SelectObject(&brush);

	for(int i=0;i<size;i++){
		InPin *Pin;
		Pin = plugin->inPins[i];		
		int inY = y+this->size;
		int newX=x+(i*s*2);

		CBrush brush;
		COLORREF col=types[Pin->GetDataType()];		

		if(pPin!=NULL){
			bool isFree = Pin->GetLink();

			if (isFree&& ((pPin->GetDataType()!=Pin->GetDataType())||(pPin->GetDirection()==Pin->GetDirection()))){
				brush.CreateSolidBrush(RGB(255,0,0));
				
			}else{
				brush.CreateSolidBrush(col);
			}
		}else{
			brush.CreateSolidBrush(col);
		}
		DrawInCone(pDC,newX+s/2, inY+s/2-10,brush);
	};

	size =this->plugin->outPins.size();
	for(int i=0;i<size;i++){
		OutPin *Pin;
		Pin = plugin->outPins[i];
		int outY = y-this->size;
		int newX=x+(i*s*2);
			
		COLORREF col = types[Pin->GetDataType()];
		CBrush brush;
		if(pPin!=NULL){
			if ((pPin->GetDataType()!=Pin->GetDataType())||(pPin->GetDirection()==Pin->GetDirection())){
				brush.CreateSolidBrush(RGB(255,0,0));
			}else{
				brush.CreateSolidBrush(col);
			}
		}else{
			brush.CreateSolidBrush(col);
		}
		
		DrawOutCone(pDC,newX+s/2, outY-s/2+10, brush);		
	};
   pDC->SelectObject(pOldPen);
   pDC->SelectObject(pOldBrush);
};