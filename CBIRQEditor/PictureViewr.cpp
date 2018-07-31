// PictureViewr.cpp : implementation file
//

#include "stdafx.h"
#include "CBIRQEditor.h"
#include "PictureViewr.h"


// CPictureViewr dialog

IMPLEMENT_DYNAMIC(CPictureViewr, CDialog)

CPictureViewr::CPictureViewr(GFL_BITMAP *img,CBIR::Core  *core, CWnd* pParent /*=NULL*/)
	: CDialog(CPictureViewr::IDD, pParent)
{
m_img = img;
m_core=core;
}

CPictureViewr::~CPictureViewr()
{
}

void CPictureViewr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPictureViewr, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPictureViewr message handlers

void CPictureViewr::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (m_img!=NULL){		
			//GFL_BITMAP *bmp = img->bitmap;
			LPBITMAPINFOHEADER dib_info; 
			pCore->GetDIBFromBitmap( m_img , &dib_info); 

			CPaintDC dc(this); // device context for painting
			int res= StretchDIBits( dc.m_hDC, 
							0, 0, m_img->Width , m_img->Height, 
							0, 0, m_img->Width , m_img->Height, 
							m_img->Data, (BITMAPINFO *)dib_info, DIB_RGB_COLORS, SRCCOPY ); 
			
			if ( dib_info )
				free( dib_info );
		};
	};	
}
