#pragma once

//#include "CWndInject.h"
// CWndAddGame 对话框

class CWndAddGame : public CDialogEx {
	DECLARE_DYNAMIC(CWndAddGame)

public:
	CWndAddGame(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndAddGame();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_GAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_GameName;
	CString m_GamePath;
	CString m_GameFullFile;
	CString m_cmd;
	CString m_dllPath;
	void* father;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
public:
	bool m_bIsCreate;
	void Init(void* _father,
		bool bIsCreate = true,
		CString GameName = L"",
		CString GamePath = L"",
		CString GameFullFile = L"",
		CString cmd = L"",
		CString dllPath = L"") {

		father = _father;
		m_bIsCreate = bIsCreate;
		m_GameName = GameName;
		m_GamePath = GamePath;
		m_GameFullFile = GameFullFile;
		m_cmd = cmd;
		m_dllPath = dllPath;
	}
};
