// interfaceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CBIRQEditor.h"
#include "interfaceDialog.h"


// CinterfaceDialog dialog

IMPLEMENT_DYNAMIC(CinterfaceDialog, CDialog)

CinterfaceDialog::CinterfaceDialog(CBIR::Core *cc, CWnd* pParent /*=NULL*/)
	: CDialog(CinterfaceDialog::IDD, pParent), core(cc)
{

}
BOOL CinterfaceDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	

	CBIR::PluginVectorIt begin, end, it;
	begin = core->interfacePlugins.begin();
	end = core->interfacePlugins.end();

	
	if (begin == end){
		CBIR::PluginEntry *plg = *begin;
		MyListBox.AddString(plg->GetName());
	}else{
		for(it = begin; it != end; it++){
			CBIR::PluginEntry *plg = *it;
			MyListBox.AddString(plg->GetName());
		}
	};
	
	return TRUE;
}

CinterfaceDialog::~CinterfaceDialog()
{
}

void CinterfaceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, MyListBox);
}


BEGIN_MESSAGE_MAP(CinterfaceDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CinterfaceDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// CinterfaceDialog message handlers

void CinterfaceDialog::OnBnClickedButton1()
{
	int nIndex = MyListBox.GetCurSel();
	int nCount = MyListBox.GetCount();
	
	if (nCount >0)
	{
		CBIR::PluginEntry *plgE = (CBIR::PluginEntry *)core->interfacePlugins[nCount-1-nIndex];
		CBIR::InterfacePlugin *plg = (CBIR::InterfacePlugin *)plgE->CreatePlugin();

		//CBIR::InterfacePlugin *plg = (CBIR::InterfacePlugin *)core->interfacePlugins[nCount-1-nIndex];
		//AfxMessageBox (plg->GetName());
		plg->DoModal();
		delete plg;

		//TODO: Add Plugin
		//CInterfacePlugin plg = 	core->plugins->InterfacePlugins->pluginsList.GetAt(core->plugins->InterfacePlugins->pluginsList.FindIndex(nIndex));		
		//plg.call_ShowDialog();
	}
}
