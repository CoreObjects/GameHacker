
// GameHackerDlg.h: 头文件
//

#pragma once
#include "CWndInject.h"
#include "CWndInjectDetection.h"
#include "CWndAddGame.h"
#include "CWndRAN.h"

// CGameHackerDlg 对话框
class CGameHackerDlg : public CDialogEx
{
private:
	typedef struct _TAB_Name {
		CString strName;
		CDialogEx* pDlg;
	}dlgName;
// 构造
public:
	CGameHackerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMEHACKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	bool InstallPage(CDialogEx* dlg, DWORD dwPageId, CTabCtrl* pTab,const std::wstring& strPageName = L"", int nCmdShow = SW_HIDE);
	bool InstallPage(CDialogEx* dlg, DWORD dwPageId, std::wstring&& strPageName = L"", int nCmdShow = SW_HIDE);
private:
	std::vector<dlgName> m_Pages;
	CString strAppPath;
public:
	CTabCtrl m_Tab;
	CWndInject m_Inject;
	CWndInjectDetection m_InjectDetection;
	CWndRAN m_RAN;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};
