// CWndInjectDetection.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "CWndInjectDetection.h"
#include "afxdialogex.h"


// CWndInjectDetection 对话框

IMPLEMENT_DYNAMIC(CWndInjectDetection, CDialogEx)

CWndInjectDetection::CWndInjectDetection(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_1, pParent)
	, m_ProcessName(_T("")), m_Inf_ProcessName(_T("")) {

}

CWndInjectDetection::~CWndInjectDetection()
{
}

void CWndInjectDetection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ProcessName);
	DDX_Text(pDX, IDC_EDIT2, m_Inf_ProcessName);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}
bool CWndInjectDetection::InstallPage(CDialogEx* dlg, DWORD dwPageId, CTabCtrl* pTab, const std::wstring& strPageName, int nCmdShow /*= SW_HIDE*/) {
	if (dlg == nullptr || pTab == nullptr) {
		return false;
	}
	pTab->InsertItem(pTab->GetItemCount(), strPageName.c_str());
	m_Pages.push_back({ strPageName.c_str(),dlg });
	dlg->Create(dwPageId, pTab);
	CRect rect;
	pTab->GetClientRect(&rect);
	rect.bottom -= 25;
	dlg->MoveWindow(&rect);
	dlg->ShowWindow(nCmdShow);
	return true;
}

bool CWndInjectDetection::InstallPage(CDialogEx* dlg, DWORD dwPageId, std::wstring&& strPageName /*= L""*/, int nCmdShow /*= SW_HIDE*/) {
	return InstallPage(dlg, dwPageId, &m_Tab, strPageName, nCmdShow);
}

BOOL CWndInjectDetection::OnInitDialog() {
	CDialogEx::OnInitDialog();
	InstallPage(&m_NomalMoudleDlg, IDD_MOD_List, L"Normal Process", SW_SHOW);
	InstallPage(&m_InfectMoudleDlg, IDD_MOD_List, L"Infected Process", SW_HIDE);
	InstallPage(&m_ModeAnalysisDlg, IDD_MOD_ANIY, L"Analysis result", SW_HIDE);
//	InstallPage(&m_MoudleDlg, IDD_MOD_List, L"Infected Process", SW_HIDE);
// 	m_Tab.InsertItem(0, L"Normal Process");
// 	m_Tab.InsertItem(1, L"Infected Process");
// 	m_Tab.InsertItem(2, L"Analysis result");
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWndInjectDetection, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndInjectDetection::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWndInjectDetection::OnBnClickedButton2)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CWndInjectDetection::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON5, &CWndInjectDetection::OnBnClickedButton5)
END_MESSAGE_MAP()


// CWndInjectDetection 消息处理程序


void CWndInjectDetection::OnBnClickedButton1() {
	// TODO: 在此添加控件通知处理程序代码
	if (m_ProcessDlg.DoModal() == IDOK) {
		m_ProcessName = m_ProcessDlg.strProcessName;
		m_ProcessId = wcstoul(m_ProcessDlg.strProcessId, 0, 10);
		m_NomalMoudleDlg.GetMoudleList(m_ProcessId);
		UpdateData(FALSE);
	}
}


void CWndInjectDetection::OnBnClickedButton2() {
	// TODO: 在此添加控件通知处理程序代码
	if (m_ProcessDlg.DoModal() == IDOK) {
		m_Inf_ProcessName = m_ProcessDlg.strProcessName;
		m_Inf_ProcessId = wcstoul(m_ProcessDlg.strProcessId, 0, 10);
		m_InfectMoudleDlg.GetMoudleList(m_Inf_ProcessId);
		UpdateData(FALSE);
	}
}


void CWndInjectDetection::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if (nType == 1)return;
	if (m_Tab.m_hWnd == nullptr)return;
	CRect rect;
	m_Tab.GetClientRect(&rect);
	//rect.top += 25;
	rect.bottom -= 25;
	for (auto& i : m_Pages) {
		i.pDlg->MoveWindow(&rect);
	}
}


