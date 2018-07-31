// PathSelector.cpp : implementation file
//

#include "stdafx.h"
#include "Control_ProcessConnector.h"
#include "PathSelector.h"


// PathSelector dialog

IMPLEMENT_DYNAMIC(PathSelector, CDialog)

BOOL PathSelector::OnInitDialog(){
	BOOL z = CDialog::OnInitDialog();
	SetDlgItemText(IDC_PATHEDT,*tmpPath);

	return z;
}
PathSelector::PathSelector(CString *path, CWnd* pParent /*=NULL*/)
	: CDialog(PathSelector::IDD, pParent)
{
	tmpPath = path;
}

PathSelector::~PathSelector()
{
}

void PathSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(PathSelector, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &PathSelector::OnBnClickedButton1)
END_MESSAGE_MAP()


// PathSelector message handlers

void PathSelector::OnBnClickedButton1()
{
	char szFilters[]=
      "Process (*.exe)||";

   // Create an Open dialog; the default file name extension is ".my".
   CFileDialog fileDlg (TRUE, "", "*.exe",
      OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
   
   // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
   // returns IDOK.
   if( fileDlg.DoModal ()==IDOK )
   {
      *tmpPath = fileDlg.GetPathName();
	  SetDlgItemText(IDC_PATHEDT,*tmpPath);
   }
}
