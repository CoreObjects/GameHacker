#include "pch.h"
#include "COBJContext.h"

COBJContext::COBJContext(const wchar_t* folder, const wchar_t* _name) {

	lpAddress = NULL;
	dwSize = 0;
	szData = 0;
	strIniPath = folder;
	strIniFile = strIniPath + L"\\" + _name + L".ini";
	wchar_t _address[MAX_PATH]{ 0 };
	wchar_t _note[MAX_PATH]{ 0 };
	GetPrivateProfileString(L"main", L"address", L"0", _address, 0xff, strIniFile);

	GetPrivateProfileString(L"main", L"note", L"", _note, 0xff, strIniFile);

   DWORD _dwSize = GetPrivateProfileInt(L"main", L"size", 0, strIniFile);

   Set(_name, _address, _dwSize, _note, false);
}

COBJContext::COBJContext(const wchar_t* folder, const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note) {
	strIniPath = folder;
	strIniFile = strIniPath + L"\\" + _name + L".ini";
	Set(_name, _address, _size, note, false);
}

BOOL COBJContext::UptateData(HANDLE _hProcess) {
	return ReadProcessMemory(_hProcess, lpAddress, szData, dwSize, NULL);
}

void COBJContext::Save() {
	//保存数据到文件
	CString strSize;
	strSize.Format(L"%d", dwSize);
	WritePrivateProfileString(L"main", L"address", strAddress, strIniFile);
	WritePrivateProfileString(L"main", L"size", strSize, strIniFile);
	WritePrivateProfileString(L"main", L"note", strNote, strIniFile);

}

void COBJContext::Set(const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* note, bool bIsSet) {
	
	CString _strName;
	_strName.Format(L"%s", _name);//名称改变
	strAddress.Format(L"%s", _address);
	strNote.Format(L"%s", note);
	if (dwSize < _size) {
		dwSize = _size;
		if (szData != nullptr) delete[]szData;
		szData = new char[dwSize];
	}
	lpAddress = (LPVOID)wcstoul(_address, 0, 16);

	if (_strName != strClassName && bIsSet == true) {
		DeleteFile(strIniFile);
	}
	strClassName = _strName;
	strIniFile = strIniPath + L"\\" + _name + L".ini";
	Save();
}
