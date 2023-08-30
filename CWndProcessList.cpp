// CWndProcessList.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "CWndProcessList.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// CWndProcessList 对话框

IMPLEMENT_DYNAMIC(CWndProcessList, CDialogEx)

CWndProcessList::CWndProcessList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPEN_PROCESS, pParent)
	, m_ProcessName(_T("")) {

}

CWndProcessList::~CWndProcessList() {
}

void CWndProcessList::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProcessList);
	DDX_Text(pDX, IDC_EDIT1, m_ProcessName);
}


BOOL CWndProcessList::OnInitDialog() {
	CDialogEx::OnInitDialog();

	LONG_PTR lStyle = GetWindowLongPtr(m_ProcessList.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(m_ProcessList.m_hWnd, GWL_STYLE, lStyle);
	m_ProcessList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ProcessList.InsertColumn(0, L"Process Name", 0, 200);
	m_ProcessList.InsertColumn(1, L"Process Id", 0, 200);
	Flush();
	m_ProcessList.SortItems(CompareFunc, reinterpret_cast<LPARAM>(&m_ProcessList));
	return TRUE;
}

void CWndProcessList::Flush() {
	m_ProcessList.DeleteAllItems();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pi{ sizeof(pi) };
	BOOL bRet = Process32First(hSnapshot, &pi);
	DWORD dwIndex = 0;
	while (bRet) {
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.iItem = dwIndex;
		lvItem.iSubItem = 0;
		lvItem.pszText = pi.szExeFile;
		lvItem.lParam = dwIndex;  // 设置一个唯一的 LPARAM 值
		m_ProcessList.InsertItem(&lvItem);
		CString strProcessId;
		strProcessId.Format(L"%d", pi.th32ProcessID);
		m_ProcessList.SetItemText(dwIndex, 1, strProcessId);
		bRet = Process32Next(hSnapshot, &pi);
		dwIndex++;
	}
	CloseHandle(hSnapshot);
}

BEGIN_MESSAGE_MAP(CWndProcessList, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CWndProcessList::OnEnChangeEdit1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWndProcessList::OnDblclkList1)
END_MESSAGE_MAP()


void CWndProcessList::SearchByProcessName(const CString& targetName) {
	int itemCount = m_ProcessList.GetItemCount();

	for (int i = 0; i < itemCount; ++i) {
		CString itemName = m_ProcessList.GetItemText(i, 0);
		CString itemSubName = itemName.Left(targetName.GetLength());  // 获取前N个字符
		if (itemSubName.CompareNoCase(targetName) == 0) {
			m_ProcessList.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_ProcessList.EnsureVisible(i, FALSE);
			m_ProcessList.SetFocus();
			break;
		}
	}
}

int CALLBACK CWndProcessList::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	CListCtrl* pListCtrl = reinterpret_cast<CListCtrl*>(lParamSort);
	CString str1 = pListCtrl->GetItemText(lParam1, 0);
	CString str2 = pListCtrl->GetItemText(lParam2, 0);
	int ret = str1.CompareNoCase(str2);
	return ret;
}

// CWndProcessList 消息处理程序
void CWndProcessList::OnEnChangeEdit1() {
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SearchByProcessName(m_ProcessName);
	UpdateData(false);
}




void CWndProcessList::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nSelectedIndex = (int)m_ProcessList.GetFirstSelectedItemPosition() - 1;
	if (nSelectedIndex != -1) {
		strProcessName = m_ProcessList.GetItemText(nSelectedIndex, 0);
		strProcessId = m_ProcessList.GetItemText(nSelectedIndex, 1);
	}
	OnOK();
}
