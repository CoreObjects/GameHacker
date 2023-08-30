#include "pch.h"
#include "COBJContext.h"

COBJContext::COBJContext(const wchar_t* folder, const wchar_t* _name) {
	lpAddress = nullptr;
	strClassName = _name;
	strIniPath = folder;
	szData = nullptr;
	CString strIniFile = strIniPath + L"\\" + _name + L".ini";
	wchar_t _wRead[MAX_PATH]{ 0 };
	GetPrivateProfileString(L"main", L"address", L"0", _wRead, 0xff, strIniFile);
	strAddress.Format(L"%s", _wRead);
	GetPrivateProfileString(L"main", L"note", L"", _wRead, 0xff, strIniFile);
	strNote.Format(L"%s", _wRead);
	dwSize = GetPrivateProfileInt(L"main", L"size", 0, strIniFile);
	szData = new char[dwSize] {0};

}

COBJContext::COBJContext(const wchar_t* folder, const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note) {
	lpAddress = nullptr;
	strAddress = _address;
	dwSize = _size;
	strClassName = _name;
	strNote = note;
	strIniPath = folder;
	szData = new char[dwSize];
}

COBJContext::COBJContext(const wchar_t* folder /*= nullptr*/) {
	lpAddress = nullptr;
	strAddress = L"";
	dwSize = 0;
	strClassName = L"";
	strNote = L"";
	strIniPath = folder;
	szData = nullptr;
}

BOOL COBJContext::UptateData(HANDLE _hProcess) {
	return ReadProcessMemory(_hProcess, lpAddress, szData, dwSize, NULL);
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
	
	strClassName = _name;
	lpAddress = (LPVOID)wcstoul(_address, 0, 16);
	strAddress = _address;
	dwSize = _size;
	strNote = note;
	if (szData != nullptr)
		delete szData;
	szData = new char[dwSize];
}
