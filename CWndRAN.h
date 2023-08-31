#pragma once


// CWndRAN 对话框
#include "CWndAddClass.h"
#include "CWndProcessList.h"
#include "COBJContext.h"
class CWndRAN : public CDialogEx {
	DECLARE_DYNAMIC(CWndRAN)

public:
	CWndRAN(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndRAN();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void SetAppPath(CString _strAppPath) {
		strAppPath = _strAppPath;
		strAnlyPath = strAppPath + L"\\RAN\\";
		DWORD_PTR;
	}
	HTREEITEM InsertItemEx(const wchar_t* szTxt, DWORD dwMenuId, LPVOID lpData, HTREEITEM h = TVI_ROOT) {
		auto _rt = m_Tree.InsertItem(szTxt, h);
		PTREE_DATA DATA_PTR = new TREE_DATA{ dwMenuId, lpData };
		m_Tree.SetItemData(_rt, (DWORD_PTR)DATA_PTR);
		return _rt;
	}
	void DeleteItemEx(HTREEITEM h) {
		PTREE_DATA Val = (PTREE_DATA)m_Tree.GetItemData(h);
		if (Val) {
			COBJContext* p = (COBJContext*)Val->DATA_PTR;
			if (p) {
				p->deleteFile();
				delete p;
			}
			delete Val;
		}
		m_Tree.DeleteItem(h);
	}
	HTREEITEM GetFirstItem(HTREEITEM h) {
		HTREEITEM hRoot = h;
		// 检查h是否为空
		if (h == NULL) {
			return NULL;
		}
		// 循环找到根节点
		while (m_Tree.GetParentItem(hRoot) != NULL) {
			hRoot = m_Tree.GetParentItem(hRoot);
		}
		return hRoot;
	}
public:
	CWndAddClass m_AddClass;
	CWndProcessList m_SelectProcess;
	CTreeCtrl m_Tree;
	CListCtrl m_DataList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLoadGame();

	HANDLE hProcess;
	CString strAppPath;//E:\GameHacker\GameHacker\Release
	CString strAnlyPath;//E:\GameHacker\GameHacker\Release\RAN\

	CString strAnlyData;//E:\GameHacker\GameHacker\Release\RAN\GameHacker
	afx_msg void OnAddClass();

	void InsertOBJContext(COBJContext* obj);

	afx_msg void OnDeleteClass();
	afx_msg void OnSetClass();

	COBJContext* GetOBJPtr(HTREEITEM h);
	COBJContext* GetSelectOBJPtr();

	HTREEITEM GetSelectRootItem() {
		auto hsel = m_Tree.GetSelectedItem();
		auto hRoot = m_Tree.GetRootItem();
		if (hsel == hRoot) return nullptr;
		auto h = GetFirstItem(hsel);
		return h;
	}

	afx_msg void OnTimer(UINT_PTR nIDEvent) {
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (hProcess == NULL) {
			return CDialogEx::OnTimer(nIDEvent);
		}
		if (pCurObj == nullptr) {
			return CDialogEx::OnTimer(nIDEvent);
		}
		if (pCurObj->UpdateData(hProcess) == FALSE) {
			hProcess = nullptr;
			AfxMessageBox(L"游戏加载失败");
		}
		ShowMemObjContext();
		CDialogEx::OnTimer(nIDEvent);
	}

	void ShowMemObjContext() {
//		m_DataList.DeleteAllItems();
		static bool bIsFirst = true;
		int nIndex = 0;
		CMEMContextList* pMemList = pCurObj->GetMemListPtr();
		PCMEMContext pMemHeader = pMemList->m_pHeader;
// 		m_DataList.InsertColumn(0, L"VA", 0, 100);
// 		m_DataList.InsertColumn(1, L"Offset", 0, 100);
// 		m_DataList.InsertColumn(2, L"Type", 0, 100);
// 		m_DataList.InsertColumn(3, L"Name", 0, 100);
// 		m_DataList.InsertColumn(4, L"Value", 0, 100);
// 		m_DataList.InsertColumn(5, L"Comment", 0, 100);
		if (bIsFirst) {
			while (pMemHeader) {
				for (int i = 0; i < pMemHeader->dwLength; i++) {
					m_DataList.InsertItem(i, pCurObj->GetAddress());
					DWORD dwOffset = pMemHeader->dwOffset + i * pMemHeader->pDataType->size;
					CString strOffset; strOffset.Format(L"%d", dwOffset);
					m_DataList.SetItemText(i, 1, strOffset);
					m_DataList.SetItemText(i, 2, pMemHeader->pDataType->UName);
					m_DataList.SetItemText(i, 3, pMemHeader->strValName);

					m_DataList.SetItemText(i, 4, pMemList->ReadVal(pMemHeader, i));
					m_DataList.SetItemText(i, 5, pMemHeader->strNote);
				}
				pMemHeader = pMemHeader->next;
			}
		}
		else {
			while (pMemHeader) {
				for (int i = 0; i < pMemHeader->dwLength; i++) {
					m_DataList.SetItemText(i, 0, pCurObj->GetAddress());
					DWORD dwOffset = pMemHeader->dwOffset + i * pMemHeader->pDataType->size;
					CString strOffset; strOffset.Format(L"%d", dwOffset);
					m_DataList.SetItemText(i, 1, strOffset);
					m_DataList.SetItemText(i, 2, pMemHeader->pDataType->UName);
					m_DataList.SetItemText(i, 3, pMemHeader->strValName);

					m_DataList.SetItemText(i, 4, pMemList->ReadVal(pMemHeader, i));
					m_DataList.SetItemText(i, 5, pMemHeader->strNote);
				}
				pMemHeader = pMemHeader->next;
			}
		}
		
	}
	COBJContext* pCurObj;
	afx_msg void OnAnalysisOBJ();
};
