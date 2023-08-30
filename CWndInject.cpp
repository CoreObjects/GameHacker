// CWndInject.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "CWndInject.h"
#include "afxdialogex.h"
#include "x86EntryPointInject.h"

// CWndInject 对话框

IMPLEMENT_DYNAMIC(CWndInject, CDialogEx)

CWndInject::CWndInject(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_0, pParent)
	, m_bIsInject(FALSE), m_isSuspend(FALSE), m_bIsDebug(FALSE) {

}

CWndInject::~CWndInject() {
}

void CWndInject::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ExeList);
	DDX_Check(pDX, IDC_CHECK1, m_bIsInject);
	DDX_Check(pDX, IDC_CHECK2, m_isSuspend);
	DDX_Check(pDX, IDC_CHECK3, m_bIsDebug);
}


BOOL CWndInject::OnInitDialog() {
	CDialogEx::OnInitDialog();
	LONG_PTR lStyle = GetWindowLongPtr(m_ExeList.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(m_ExeList.m_hWnd, GWL_STYLE, lStyle);
	m_ExeList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ExeList.InsertColumn(0, L"Game Name", 0, 200);
	m_ExeList.InsertColumn(1, L"ExePath", 0, 200);
	m_ExeList.InsertColumn(2, L"Folder", 0, 200);
	m_ExeList.InsertColumn(3, L"Cmd", 0, 200);
	m_ExeList.InsertColumn(4, L"Dll Path", 0, 200);
	LoadGame();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWndInject, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndInject::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWndInject::OnNMDblclkList1)
	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_START, &CWndInject::OnStart)
	ON_COMMAND(ID_SET, &CWndInject::OnSet)
	ON_COMMAND(ID_DELETE, &CWndInject::OnDelete)
	ON_COMMAND(ID_FLUSH, &CWndInject::OnFlush)
END_MESSAGE_MAP()


// CWndInject 消息处理程序


void CWndInject::Init(CString& _AppPath) {
	m_strAppPath = _AppPath;
	m_strGameIniPath.Format(L"%sset\\Game.ini", m_strAppPath.GetBuffer());
	//AfxMessageBox(m_strGameIniPath);
}

void CWndInject::AddGame(CString& _GameName, CString& _GameExe, CString& _GamePath, CString& _GameCmds, CString& _DllPath) {
	int nCount = GetPrivateProfileInt(L"main", L"count", 0, m_strGameIniPath);
	nCount++;
	CString strCount;
	strCount.Format(L"%d", nCount);
	WritePrivateProfileStringW(L"main", L"count", strCount, m_strGameIniPath);
	DWORD dwCount = m_ExeList.GetItemCount();
	m_ExeList.InsertItem(dwCount, _GameName);
	m_ExeList.SetItemText(dwCount, 1, _GameExe);
	m_ExeList.SetItemText(dwCount, 2, _GamePath);
	m_ExeList.SetItemText(dwCount, 3, _GameCmds);
	m_ExeList.SetItemText(dwCount, 4, _DllPath);
	CString strKey;
	strKey.Format(L"count_%d", nCount);
	WritePrivateProfileStringW(strKey, L"GameName", _GameName, m_strGameIniPath);
	WritePrivateProfileStringW(strKey, L"GameExe", _GameExe, m_strGameIniPath);
	WritePrivateProfileStringW(strKey, L"GamePath", _GamePath, m_strGameIniPath);
	WritePrivateProfileStringW(strKey, L"GameCmd", _GameCmds, m_strGameIniPath);
	WritePrivateProfileStringW(strKey, L"DllPath", _DllPath, m_strGameIniPath);
}

