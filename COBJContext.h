#pragma once
class COBJContext {
public:
	~COBJContext() {
		if (szData) {
			delete[]szData;
		}
	}

	COBJContext() {
		lpAddress = NULL;
		dwSize = 0;
		szData = 0;
	}
	/*COBJContext(const wchar_t* szIniFilePath);*/
	COBJContext(const wchar_t* folder, const wchar_t* _name);
	COBJContext(const wchar_t* folder, const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note);
	BOOL UptateData(HANDLE _hProcess);
	void Save();
	void Set(const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note,bool bIsSet = true);
	void deleteFile() {
		DeleteFile(strIniFile);
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
private:
	LPVOID lpAddress{ 0 };
	CString strAddress;
	DWORD dwSize{ 0 };
	CString strClassName;
	CString strNote;
	CString strIniFile;
	CString strIniPath;
	char* szData{ 0 };
};


typedef struct _TREE_DATA {
	DWORD MenuId{ 0 };
	LPVOID DATA_PTR{ 0 };
}TREE_DATA, * PTREE_DATA;

////strNote
//class strClassName {
//
//
//};