// CBIRQEditorDoc.cpp : implementation of the CCBIRQEditorDoc class
//

#include "stdafx.h"
#include "CBIRQEditor.h"

#include "CBIRQEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CBIRQEditor.h"

// CCBIRQEditorDoc

IMPLEMENT_DYNCREATE(CCBIRQEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CCBIRQEditorDoc, CDocument)
END_MESSAGE_MAP()


// CCBIRQEditorDoc construction/destruction

CCBIRQEditorDoc::CCBIRQEditorDoc()
{
	CBIRElements = NULL;
	CCBIRQEditorApp *myApp =(CCBIRQEditorApp*)AfxGetApp();
	core = myApp->pCore;
	CBIRElements = new CCBIRElements(core);
}

CCBIRQEditorDoc::~CCBIRQEditorDoc()
{
	delete CBIRElements;
	CBIRElements = NULL;

//CLEAR!!!!!!!!
}

BOOL CCBIRQEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CCBIRQEditorDoc serialization

void CCBIRQEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()){
		CBIRElements->Serialize(ar);
		ar << script;
	}else{
		CBIRElements->Serialize(ar);
		try{
			ar >> script;
			if(script.Left(4) == CString("auto"))
				RunScript();
		}catch(...){

		}
	}
}

void CCBIRQEditorDoc::RunScript(){

	//–азбить построчно
	string all(script.GetBuffer());
	int i=0;
	string line;
	while(i<all.length()){
		char ch = all[i];
		if ((ch==10)|| (ch==13)){
			i++;
			continue;
		}
		if ((ch==';') || (i==all.length())){
			Sleep(200);
			//интерпретируем строку
			MSG message;
			while (::PeekMessage(&message,NULL,0,0,PM_REMOVE)){
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			};
			string command, param;
			string::size_type spaceNum = line.find(" ");
			command = line.substr(0, spaceNum);
			param = line.substr(spaceNum+1);
			
			if (command=="alert"){
				AfxMessageBox(CString(param.c_str()));
			};
			if (command=="sleep"){
				int i =0;
				i = atoi(param.c_str());
				Sleep(i);
			};
			if (command=="run"){
				CBIRElements->scriptRun(param);				
			};
			if (command=="show"){
				CBIRElements->scriptShow(param);
			};
			if (command=="unlink"){
				CBIRElements->scriptUnlink(param);
			};
			if (command=="auto"){
				
			};
			if (command=="link"){
				string param1, param2;
				string::size_type spaceNum = param.find(" ");
				param1 = param.substr(0, spaceNum);
				param2 = param.substr(spaceNum+1);

				CBIRElements->scriptLink(param1, param2);
			};
			line.resize(0);
		}else{
			line.push_back(ch);
		}		
		i++;
	}


	//первое слово с троке - кооманда, воторое - едйсвие

	//команды: run "им€", show "им€", sleep "имллисекунды", alert "сообщение"
	
}
// CCBIRQEditorDoc diagnostics

#ifdef _DEBUG
void CCBIRQEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}



void CCBIRQEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCBIRQEditorDoc commands
