// AddElementD.cpp : implementation file
//

#include "stdafx.h"
#include "CBIRQEditor.h"
#include "AddElementD.h"
#include "../core/core/core.h"


#include "resource.h"
#include ".\addelementd.h"



// CAddElementD dialog

IMPLEMENT_DYNAMIC(CAddElementD, CDialog)
CAddElementD::CAddElementD(Core* core, CWnd* pParent /*=NULL*/)
	: CDialog(CAddElementD::IDD, pParent)
{
	CAddElementD::core = core;
	isActive=false;
	m_plge = NULL;

}

CAddElementD::~CAddElementD()
{
}

void CAddElementD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGINSTREE, SelectPluginsCtrl);
}


BEGIN_MESSAGE_MAP(CAddElementD, CDialog)
ON_WM_ACTIVATE()
ON_BN_CLICKED(IDOK, &CAddElementD::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddElementD message handlers
void CAddElementD::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if (!isActive){
		isActive=true;

		TVINSERTSTRUCT ins;

		ins.hParent	= 0;
		ins.hInsertAfter = TVI_LAST;
		ins.item.mask = TVIF_TEXT;
		ins.item.hItem=0;
		ins.item.state=TVIS_EXPANDED;
		ins.item.stateMask=0;
		ins.item.cChildren=1;
		ins.item.lParam=0;
		ins.item.pszText="Input";
		HTREEITEM hInput = SelectPluginsCtrl.InsertItem(&ins);


		ins.hParent	= 0;
		ins.item.pszText="Metric";
		HTREEITEM hMetric = SelectPluginsCtrl.InsertItem(&ins);
		
		ins.item.pszText="Logic";
		HTREEITEM hLogic = SelectPluginsCtrl.InsertItem(&ins);
		
		ins.item.pszText="Filter";
		HTREEITEM hFilter = SelectPluginsCtrl.InsertItem(&ins);

		ins.item.pszText="Control";
		HTREEITEM hControl = SelectPluginsCtrl.InsertItem(&ins);
		for(vector<PluginEntry*>::iterator it = core->plugins.begin(); it!=core->plugins.end(); it++){
			PluginEntry *plgE = *it;
			CString str = CString(plgE->GetName().c_str());

			LPTSTR pszText = str.GetBuffer(100);
			ins.item.pszText = T2A(pszText);
			CORE_PLUGIN_TYPE typ = plgE->GetType();

			switch(typ){
				case CORE_PLG_METRIC:
					ins.hParent = hMetric;
				break;
				case CORE_PLG_INPUT:
					ins.hParent = hInput;
				break;
				case CORE_PLG_OUT:
					ins.hParent = hInput;
				break;
				case CORE_PLG_LOGIC:
					ins.hParent = hLogic;
				break;				
				case CORE_PLG_FILTER:
					ins.hParent = hFilter;
				break;				
				default:
					ins.hParent = hControl;
				break;
			}
			HTREEITEM tmp = SelectPluginsCtrl.InsertItem(&ins);
			SelectPluginsCtrl.SetItemData(tmp, (DWORD_PTR)plgE);
		}
	}
}

void CAddElementD::OnBnClickedOk()
{
	HTREEITEM hItem  = SelectPluginsCtrl.GetSelectedItem();
	if (hItem != NULL){
		DWORD_PTR tmp_plg = SelectPluginsCtrl.GetItemData(hItem);
		m_plge = (PluginEntry*)tmp_plg;

	}
	OnOK();
}

