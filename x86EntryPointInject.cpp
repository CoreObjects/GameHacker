#include "pch.h"
#include "x86EntryPointInject.h"

x86EntryPointInject::x86EntryPointInject() {
	RtlZeroMemory(&pi, sizeof(pi));
	RtlZeroMemory(&stcRemoteData, sizeof(stcRemoteData));
	lEntryPoint = 0;
	lpRemoteData = nullptr;
	lpRemoteCode = nullptr;
}

x86EntryPointInject::~x86EntryPointInject() {
	if (pi.hProcess != nullptr) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		RtlZeroMemory(&pi, sizeof(pi));
	}
}

BOOL x86EntryPointInject::CreateRemoteCode() {
	if (pi.hProcess == NULL || lpRemoteData == nullptr) return FALSE;
	char code[0x200];
	memcpy(code, INJ_Code, sizeof(code));
	for (int i = 0; i < 0x100; i++) {
		unsigned* p = (unsigned*)(code + i);
		if (*p == 0XCCCCCCCC) {
			*p = (unsigned)lpRemoteData;
			break;
		}
	}
	stcRemoteData.addrINJCode = lpRemoteCode;
	WriteProcessMemory(pi.hProcess, stcRemoteData.addrINJCode, code, 0x200, NULL);
	stcRemoteData.addrHookCode = (LPVOID)((DWORD)lpRemoteCode + 0x300);
	WriteProcessMemory(pi.hProcess, stcRemoteData.addrHookCode, HOOK_Code, 0x200, NULL);
	stcRemoteData.addrINJCode_RemoteThread = (LPVOID)((DWORD)lpRemoteCode + 0x600);
	memcpy(code, INJ_Code_RemoteThread, sizeof(code));
	for (int i = 0; i < 0x100; i++) {
		unsigned* p = (unsigned*)(code + i);
		if (*p == 0XCCCCCCCC) {
			*p = (unsigned)lpRemoteData;
			break;
		}
	}
	WriteProcessMemory(pi.hProcess, stcRemoteData.addrINJCode_RemoteThread, code, 0x200, NULL);
	return TRUE;
}

BOOL x86EntryPointInject::CreateRemoteData(const char* szDllName) {
	if (pi.hProcess == NULL) return FALSE;
	RtlCopyMemory(stcRemoteData.szDllName, szDllName, strlen(szDllName));
	HMODULE hKernel32 = LoadLibrary(L"kernel32.dll");
	if (hKernel32 == NULL)return FALSE;
	HMODULE hUser32 = LoadLibrary(L"user32.dll");
	if (hUser32 == NULL) return FALSE;
	stcRemoteData.fnLoadLibrary = (decltype(LoadLibraryA)*)GetProcAddress(hKernel32, "LoadLibraryA");
	stcRemoteData.fnGetModuleHandle = (decltype(GetModuleHandleA)*)GetProcAddress(hKernel32, "GetModuleHandleA");
	stcRemoteData.fnVirtualProtect = (decltype(VirtualProtect)*)GetProcAddress(hKernel32, "VirtualProtect");
	stcRemoteData.fnMessageBox = (decltype(MessageBoxA)*)GetProcAddress(hUser32, "MessageBoxA");
	stcRemoteData.EntryPoint = lEntryPoint;
	WriteProcessMemory(pi.hProcess, lpRemoteData, &stcRemoteData, sizeof(_REMOTE_DATA), NULL);
	return TRUE;
}

BOOL x86EntryPointInject::startProcess(wchar_t* GameExe, wchar_t* GamePath, wchar_t* cmd) {
	STARTUPINFO si{ 0 };
	si.cb = sizeof(si);
	BOOL bRet = CreateProcess(GameExe, cmd, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, GamePath, &si, &pi);
	if (bRet)ResumeThread(pi.hThread);
	return bRet;
}