void CWndInjectDetection::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) {
	// TODO: 在此添加控件通知处理程序代码
	int nSelectedTab = m_Tab.GetCurSel(); // 获取当前选中的选项卡索引
	if (nSelectedTab != -1) {
		for (auto& stcdlg : m_Pages) {
			stcdlg.pDlg->ShowWindow(SW_HIDE);
		}
		m_Pages[nSelectedTab].pDlg->ShowWindow(SW_SHOW);
	}
	*pResult = 0;

}
struct CaseInsensitiveCompare {
	bool operator()(const CString& a, const CString& b) const {
		return a.CompareNoCase(b) < 0;
	}
};
typedef std::vector<CString> RowVector; // 用于存储一整行的内容
void CWndInjectDetection::CompareCListCtrls(CListCtrl& listCtrl1, CListCtrl& listCtrl2, CListCtrl& listCtrlResult) {
	std::map<CString, RowVector, CaseInsensitiveCompare> map1, map2;
	listCtrlResult.DeleteAllItems();
	// 从第一个CListCtrl读取内容，并存储第四列的值以及整行的内容
	for (int i = 0; i < listCtrl1.GetItemCount(); ++i) {
		RowVector row = {
			listCtrl1.GetItemText(i, 0),
			listCtrl1.GetItemText(i, 1),
			listCtrl1.GetItemText(i, 2),
			listCtrl1.GetItemText(i, 3)
		};
		map1[row[3]] = row;  // 假设第四列的值是唯一的
	}

	// 从第二个CListCtrl读取内容，并存储第四列的值以及整行的内容
	for (int i = 0; i < listCtrl2.GetItemCount(); ++i) {
		RowVector row = {
			listCtrl2.GetItemText(i, 0),
			listCtrl2.GetItemText(i, 1),
			listCtrl2.GetItemText(i, 2),
			listCtrl2.GetItemText(i, 3)
		};
		map2[row[3]] = row;  // 假设第四列的值是唯一的
	}

	// 找出在map2中但不在map1中的项，并添加到listCtrlResult
	for (const auto& pair : map2) {
		if (map1.find(pair.first) == map1.end()) {
			// 这一行在listCtrl2中，但不在listCtrl1中
			int index = listCtrlResult.InsertItem(listCtrlResult.GetItemCount(), pair.second[0]);
			listCtrlResult.SetItemText(index, 1, pair.second[1]);
			listCtrlResult.SetItemText(index, 2, pair.second[2]);
			listCtrlResult.SetItemText(index, 3, pair.second[3]);
			listCtrlResult.SetItemText(index, 4, _T("Add"));
			listCtrlResult.SetItemData(index, 0);
		}
	}

	// 找出在map1中但不在map2中的项，并添加到listCtrlResult
	for (const auto& pair : map1) {
		if (map2.find(pair.first) == map2.end()) {
			// 这一行在listCtrl1中，但不在listCtrl2中
			int index = listCtrlResult.InsertItem(listCtrlResult.GetItemCount(), pair.second[0]);
			listCtrlResult.SetItemText(index, 1, pair.second[1]);
			listCtrlResult.SetItemText(index, 2, pair.second[2]);
			listCtrlResult.SetItemText(index, 3, pair.second[3]);
			listCtrlResult.SetItemText(index, 4, _T("Remove"));
			listCtrlResult.SetItemData(index, 1);
		}
	}

	// 找出在map1和map2中都存在的项，并添加到listCtrlResult
	for (const auto& pair : map1) {
		if (map2.find(pair.first) != map2.end()) {
			int index = listCtrlResult.InsertItem(listCtrlResult.GetItemCount(), pair.second[0]);
			listCtrlResult.SetItemText(index, 1, pair.second[1]);
			listCtrlResult.SetItemText(index, 2, pair.second[2]);
			listCtrlResult.SetItemText(index, 3, pair.second[3]);
			listCtrlResult.SetItemText(index, 4, _T("Normal"));
			listCtrlResult.SetItemData(index, 2);
		}
	}
}
void CWndInjectDetection::OnBnClickedButton5() {
	// TODO: 在此添加控件通知处理程序代码
	CompareCListCtrls(m_NomalMoudleDlg.m_ModList, m_InfectMoudleDlg.m_ModList, m_ModeAnalysisDlg.m_ModList);
}
