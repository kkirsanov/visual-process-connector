#include "StdAfx.h"
#include "CBIRQElement.h"

CCBIRQElement::CCBIRQElement(Core* pcore, CPoint point, UINT ID)
{
	pCore = pcore;
	CCBIRQElement::ID=ID;
	position=point; 
	size=40;
	inCount = 1;
	outCount = 1;
	group=0;
	isRun=0;
};

bool CCBIRQElement::updateState(CPoint point){
	double xScale=2.1;
	if (((max(point.x, position.x)-min(point.x, position.x))<=size*xScale)
		&&((max(point.y, position.y)-min(point.y, position.y))<=size))
	{
		return true;
	}else{
		return false;
	};
};

void CCBIRQElement::draw (CDC *pDC, bool state, Pin *pPin = NULL){
	CPen *pOldPen;
	size=35;
	double xScale = 2.1;
	
	if (isRun){
		CPen myPen1;		

		myPen1.CreatePen(PS_SOLID, 7, RGB(0,0,255));
		pOldPen = pDC->SelectObject(&myPen1);
		pDC->Rectangle(CRect(position.x-size*xScale, position.y-size, position.x+size*xScale, position.y+size-10));
		pDC->SelectObject(pOldPen);

	}
	if (!state){
		CPen myPen1;		
		if(plugin->IsWorking){
			myPen1.CreatePen(PS_SOLID, 4, RGB(0,255,0));
		}else{
			myPen1.CreatePen(PS_SOLID, 1, RGB(0,100,0));
		}

		pOldPen = pDC->SelectObject(&myPen1);
		pDC->Rectangle(CRect(position.x-size*xScale, position.y-size, position.x+size*xScale, position.y+size-10));
		if(description.GetLength())			
			pDC->TextOut(position.x-description.GetLength()*3.9, position.y-15,	description);
		else
			pDC->TextOut(position.x-plugin->GetName().size()*3.9, position.y-15, CString(plugin->GetName().c_str()));

	}else{
		CPen myPen1;

		if(plugin->IsWorking){
			myPen1.CreatePen(PS_SOLID, 4, RGB(0,255,0));
		}else{
			myPen1.CreatePen(PS_SOLID, 3, RGB(100,100,100));
		}

		pOldPen = pDC->SelectObject(&myPen1);
		pDC->Rectangle(CRect(position.x-size*xScale, position.y-size, position.x+size*xScale, position.y+size-10));
		pDC->SelectObject(pOldPen);

		pDC->TextOut(position.x-plugin->GetName().size()*3.5, position.y-15, CString(plugin->GetName().c_str()));
	};
	int x=position.x;
	int y=position.y-40;

	DrawPins(pDC, pPin);
	int t = plugin->GetTime();
	char str[20];
	itoa(t, str, 10);
	if (t!=0)
		pDC->TextOut(position.x-10, position.y+1, str);

	itoa(plugin->group, str, 10);
	pDC->TextOut(position.x+size*xScale-10, position.y-size+2, str);
	itoa(plugin->groupData, str, 10);
	pDC->TextOut(position.x-size*xScale+4, position.y-size+2, str);

};	
void CCBIRQElement::DrawInCone(CDC *pDc, int x,int y, CBrush &brush){

	y-=10;
	//x and y = center
	pDc->MoveTo(x-5,y+5);
	pDc->LineTo(x-5,y);
	pDc->LineTo(x,y-5);
	pDc->LineTo(x+5,y);
	pDc->LineTo(x+5,y+5);
	pDc->LineTo(x-5,y+5);

	CRgn rgn;
	CPoint ptVertex[5];

	ptVertex[0].x = x-5;
	ptVertex[0].y = y+5;

	ptVertex[1].x = x-5;
	ptVertex[1].y = y;

	ptVertex[2].x = x;
	ptVertex[2].y = y-5;
	
	ptVertex[3].x = x+5;
	ptVertex[3].y = y;
	
	ptVertex[4].x = x+5;
	ptVertex[4].y = y+5;


	rgn.CreatePolygonRgn( ptVertex, 5, ALTERNATE);

	pDc->FillRgn( &rgn, &brush);
}
void CCBIRQElement::DrawOutCone(CDC *pDc, int x,int y, CBrush &brush){
	//x and y = center

	CRgn rgn;
	CPoint ptVertex[5];

	ptVertex[0].x = x-5;
	ptVertex[0].y = y-5;

	ptVertex[1].x = x-5;
	ptVertex[1].y = y;

	ptVertex[2].x = x;
	ptVertex[2].y = y+5;
	
	ptVertex[3].x = x+5;
	ptVertex[3].y = y;
	
	ptVertex[4].x = x+5;
	ptVertex[4].y = y-5;


	rgn.CreatePolygonRgn( ptVertex, 5, ALTERNATE);

	pDc->FillRgn( &rgn, &brush);

	pDc->MoveTo(x-5,y-5);
	pDc->LineTo(x-5,y);
	pDc->LineTo(x,y+5);
	pDc->LineTo(x+5,y);
	pDc->LineTo(x+5,y-5);
	pDc->LineTo(x-5,y-5);
}