// NetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Filter_network.h"
#include "NetDlg.h"


// CNetDlg dialog

IMPLEMENT_DYNAMIC(CNetDlg, CDialog)

CNetDlg::CNetDlg(DWORD *ip, UINT *remotePort,UINT *localPort, CWnd* pParent /*= NULL*/)
	: CDialog(CNetDlg::IDD, pParent),
	IP(ip), REMOTEPORT(remotePort), LOCALPORT(localPort)
{	

}

CNetDlg::~CNetDlg()
{
}

void CNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, IPCtrl);
}


BEGIN_MESSAGE_MAP(CNetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CNetDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CNetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNetDlg message handlers

void CNetDlg::OnBnClickedButton1()
{
	//IPCtrl.SetAddress
}
BOOL CNetDlg::OnInitDialog(){
	BOOL z = CDialog::OnInitDialog();
	IPCtrl.SetAddress(*IP);
	SetDlgItemInt(IDC_PORT, *REMOTEPORT);
	SetDlgItemInt(IDC_PORT2, *LOCALPORT);

	return z;	
}
void CNetDlg::OnBnClickedOk()
{
	IPCtrl.GetAddress(*IP);

	*REMOTEPORT = GetDlgItemInt(IDC_PORT);
	*LOCALPORT = GetDlgItemInt(IDC_PORT2);
	OnOK();
}
