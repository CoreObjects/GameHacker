#pragma once

#include "CWndAddGame.h"
// CWndInject 对话框
#include "x86EntryPointInject.h"
class CWndInject : public CDialogEx
{
	DECLARE_DYNAMIC(CWndInject)

public:
	CWndInject(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndInject();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_0 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ExeList;
	BOOL m_bIsInject;
	BOOL m_isSuspend;
	BOOL m_bIsDebug;
public:
	CWndAddGame m_AddGame;
	CString m_strAppPath;
	CString m_strGameIniPath;
	void Init(CString& _AppPath);
	void AddGame(CString& _GameName, CString& _GameExe, CString& _GamePath, CString& _GameCmds, CString& _DllPath);
	void SaveGame(CString& _GameName, CString& _GameExe, CString& _GamePath, CString& _GameCmds, CString& _DllPath);

	void LoadGame();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);

	void Start();
	void SaveLstToIni();
	x86EntryPointInject m_Injector;
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnStart();
	afx_msg void OnSet();
	afx_msg void OnDelete();
	afx_msg void OnFlush();
};
