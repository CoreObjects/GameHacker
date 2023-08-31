#include "pch.h"
#include "COBJContext.h"

COBJContext::COBJContext(const wchar_t* folder, const wchar_t* _name) {
	lpAddress = nullptr;
	strClassName = _name;
	strIniPath = folder;
	CString strIniFile = strIniPath + L"\\" + _name + L".ini";
	wchar_t _wRead[MAX_PATH]{ 0 };
	GetPrivateProfileString(L"main", L"address", L"0", _wRead, 0xff, strIniFile);
	strAddress.Format(L"%s", _wRead);
	GetPrivateProfileString(L"main", L"note", L"", _wRead, 0xff, strIniFile);
	strNote.Format(L"%s", _wRead);
	dwSize = GetPrivateProfileInt(L"main", L"size", 0, strIniFile);
	lpAddress = (LPVOID)wcstoul(strAddress.GetBuffer(), 0, 16);
	pMemList = 0;
}

COBJContext::COBJContext(const wchar_t* folder, const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note) {
	lpAddress = (LPVOID)wcstoul(_address, 0, 16);
	strAddress = _address;
	dwSize = _size;
	strClassName = _name;
	strNote = note;
	strIniPath = folder;
	pMemList = 0;
}

COBJContext::COBJContext(const wchar_t* folder /*= nullptr*/) {
	lpAddress = nullptr;
	strAddress = L"";
	dwSize = 0;
	strClassName = L"";
	strNote = L"";
	strIniPath = folder;
	pMemList = 0;
}

BOOL COBJContext::UpdateData(HANDLE _hProcess) {
	char* szData = new char[dwSize] {0};
	if (pMemList != nullptr) {
		delete pMemList;
		pMemList = nullptr;
	}
	BOOL bRet = ReadProcessMemory(_hProcess, lpAddress,szData, dwSize, NULL);
	pMemList = new CMEMContextList(szData, dwSize);
	delete[] szData;
	return bRet;
}

void COBJContext::Save() {
	//保存数据到文件
	CString strSize;
	strSize.Format(L"%d", dwSize);
	CString strIniFile = strIniPath + L"\\" + strClassName + L".ini";
	WritePrivateProfileString(L"main", L"address", strAddress, strIniFile);
	WritePrivateProfileString(L"main", L"size", strSize, strIniFile);
	WritePrivateProfileString(L"main", L"note", strNote, strIniFile);
}

void COBJContext::SetPath(const wchar_t* folder) {
	strIniPath = folder;
}

void COBJContext::SetContext(const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note) {
	pMemList = 0;
	strClassName = _name;
	lpAddress = (LPVOID)wcstoul(_address, 0, 16);
	strAddress = _address;
	dwSize = _size;
	strNote = note;
}

// void COBJContext::CreateMEMContext() {
// 	PMEMContext _mem1{}; 
// 	int v = dwSize % 4;
// 	int s = dwSize / 4;//
// 	if (s > 0) {
// 		pMemContext = new CMEMContext(T_INT, s, L"Unknown", L"Unknown", nullptr);
// 	}
// 	switch (v) {
// 	case 1:
// 		_mem1 = new CMEMContext(T_BOOL,1,L"unknown",L"unknown", pMemContext);
// 		break;
// 	case 2:
// 		_mem1 = new CMEMContext(T_SHORT, 1, L"unknown", L"unknown", pMemContext);
// 		break;
// 	case 3:
// 		_mem1 = new CMEMContext(T_SHORT, 1, L"unknown", L"unknown", pMemContext);
// 		_mem1->next = new CMEMContext(T_BOOL, 1, L"unknown", L"unknown", pMemContext);
// 		break;
// 	}
// 	if (pMemContext == nullptr) pMemContext = _mem1;
// 	else
// 		pMemContext -> next = _mem1;
// }
