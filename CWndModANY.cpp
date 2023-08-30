// CWndModANY.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "CWndModANY.h"
#include "afxdialogex.h"


// CWndModANY 对话框

IMPLEMENT_DYNAMIC(CWndModANY, CDialogEx)

CWndModANY::CWndModANY(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOD_ANIY, pParent) {

}

CWndModANY::~CWndModANY() {
}

void CWndModANY::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ModList);
}


BOOL CWndModANY::OnInitDialog() {
	CDialogEx::OnInitDialog();

	LONG_PTR lStyle = GetWindowLongPtr(m_ModList.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(m_ModList.m_hWnd, GWL_STYLE, lStyle);
	m_ModList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ModList.InsertColumn(0, L"Moudle Name", 0, 100);
	m_ModList.InsertColumn(1, L"Base Address", 0, 100);
	m_ModList.InsertColumn(2, L"Size", 0, 75);
	m_ModList.InsertColumn(3, L"Path", 0, 300);
	m_ModList.InsertColumn(4, L"Add or Remove", 0, 150);
	//	Flush();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWndModANY, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CWndModANY::OnNMCustomdrawList1)
END_MESSAGE_MAP()


// CWndModANY 消息处理程序


void CWndModANY::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NMLVCUSTOMDRAW* lp = (NMLVCUSTOMDRAW*)pNMHDR;
	auto& cmd = lp->nmcd;
	switch (cmd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
	{
		DWORD  dwIndex = m_ModList.GetItemData(cmd.dwItemSpec);
		if (dwIndex == 0) {
			lp->clrText = RGB(255, 0, 0);
			lp->clrTextBk = RGB(128, 255, 255);
		}
		if (dwIndex == 1) {
			lp->clrTextBk = RGB(128, 0, 128);
			lp->clrText = RGB(255, 255, 255);
		}
	}
	*pResult = CDRF_DODEFAULT;
	break;
	}
}
