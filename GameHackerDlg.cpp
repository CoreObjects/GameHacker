
// GameHackerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GameHacker.h"
#include "GameHackerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGameHackerDlg 对话框



CGameHackerDlg::CGameHackerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAMEHACKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameHackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CGameHackerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CGameHackerDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGameHackerDlg 消息处理程序

BOOL CGameHackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	TCHAR szExePath[MAX_PATH]{ 0 };
	GetModuleFileName(NULL, szExePath, MAX_PATH);
	for (int i = MAX_PATH - 1; szExePath[i] != L'\\'; i--){
		szExePath[i] = 0;
	}
	strAppPath = szExePath;
	m_Inject.Init(strAppPath);
	m_RAN.SetAppPath(strAppPath);
	InstallPage(&m_Inject, IDD_PAGE_0, L"Inject", SW_SHOW);
	InstallPage(&m_InjectDetection, IDD_PAGE_1, L"Injection Detection");
	//InstallPage(&m_RAN, IDD_PAGE_2, L"RAN");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGameHackerDlg::OnOK() {

}

void CGameHackerDlg::OnCancel() {

}

void CGameHackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGameHackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGameHackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



bool CGameHackerDlg::InstallPage(CDialogEx* dlg, DWORD dwPageId, CTabCtrl* pTab,const std::wstring& strPageName, int nCmdShow /*= SW_HIDE*/) {
	if (dlg == nullptr || pTab == nullptr) {
		return false;
	}
	pTab->InsertItem(m_Tab.GetItemCount(), strPageName.c_str());
	m_Pages.push_back({ strPageName.c_str(),dlg });
	dlg->Create(dwPageId, pTab);
	CRect rect;
	pTab->GetClientRect(&rect);
	rect.top += 25;
	dlg->MoveWindow(&rect);
	dlg->ShowWindow(nCmdShow);
	return true;
}

bool CGameHackerDlg::InstallPage(CDialogEx* dlg, DWORD dwPageId, std::wstring&& strPageName /*= L""*/, int nCmdShow /*= SW_HIDE*/) {
	return InstallPage(dlg, dwPageId, &m_Tab, strPageName, nCmdShow);
}



void CGameHackerDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) {
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


void CGameHackerDlg::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);
	
	// TODO: 在此处添加消息处理程序代码
	if (nType == 1)return;
	if (m_Tab.m_hWnd == nullptr)return;
	CRect rect;
	m_Tab.GetClientRect(&rect);
	rect.top += 25;
	for (auto& i : m_Pages)
	{
		i.pDlg->MoveWindow(&rect);
	}
}


void CGameHackerDlg::OnClose() {
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnCancel();
}
