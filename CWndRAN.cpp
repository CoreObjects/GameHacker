// CWndRAN.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "CWndRAN.h"
#include "afxdialogex.h"


// CWndRAN 对话框

IMPLEMENT_DYNAMIC(CWndRAN, CDialogEx)

CWndRAN::CWndRAN(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_2, pParent) {
	hProcess = nullptr;
}

CWndRAN::~CWndRAN() {
}

void CWndRAN::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_DataList);
}


BOOL CWndRAN::OnInitDialog() {
	CDialogEx::OnInitDialog();
	LONG_PTR lStyle = GetWindowLongPtr(m_DataList.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(m_DataList.m_hWnd, GWL_STYLE, lStyle);
	m_DataList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_DataList.InsertColumn(0, L"VA", 0, 100);
	m_DataList.InsertColumn(1, L"Offset", 0, 100);
	m_DataList.InsertColumn(2, L"Type", 0, 100);
	m_DataList.InsertColumn(3, L"Name", 0, 100);
	m_DataList.InsertColumn(4, L"Value", 0, 100);
	m_DataList.InsertColumn(5, L"Comment", 0, 100);
	return FALSE;
}

BEGIN_MESSAGE_MAP(CWndRAN, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndRAN::OnBnClickedButton1)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32780, &CWndRAN::OnLoadGame)
	ON_COMMAND(ID_32779, &CWndRAN::OnAddClass)
	ON_COMMAND(ID_32790, &CWndRAN::OnDeleteClass)
	ON_COMMAND(ID_32791, &CWndRAN::OnSetClass)
END_MESSAGE_MAP()


// CWndRAN 消息处理程序


void CWndRAN::OnBnClickedButton1() {
	// TODO: 在此添加控件通知处理程序代码
	auto ite = m_Tree.InsertItem(L"剑侠", TVI_ROOT);
	auto jData = m_Tree.InsertItem(L"数据", ite);
	// 
	// 	m_Tree.GetRootItem();
	// 
	// 	//m_Tree.GetNextSiblingItem()
	// 	
	// 	m_Tree.DeleteItem(ite);
}


void CWndRAN::OnContextMenu(CWnd* pWnd, CPoint point) {
	// TODO: 在此处添加消息处理程序代码



	if (pWnd->GetDlgCtrlID() == IDC_TREE1) {
		DWORD dwMenuID = 1;
		if (hProcess) {
			auto hSel = m_Tree.GetSelectedItem();
			if (hSel) {
				PTREE_DATA val = (PTREE_DATA)m_Tree.GetItemData(hSel);
				if (val)dwMenuID = val->MenuId;
				else dwMenuID = 0;
			}
			else dwMenuID = 0;

		}
		if (dwMenuID == 0)return;
		// 加载菜单资源
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CMenu* pSubMenu = menu.GetSubMenu(dwMenuID);

		pSubMenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this); // 弹出菜单
	}
}


void CWndRAN::OnLoadGame() {
	// TODO: 在此添加命令处理程序代码
	if (m_SelectProcess.DoModal() == IDOK) {

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, std::stoi(m_SelectProcess.strProcessId.GetBuffer()));

		CString strProcess = m_SelectProcess.strProcessName;
		strProcess.Replace(L".exe", L"");
		auto tRoot = InsertItemEx(strProcess, 2, NULL);

		InsertItemEx(L"Public Data", 3, NULL, tRoot);
		InsertItemEx(L"Public Function", 4, NULL, tRoot);
		InsertItemEx(L"Hook", 5, NULL, tRoot);

		strAnlyData = strAnlyPath + m_SelectProcess.strProcessName;
		strAnlyData.Replace(L".exe", L"");
		if (!PathIsDirectory(strAnlyData)) {
			if (CreateDirectory(strAnlyData, NULL) == FALSE) {
				AfxMessageBox(L"创建文件夹失败!");
				return;
			}
		}
		WIN32_FIND_DATA fileInfo;
		HANDLE hFilefind = FindFirstFile(strAnlyData + L"\\*.ini", &fileInfo);
		while (hFilefind != INVALID_HANDLE_VALUE) {
			CString _Name = fileInfo.cFileName;
			_Name.Replace(L".ini", L"");
			COBJContext* obj = new COBJContext(strAnlyData, _Name);
			InsertOBJContext(obj);
			if (!FindNextFile(hFilefind, &fileInfo))break;
		}FindClose(hFilefind);

	}
}


void CWndRAN::OnAddClass() {
	// TODO: 在此添加命令处理程序代码
	if (!hProcess) {
		return;
	}
	if (m_AddClass.DoModal() == IDOK) {

		auto h = m_Tree.GetRootItem();
		while (h) {
			h = m_Tree.GetNextSiblingItem(h);
			auto txt = m_Tree.GetItemText(h);
			if (txt == m_AddClass.m_ClassName) {
				AfxMessageBox(L"该对象已经存在，请勿重复添加");
				return;
			}
		}
		COBJContext* obj = new COBJContext(strAnlyData, m_AddClass.m_ClassName, \
			m_AddClass.m_Address, m_AddClass.m_Size, m_AddClass.m_Note);
		obj->Save();

		InsertOBJContext(obj);

	}
}


void CWndRAN::InsertOBJContext(COBJContext* obj) {
	auto hRoot = InsertItemEx(obj->GetClassName(), 6, obj);
	auto hBase = InsertItemEx(L"基本信息", 6, NULL, hRoot);
	CString strVal;
	strVal = L"内存地址：" + obj->GetAddress();
	InsertItemEx(strVal, 6, NULL, hBase);
	strVal.Format(L"对象大小: [%d]", obj->GetSize());
	InsertItemEx(strVal, 6, NULL, hBase);
	strVal = L"注    释:" + obj->GetNote();
	InsertItemEx(strVal, 6, NULL, hBase);
	InsertItemEx(L"成员变量", 6, NULL, hRoot);
	InsertItemEx(L"成员函数", 6, NULL, hRoot);
}

void CWndRAN::OnDeleteClass() {
	// TODO: 在此添加命令处理程序代码
	auto h = GetSelectRootItem();
	DeleteItemEx(h);

}


void CWndRAN::OnSetClass() {
	// TODO: 在此添加命令处理程序代码
	auto h = GetSelectRootItem();
	COBJContext* obj = GetOBJPtr(h);
	m_AddClass.m_Address = obj->GetAddress();
	m_AddClass.m_Note = obj->GetNote();
	m_AddClass.m_Size = obj->GetSize();
	m_AddClass.m_ClassName = obj->GetClassName();
	if (m_AddClass.DoModal() == IDOK) {
		DeleteItemEx(h);
		obj = new COBJContext(strAnlyData, m_AddClass.m_ClassName, \
			m_AddClass.m_Address, m_AddClass.m_Size, m_AddClass.m_Note);
		obj->Save();
		InsertOBJContext(obj);
	}
}
COBJContext* CWndRAN::GetOBJPtr(HTREEITEM h) {
	if (h) {
		PTREE_DATA val = (PTREE_DATA)m_Tree.GetItemData(h);
		if (val) {
			COBJContext* obj = (COBJContext*)val->DATA_PTR;
			return obj;
		}
	}
	return nullptr;
}
COBJContext* CWndRAN::GetSelectOBJPtr() {
	auto h = GetSelectRootItem();
	if (h) {
		PTREE_DATA val = (PTREE_DATA)m_Tree.GetItemData(h);
		if (val) {
			COBJContext* obj = (COBJContext*)val->DATA_PTR;
			return obj;
		}
	}
	return nullptr;
}
