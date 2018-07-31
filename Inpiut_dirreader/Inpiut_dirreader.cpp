// Inpiut_dirreader.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Inpiut_dirreader.h"
#include "FolderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CInpiut_dirreaderApp, CWinApp)
END_MESSAGE_MAP()



CInpiut_dirreaderApp::CInpiut_dirreaderApp()
{

}


CInpiut_dirreaderApp theApp;


BOOL CInpiut_dirreaderApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



extern "C" PLUGIN_API int isCBIRPlugin		=	TRUE;
extern "C" PLUGIN_API int isInputPlugin		=	TRUE;

extern "C" PLUGIN_API Plugin* init(Core *pCore)
{	
    return new DirReader(pCore);
}

DirReader::DirReader(Core *pCore): Plugin(pCore){	
	currentNum=totalNum =0;
	counter = 0;
	image = PImage(new Image());
	outPins.push_back(new SimpleOutImagePin<DirReader>(this));

	pluginType = CORE_PLUGIN_TYPE::CORE_PLG_INPUT;

	name = "Directory Reader";
	description = "Directory Reader";
};

DirReader::~DirReader(){	
	delete outPins.at(0);
	fileNames.RemoveAll();
	outPins.clear();
};


void DirReader::ReadFiles(LPCTSTR pstr){
   CFileFind finder;

   // build a string with wildcards
   CString strWildcard(pstr);
   strWildcard += _T("\\*.*");

   // start working for files
   BOOL bWorking = finder.FindFile(strWildcard);
   while (bWorking){
      bWorking = finder.FindNextFile();

      // skip . and .. files; otherwise, we'd
      // recur infinitely!

      if (finder.IsDots())
         continue;

      // if it's a directory, recursively search it

      if (finder.IsDirectory()){
         CString str = finder.GetFilePath();
         ReadFiles(str);
      }
	  CString fname = finder.GetFilePath();
	  int a = fname.Find(".jpg");
	  if(a!=-1)
		  fileNames.Add(finder.GetFilePath());
	  
	  a = fname.Find(".bmp");
	  if(a!=-1)
		  fileNames.Add(finder.GetFilePath());
	  
	  a = fname.Find(".png");
	  if(a!=-1)
		  fileNames.Add(finder.GetFilePath());

	  a = fname.Find(".tga");
	  if(a!=-1)
		  fileNames.Add(finder.GetFilePath());
   }
   finder.Close();
};


void DirReader::Serialize(CArchive &ar){

	if (ar.IsStoring()){
		ar<<workPath;
	}else{			
		ar>>workPath;
		if (workPath.GetLength()>4)
			ReadFiles(workPath);
	}
};
UINT DirReader::DoModal(){
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CFolderDialog dlg;
//	if(workPath!=CString(""))
//		dlg.SetRootFolder(workPath);
	UINT result = dlg.DoModal();
	if (result==1){
		workPath = dlg.GetFolderPath();
		fileNames.RemoveAll();
		ReadFiles(dlg.GetFolderPath());
		return MB_OK;
	};
	return 0;
};


long DirReader::GetCount(){
	return fileNames.GetCount();
};



Counter DirReader::Next(){	
	if(counter<fileNames.GetCount()){
		CString path;		
		if (counter>fileNames.GetSize())
			return Counter();
		StartTiming();
		
		Bitmap  = PBitmap(new CxImage());
		Bitmap->Load(fileNames[counter]);


		if(Bitmap.get()==NULL)
			return Counter();		
		image->path = fileNames[counter];
		image->pBitmap= Bitmap;
		Counter c;
		c.count=fileNames.GetCount() - counter++;
		if (counter>fileNames.GetCount())
			counter=0;

		return c;
	};
	if (counter=fileNames.GetCount())
		counter=0;
	return Counter();
};