// VideoSelector.cpp : implementation file
//

#include "stdafx.h"
#include "Input_VideoReader.h"
#include "VideoSelector.h"


// VideoSelector dialog

IMPLEMENT_DYNAMIC(VideoSelector, CDialog)

VideoSelector::VideoSelector(int step, CString path, CWnd* pParent /*=NULL*/)
: CDialog(VideoSelector::IDD, pParent)
{
	FrameStep = step;
	FilePath = path;
}

BOOL VideoSelector::OnInitDialog(){
	BOOL z = CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT1, FilePath);
	SetDlgItemInt(IDC_EDIT2, FrameStep);

	return z;
}

VideoSelector::~VideoSelector()
{
}

void VideoSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(VideoSelector, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &VideoSelector::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &VideoSelector::OnBnClickedOk)
END_MESSAGE_MAP()


// VideoSelector message handlers

void VideoSelector::OnBnClickedButton1()
{
	char szFilters[]=	"Video (*.avi;*.mpg)||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (TRUE, "", "*.avi;*.mpg",
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK ){
		CString pathName = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT1, pathName);
	}
}

void VideoSelector::OnBnClickedOk()
{
	char pStr[999];
	FrameStep = GetDlgItemInt(IDC_EDIT2);
	GetDlgItemText(IDC_EDIT1, pStr, 600);

	FilePath = CString(pStr);
	OnOK();
}
