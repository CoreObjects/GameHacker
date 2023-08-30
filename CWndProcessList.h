#pragma once


// CWndProcessList 对话框

class CWndProcessList : public CDialogEx
{
	DECLARE_DYNAMIC(CWndProcessList)

public:
	CWndProcessList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndProcessList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPEN_PROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	void Flush();
	void SearchByProcessName(const CString& targetName);
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
public:
	CString strProcessName;
	CString strProcessId;
public:
	CListCtrl m_ProcessList;
	afx_msg void OnEnChangeEdit1();
	CString m_ProcessName;
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
};
