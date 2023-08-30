// CWndAddGame.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "CWndAddGame.h"
#include "afxdialogex.h"
#include "CWndInject.h"

// CWndAddGame 对话框

IMPLEMENT_DYNAMIC(CWndAddGame, CDialogEx)

CWndAddGame::CWndAddGame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_GAME, pParent)
	, m_GameName(_T("")), m_GamePath(_T("")), m_GameFullFile(_T("")), m_cmd(_T("")), m_dllPath(_T("")) {

}

CWndAddGame::~CWndAddGame()
{
}

void CWndAddGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_GameName);
	DDX_Text(pDX, IDC_EDIT2, m_GamePath);
	DDX_Text(pDX, IDC_EDIT3, m_GameFullFile);
	DDX_Text(pDX, IDC_EDIT4, m_cmd);
	DDX_Text(pDX, IDC_EDIT5, m_dllPath);
}


BOOL CWndAddGame::OnInitDialog() {
	CDialogEx::OnInitDialog();
	UpdateData(FALSE);
	return true;
}

BEGIN_MESSAGE_MAP(CWndAddGame, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndAddGame::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWndAddGame::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CWndAddGame::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CWndAddGame::OnBnClickedButton3)
END_MESSAGE_MAP()


// CWndAddGame 消息处理程序


void CWndAddGame::OnBnClickedButton1() {
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CFileDialog fileDlg(TRUE, NULL, NULL, 6, L"exe (*.exe)|*.exe|dat(*.dat)|*.dat|All Files(*.*)|*.*|");
	if (fileDlg.DoModal() == IDOK) {
		m_GameFullFile = fileDlg.GetPathName();
		m_GameName = fileDlg.GetFileTitle();
		m_GamePath = fileDlg.GetFolderPath() + L"\\";
	}
	UpdateData(FALSE);
}


void CWndAddGame::OnBnClickedButton2() {
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CFileDialog fileDlg(TRUE, NULL, NULL, 6, L"Dynamic Link Library(*.dll)|*.dll|");
	if (fileDlg.DoModal() == IDOK) {
		m_dllPath = fileDlg.GetPathName();
	}
	UpdateData(FALSE);
}


void CWndAddGame::OnBnClickedButton4() {
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CWndInject* p = (CWndInject*)father;
	if (m_bIsCreate) {
		p->AddGame(m_GameName, m_GameFullFile, m_GamePath, m_cmd, m_dllPath);
	}
	else {
		p->SaveGame(m_GameName, m_GameFullFile, m_GamePath, m_cmd, m_dllPath);
	}
	CDialogEx::OnOK();
}


void CWndAddGame::OnBnClickedButton3() {
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