void CWndInject::SaveGame(CString& _GameName, CString& _GameExe, CString& _GamePath, CString& _GameCmds, CString& _DllPath) {
	int nSelectedIndex = (int)m_ExeList.GetFirstSelectedItemPosition() - 1;
	if (nSelectedIndex != -1) {
		// nSelectedIndex 是所选行的索引
		m_ExeList.SetItemText(nSelectedIndex, 0, _GameName);
		m_ExeList.SetItemText(nSelectedIndex, 1, _GameExe);
		m_ExeList.SetItemText(nSelectedIndex, 2, _GamePath);
		m_ExeList.SetItemText(nSelectedIndex, 3, _GameCmds);
		m_ExeList.SetItemText(nSelectedIndex, 4, _DllPath);
		CString strKey;
		strKey.Format(L"count_%d", nSelectedIndex + 1);
		WritePrivateProfileStringW(strKey, L"GameName", _GameName, m_strGameIniPath);
		WritePrivateProfileStringW(strKey, L"GameExe", _GameExe, m_strGameIniPath);
		WritePrivateProfileStringW(strKey, L"GamePath", _GamePath, m_strGameIniPath);
		WritePrivateProfileStringW(strKey, L"GameCmd", _GameCmds, m_strGameIniPath);
		WritePrivateProfileStringW(strKey, L"DllPath", _DllPath, m_strGameIniPath);
	}
}

void CWndInject::LoadGame() {
	m_ExeList.DeleteAllItems();
	int nCount = GetPrivateProfileInt(L"main", L"count", 0, m_strGameIniPath);
	for (int i = 0; i < nCount; i++) {
		CString GameName, GameExe, GamePath, GameCmds, DllPath, _AppName;
		_AppName.Format(L"count_%d", i + 1);
		wchar_t wRead[MAX_PATH]{ 0 };
		GetPrivateProfileString(_AppName, L"GameName", L"", wRead, MAX_PATH, m_strGameIniPath);
		GameName.Format(L"%s", wRead);
		GetPrivateProfileString(_AppName, L"GameExe", L"", wRead, MAX_PATH, m_strGameIniPath);
		GameExe.Format(L"%s", wRead);
		GetPrivateProfileString(_AppName, L"GamePath", L"", wRead, MAX_PATH, m_strGameIniPath);
		GamePath.Format(L"%s", wRead);
		GetPrivateProfileString(_AppName, L"GameCmd", L"", wRead, MAX_PATH, m_strGameIniPath);
		GameCmds.Format(L"%s", wRead);
		GetPrivateProfileString(_AppName, L"DllPath", L"", wRead, MAX_PATH, m_strGameIniPath);
		DllPath.Format(L"%s", wRead);

		m_ExeList.InsertItem(i, GameName);
		m_ExeList.SetItemText(i, 1, GameExe);
		m_ExeList.SetItemText(i, 2, GamePath);
		m_ExeList.SetItemText(i, 3, GameCmds);
		m_ExeList.SetItemText(i, 4, DllPath);
	}
}

void CWndInject::OnBnClickedButton1() {
	// TODO: 在此添加控件通知处理程序代码
	m_AddGame.Init(this);
	if (m_AddGame.DoModal() == IDOK) {


	}

}




void CWndInject::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	Start();
}



void CWndInject::Start() {
	int nSelectedIndex = (int)m_ExeList.GetFirstSelectedItemPosition() - 1;
	if (nSelectedIndex != -1) {
		// nSelectedIndex 是所选行的索引
		CString strGameFullPath = m_ExeList.GetItemText(nSelectedIndex, 1);
		CString strGamePath = m_ExeList.GetItemText(nSelectedIndex, 2);
		CString strGameCmds = m_ExeList.GetItemText(nSelectedIndex, 3);
		CString DllPath = m_ExeList.GetItemText(nSelectedIndex, 4);
		CStringA DllPathA = CW2A(DllPath);
		UpdateData(TRUE);
		if (m_bIsInject) {
			m_Injector.startProcessWithInject(strGameFullPath.GetBuffer(), strGamePath.GetBuffer(), strGameCmds.GetBuffer(), DllPathA.GetBuffer());			
		//	m_Injector.startProcessWithRemoteThreadInject(strGameFullPath.GetBuffer(), strGamePath.GetBuffer(), strGameCmds.GetBuffer(), DllPathA.GetBuffer());
		}
		else {
			m_Injector.startProcess(strGameFullPath.GetBuffer(), strGamePath.GetBuffer(), strGameCmds.GetBuffer());
		}
		if (m_bIsDebug) {
			m_Injector.startDbg();
		}
		//	m_Injector.startProcessWithDbg(strGameFullPath.GetBuffer(), strGamePath.GetBuffer(), strGameCmds.GetBuffer());
		m_Injector.Clear();
	}
}

