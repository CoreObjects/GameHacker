// CWndAddClass.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "CWndAddClass.h"
#include "afxdialogex.h"


// CWndAddClass 对话框

IMPLEMENT_DYNAMIC(CWndAddClass, CDialogEx)

CWndAddClass::CWndAddClass(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SET_OBJECT, pParent)
	, m_ClassName(_T("")), m_Note(_T("")) ,m_Size(0),m_Address(L"") {

}

CWndAddClass::~CWndAddClass()
{
}

void CWndAddClass::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Address);
	DDX_Text(pDX, IDC_EDIT2, m_Size);
	DDX_Text(pDX, IDC_EDIT3, m_ClassName);
	DDX_Text(pDX, IDC_EDIT4, m_Note);
}


BOOL CWndAddClass::OnInitDialog() {
	CDialogEx::OnInitDialog();
	UpdateData(FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWndAddClass, CDialogEx)
	
	ON_BN_CLICKED(IDOK, &CWndAddClass::OnBnClickedOk)
END_MESSAGE_MAP()


// CWndAddClass 消息处理程序



void CWndAddClass::OnBnClickedOk() {
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();

}
