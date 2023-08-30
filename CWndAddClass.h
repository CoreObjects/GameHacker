#pragma once


// CWndAddClass 对话框

class CWndAddClass : public CDialogEx
{
	DECLARE_DYNAMIC(CWndAddClass)

public:
	CWndAddClass(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndAddClass();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedOk();
	CString m_Address;
	unsigned m_Size;
	CString m_ClassName;
	CString m_Note;
	afx_msg void OnBnClickedOk();
};