void CWndInject::SaveLstToIni() {
	DeleteFile(m_strGameIniPath);
	int nCount = m_ExeList.GetItemCount();
	CString strCount;
	strCount.Format(L"%d", nCount);
	WritePrivateProfileStringW(L"main", L"count", strCount, m_strGameIniPath);
	for (int i = 0; i < nCount; i++) {
		CString strGameName = m_ExeList.GetItemText(i, 0);
		CString strGameFullPath = m_ExeList.GetItemText(i, 1);
		CString strGamePath = m_ExeList.GetItemText(i, 2);
		CString strGameCmds = m_ExeList.GetItemText(i, 3);
		CString DllPath = m_ExeList.GetItemText(i, 4);
		CString strKey;
		strKey.Format(L"count_%d", i + 1);
		WritePrivateProfileStringW(strKey, L"GameName", strGameName, m_strGameIniPath);
		WritePrivateProfileStringW(strKey, L"GameExe", strGameFullPath, m_strGameIniPath);
		WritePrivateProfileStringW(strKey, L"GamePath", strGamePath, m_strGameIniPath);
		WritePrivateProfileStringW(strKey, L"GameCmd", strGameCmds, m_strGameIniPath);
		WritePrivateProfileStringW(strKey, L"DllPath", DllPath, m_strGameIniPath);
	}
}

void CWndInject::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) {
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: 在此处添加消息处理程序代码
	::CheckMenuItem(pPopupMenu->m_hMenu, ID_START, MF_CHECKED);

}


void CWndInject::OnContextMenu(CWnd* pWnd, CPoint point) {
	// TODO: 在此处添加消息处理程序代码
	if (pWnd->GetDlgCtrlID() == IDC_LIST1) {
		// 加载菜单资源
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CMenu* pSubMenu = menu.GetSubMenu(0);

		pSubMenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this); // 弹出菜单
	}
}


void CWndInject::OnStart() {
	// TODO: 在此添加命令处理程序代码
	Start();
}


void CWndInject::OnSet() {
	// TODO: 在此添加命令处理程序代码
	int nSelectedIndex = (int)m_ExeList.GetFirstSelectedItemPosition() - 1;
	if (nSelectedIndex != -1) {
		// nSelectedIndex 是所选行的索引
		CString strGameName = m_ExeList.GetItemText(nSelectedIndex, 0);
		CString strGameFullPath = m_ExeList.GetItemText(nSelectedIndex, 1);
		CString strGamePath = m_ExeList.GetItemText(nSelectedIndex, 2);
		CString strGameCmds = m_ExeList.GetItemText(nSelectedIndex, 3);
		CString DllPath = m_ExeList.GetItemText(nSelectedIndex, 4);
		m_AddGame.Init(this, false, strGameName, strGameFullPath, strGamePath, strGameCmds, DllPath);
		if (m_AddGame.DoModal() == IDOK) {


		}
	}
	
}


void CWndInject::OnDelete() {
	// TODO: 在此添加命令处理程序代码
	int nSelectedIndex = (int)m_ExeList.GetFirstSelectedItemPosition() - 1;
	if (nSelectedIndex != -1) {
		m_ExeList.DeleteItem(nSelectedIndex);
		SaveLstToIni();
		LoadGame();
	}
}


void CWndInject::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	LoadGame();
}
