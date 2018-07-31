// ImageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Input_image.h"
#include "ImageDialog.h"



// CImageDialog dialog

IMPLEMENT_DYNAMIC(CImageDialog, CDialog)

CImageDialog::CImageDialog(PImage img, Core *core, CWnd* pParent /*=NULL*/)
	: CDialog(CImageDialog::IDD, pParent)
{
	CImageDialog::img = img;

	if(img->pBitmap){
		bitmap = PBitmap(new CxImage(*(img->pBitmap)));
		int maxsize = 356;				 
		double cof;
		if (bitmap->GetWidth() > bitmap->GetHeight()){
			cof = (double)maxsize / bitmap->GetWidth();
		}else{
			cof = (double)maxsize / bitmap->GetHeight();
		};
		if (cof<1)
			bitmap->Resample(bitmap->GetWidth()*cof, bitmap->GetHeight()*cof);
	};
}

CImageDialog::~CImageDialog()
{
}

void CImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CImageDialog::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &CImageDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CImageDialog::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CImageDialog message handlers
void CImageDialog::OnBnClickedButton1()
{
	char szFilters[]=	"Images (*.jpg)||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (TRUE, "", "*.jpg",
	OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK ){
		CString pathName = fileDlg.GetPathName();

		//Change the window's title to the opened file's title.
		CString fileName = fileDlg.GetFileTitle ();
		FileName = pathName;

		bitmap = PBitmap(new CxImage());
		bitmap->Load(pathName.GetBuffer());

		if(bitmap.get()){
			int maxsize = 356;				 
			double cof;					  
			if (bitmap->GetWidth()> bitmap->GetHeight()){
				cof = (double)maxsize / bitmap->GetWidth();
			}else{
				cof = (double)maxsize / bitmap->GetHeight();
			};
		
			if (cof<1)
				bitmap->Resample(bitmap->GetWidth()*cof,  bitmap->GetHeight()*cof);

			Invalidate();
			SetWindowText(fileName);
		};
	}
}

void CImageDialog::OnPaint()
{	
	if(bitmap){	
		CPaintDC dc(this); // device context for painting		
		bitmap->Draw(dc,0,0);		
	};
}

void CImageDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CImageDialog::OnBnClickedButton2()
{
	OnOK();
}

void CImageDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
