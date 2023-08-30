#pragma once


// CWndModANY 对话框

class CWndModANY : public CDialogEx
{
	DECLARE_DYNAMIC(CWndModANY)

public:
	CWndModANY(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndModANY();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOD_ANIY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ModList;
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
};
