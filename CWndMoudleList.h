#pragma once


// CWndMoudleList 对话框

class CWndMoudleList : public CDialogEx
{
	DECLARE_DYNAMIC(CWndMoudleList)

public:
	CWndMoudleList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndMoudleList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOD_List };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	BOOL GetMoudleList(DWORD dwProcessId);
public:
	CListCtrl m_ModList;
};