BOOL x86EntryPointInject::startProcessWithInject(wchar_t* GameExe, wchar_t* GamePath, wchar_t* cmd, char* szDllPath) {
	//1.启动线程，并暂停
	//2.申请远程内存空间(数据区，代码区)
	//3.往远程内存写代码
	//3.1构建远程数据
	//3.2构建远程代码
	//4 执行远程代码
	// 执行流程：
	//4.1.使用远程线程得到基址与注入点，对注入点进行hook
	//4.2，等待远程线程结束后，恢复主线程
	//4.3,因为入口点已经hook，跳转到INC_Code;
	//4.4,INC_Code加载Dll，并恢复入口点和跳转回入口点。
	STARTUPINFO si{ 0 };
	si.cb = sizeof(si);
	BOOL bRet = CreateProcess(GameExe, cmd, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, GamePath, &si, &pi);
	if (bRet == FALSE) {
		Tool::OutputDebugPrintf("启动游戏失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	stcRemoteData.EntryPoint = GetEntryPoint(GameExe);
	if (stcRemoteData.EntryPoint == -1) {
		Tool::OutputDebugPrintf("获取入口点失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	if (AllocCodeMemory() == nullptr || AllocDataMemory() == nullptr) {
		Tool::OutputDebugPrintf("申请远程内存失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	if (FALSE == CreateRemoteCode()) {
		Tool::OutputDebugPrintf("注入远程代码失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	if (FALSE == CreateRemoteData(szDllPath)) {
		Tool::OutputDebugPrintf("注入远程数据失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)stcRemoteData.addrHookCode, lpRemoteData, 0, NULL);
	/*stcRemoteData.fnMessageBox(0, 0, 0, 0);*/
	if (hThread == NULL) {
		Tool::OutputDebugPrintf("启动远程线程失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	
	WaitForSingleObject(hThread, -1);
	CloseHandle(hThread);
	if (bRet) {
		ResumeThread(pi.hThread);
	}
	return bRet;
}

BOOL x86EntryPointInject::startProcessWithRemoteThreadInject(wchar_t* GameExe, wchar_t* GamePath, wchar_t* cmd, char* szDllPath) {
	STARTUPINFO si{ 0 };
	si.cb = sizeof(si);
	BOOL bRet = CreateProcess(GameExe, cmd, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, GamePath, &si, &pi);
	if (bRet == FALSE) {
		Tool::OutputDebugPrintf("启动游戏失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	if (AllocCodeMemory() == nullptr || AllocDataMemory() == nullptr) {
		Tool::OutputDebugPrintf("申请远程内存失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	if (FALSE == CreateRemoteCode()) {
		Tool::OutputDebugPrintf("注入远程代码失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	if (FALSE == CreateRemoteData(szDllPath)) {
		Tool::OutputDebugPrintf("注入远程数据失败,错误码：%d\r\n", GetLastError());
		return FALSE;
	}
	HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)stcRemoteData.addrINJCode_RemoteThread, lpRemoteData, 0, NULL);
	ResumeThread(pi.hThread);
	return TRUE;
}

LONG_PTR x86EntryPointInject::GetEntryPoint(const wchar_t* szFilePath) {
	HANDLE hFile = CreateFileW(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)return -1;
	DWORD dwLength = GetFileSize(hFile, NULL);
	char* szFileBuffer = new char[dwLength] {0};
	if (ReadFile(hFile, szFileBuffer, dwLength, &dwLength, NULL) == TRUE) {
		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)szFileBuffer;
		PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pDosHeader->e_lfanew + szFileBuffer);
		lEntryPoint = pNtHeader->OptionalHeader.AddressOfEntryPoint;
		delete[] szFileBuffer;
		CloseHandle(hFile);
		return lEntryPoint;
	}
	return -1;
}

DWORD x86EntryPointInject::GetFunSize(void* lpFunStartAddr) {
	DWORD dwOld;
	VirtualProtect(lpFunStartAddr, 0x1000, PAGE_EXECUTE_READWRITE, &dwOld);
	unsigned char* pStartAddr = (unsigned char*)lpFunStartAddr;
	DWORD dwFunSize = 0;
	while (*pStartAddr != 0xC3 && *pStartAddr != 0xC2 && *pStartAddr != 0xCB && *pStartAddr != 0xCA) {
		pStartAddr++;
		dwFunSize++;
		if (dwFunSize == 0x1000)
			return -1;
	}
	VirtualProtect(lpFunStartAddr, 0x1000, dwOld, &dwOld);
	return dwFunSize;
}

void WINAPI x86EntryPointInject::INJ_Code() {
	REMOTEDATA* pData = (REMOTEDATA*)0xCCCCCCCC;
	pData->hDllBase = pData->fnLoadLibrary(pData->szDllName);
	unsigned dEntry = (unsigned)pData->EntryPoint;
	char* EntryCode = (char*)pData->EntryPoint;
	memcpy(EntryCode, pData->OldEntry, 0x05);
	pData->fnVirtualProtect((LPVOID)pData->EntryPoint, 5, pData->dwOldProtect, &pData->dwOldProtect);
	_asm {
		mov eax, dEntry
		jmp eax;
	}
}

void WINAPI x86EntryPointInject::INJ_Code_RemoteThread() {
	REMOTEDATA* pData = (REMOTEDATA*)0xCCCCCCCC;
	pData->hDllBase = pData->fnLoadLibrary(pData->szDllName);
}


DWORD WINAPI x86EntryPointInject::HOOK_Code(LPVOID arg) {
	PREMOTEDATA pData = (PREMOTEDATA)arg;
	pData->EntryPoint += (LONG_PTR)(pData->fnGetModuleHandle(NULL));
	pData->fnVirtualProtect((LPVOID)pData->EntryPoint, 5, PAGE_EXECUTE_READWRITE, &pData->dwOldProtect);
	memcpy(pData->OldEntry, (LPVOID)pData->EntryPoint, 5);
	((unsigned char*)pData->EntryPoint)[0] = (unsigned char)0xE9;
	((DWORD*)(pData->EntryPoint + 1))[0] = (DWORD)(pData->addrINJCode) - (pData->EntryPoint + 5);
	return pData->EntryPoint;
}
