#pragma once
#include "CMEMContext.h"
class COBJContext {
public:
	~COBJContext() {
// 		if (szData) {
// 			delete[]szData;
// 		}
	}
	/*COBJContext(const wchar_t* szIniFilePath);*/
	COBJContext(const wchar_t* folder = nullptr);
	COBJContext(const wchar_t* folder, const wchar_t* _name);
	COBJContext(const wchar_t* folder, const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note);
	BOOL UpdateData(HANDLE _hProcess);
	void Save();
	void SetPath(const wchar_t* folder);
	void SetContext(const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note);
	bool deleteFile() {
		return DeleteFile(strIniPath + L"\\" + strClassName + L".ini");
	}
	CString& GetClassName() {
		return strClassName;
	}
	CString& GetAddress() {
		return strAddress;
	}
	CString& GetNote() {
		return strNote;
	}
	DWORD GetSize() {
		return dwSize;
	}
	LPVOID GetlpAddress() {
		return lpAddress;
	}
	CMEMContextList* GetMemListPtr() {
		return pMemList;
	}
	//PMEMContext pMemContext;
private:
	LPVOID lpAddress{ 0 };
	CString strAddress;
	DWORD dwSize{ 0 };
	CString strClassName;
	CString strNote;
	CString strIniPath;
	CMEMContextList* pMemList;
	//void CreateMEMContext();

};


typedef struct _TREE_DATA {
	DWORD MenuId{ 0 };
	LPVOID DATA_PTR{ 0 };
}TREE_DATA, * PTREE_DATA;

