// CWndMoudleList.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "CWndMoudleList.h"
#include "afxdialogex.h"


// CWndMoudleList 对话框

IMPLEMENT_DYNAMIC(CWndMoudleList, CDialogEx)

CWndMoudleList::CWndMoudleList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOD_List, pParent)
{

}

CWndMoudleList::~CWndMoudleList()
{
}

void CWndMoudleList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ModList);
}


BOOL CWndMoudleList::OnInitDialog() {
	CDialogEx::OnInitDialog();

	LONG_PTR lStyle = GetWindowLongPtr(m_ModList.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(m_ModList.m_hWnd, GWL_STYLE, lStyle);
	m_ModList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ModList.InsertColumn(0, L"Moudle Name", 0, 100);
	m_ModList.InsertColumn(1, L"Base Address", 0, 100);
	m_ModList.InsertColumn(2, L"Size", 0, 75);
	m_ModList.InsertColumn(3, L"Path", 0, 300);
//	Flush();
	return TRUE;
}

BOOL CWndMoudleList::GetMoudleList(DWORD dwProcessId) {
	m_ModList.DeleteAllItems();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &me32);
	DWORD dwIndex = 0;
	while (bRet) {
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.iItem = dwIndex;
		lvItem.iSubItem = 0;
		lvItem.pszText = me32.szModule;
		lvItem.lParam = dwIndex;  // 设置一个唯一的 LPARAM 值
		m_ModList.InsertItem(&lvItem);

		CString strmodBaseAddr;
		strmodBaseAddr.Format(L"%p", me32.modBaseAddr);
		m_ModList.SetItemText(dwIndex, 1, strmodBaseAddr);

		CString strSize;
		strSize.Format(L"%d", me32.modBaseSize);
		m_ModList.SetItemText(dwIndex, 2, strSize);

		CString strPath;
		strPath.Format(L"%s", me32.szExePath);
		m_ModList.SetItemText(dwIndex, 3, strPath);

		bRet = Module32Next(hSnapshot, &me32);
		dwIndex++;
	}
	CloseHandle(hSnapshot);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWndMoudleList, CDialogEx)
END_MESSAGE_MAP()


// CWndMoudleList 消息处理程序
