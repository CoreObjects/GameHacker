#pragma once

#include "CWndProcessList.h"
#include "CWndMoudleList.h"
#include "CWndModANY.h"
// CWndInjectDetection 对话框

class CWndInjectDetection : public CDialogEx
{
	DECLARE_DYNAMIC(CWndInjectDetection)
	typedef struct _TAB_Name {
		CString strName;
		CDialogEx* pDlg;
	}dlgName;
public:
	CWndInjectDetection(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndInjectDetection();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	bool InstallPage(CDialogEx* dlg, DWORD dwPageId, CTabCtrl* pTab, const std::wstring& strPageName, int nCmdShow /*= SW_HIDE*/);
	bool InstallPage(CDialogEx* dlg, DWORD dwPageId, std::wstring&& strPageName = L"", int nCmdShow = SW_HIDE);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CWndProcessList m_ProcessDlg;
	DWORD m_ProcessId;
	DWORD m_Inf_ProcessId;
	CWndMoudleList m_NomalMoudleDlg;
	CWndMoudleList m_InfectMoudleDlg;
	CWndModANY m_ModeAnalysisDlg;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString m_ProcessName;
	CString m_Inf_ProcessName;
	CTabCtrl m_Tab;
	std::vector<dlgName> m_Pages;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	void CompareCListCtrls(CListCtrl& listCtrl1, CListCtrl& listCtrl2, CListCtrl& listCtrlResult);
	afx_msg void OnBnClickedButton5();
};
