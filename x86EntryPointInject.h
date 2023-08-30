#pragma once
#include "Tool.h"
typedef struct _REMOTE_DATA {
	char szDllName[0x100];
	decltype(LoadLibraryA)* fnLoadLibrary;
	decltype(GetModuleHandleA)* fnGetModuleHandle;
	decltype(VirtualProtect)* fnVirtualProtect;
	decltype(MessageBoxA)* fnMessageBox;
	DWORD EntryPoint;
	LPVOID addrINJCode;//注入函数的入口
	LPVOID addrINJCode_RemoteThread;//远程线程注入函数的入口
	LPVOID addrHookCode; /*hook 函数入口*/
	char OldEntry[0x05];
	DWORD dwOldProtect;
	HMODULE hDllBase;
}REMOTEDATA, * PREMOTEDATA;

class x86EntryPointInject {
public:
	x86EntryPointInject();
	~x86EntryPointInject();
	void Clear() {
		if (pi.hProcess != nullptr) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		RtlZeroMemory(&pi, sizeof(pi));
		RtlZeroMemory(&stcRemoteData, sizeof(stcRemoteData));
		lEntryPoint = 0;
		lpRemoteData = nullptr;
		lpRemoteCode = nullptr;
	}

// 	BOOL HideRemoteDll(wchar_t* szDllName) {
// 		GetRemoteModuleHandle(szDllName);
// 		char* szBuffer = new char[dwDllSize] {0};
// 		BOOL bRet = ReadProcessMemory(pi.hProcess, hDllBase, szBuffer, dwDllSize, NULL);
// 		HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, hDllBase, 0, NULL);
// 		WaitForSingleObject(hThread, -1);
// 		LPVOID lpDllBase = VirtualAllocEx(pi.hProcess, hDllBase, dwDllSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
// 		bRet = WriteProcessMemory(pi.hProcess, lpDllBase, szBuffer, dwDllSize, NULL);
// 		delete[]szBuffer;
// 		return TRUE;
// 	}
	BOOL RemoteThreadInject(wchar_t* szDllPath) {
		if (pi.hProcess == INVALID_HANDLE_VALUE) {
			return FALSE;
		}
		LPVOID lpBuffer = VirtualAllocEx(pi.hProcess, NULL, MAX_PATH * 2, MEM_COMMIT, PAGE_READWRITE);
		if (lpBuffer == NULL) {
			return FALSE;
		}
		SIZE_T dwRealWrite;
		BOOL bRet = WriteProcessMemory(pi.hProcess, lpBuffer, szDllPath, (wcslen(szDllPath) + 1) * 2, &dwRealWrite);
		if (!bRet) {
			return FALSE;
		}
		LPTHREAD_START_ROUTINE fnLoadLibraryW = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW");
		HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)fnLoadLibraryW, lpBuffer, 0, NULL);
		if (hThread == NULL) {
			return FALSE;
		}
		//WaitForSingleObject(hThread, -1);
		CloseHandle(hThread);
		VirtualFreeEx(pi.hProcess, lpBuffer, 0, MEM_RELEASE);
		CloseHandle(pi.hProcess);
		return TRUE;
	}
	BOOL startProcess(wchar_t* GameExe, wchar_t* GamePath, wchar_t* cmd);
	BOOL startProcessWithInject(wchar_t* GameExe, wchar_t* GamePath, wchar_t* cmd, char* szDllPath);
	BOOL startDbg() {
		DWORD dwProcessId = pi.dwProcessId;
		std::wstring wszDbgPath = L"C:\\BaiduNetdiskDownload\\01.课件\\x96dbg\\release\\x32\\";
		std::wstring wszDbgName = wszDbgPath + L"x32dbg.exe";
		std::wstring wszDbgCmd = L"\"C:\\BaiduNetdiskDownload\\01.课件\\x96dbg\\release\\x32\\x32dbg.exe\" -p " + std::to_wstring(dwProcessId);
		return startProcess((wchar_t*)wszDbgName.c_str(), (wchar_t*)wszDbgPath.c_str(), (wchar_t*)wszDbgCmd.c_str());
	}
	BOOL startProcessWithRemoteThreadInject(wchar_t* GameExe, wchar_t* GamePath, wchar_t* cmd, char* szDllPath);
	BOOL startProcessWithDbg(wchar_t* GameExe, wchar_t* GamePath, wchar_t* cmd) {
		return startDbg() && startProcess(GameExe, GamePath, cmd);
	}
private:
	LONG_PTR GetEntryPoint(const wchar_t* szFilePath);
	LPVOID AllocDataMemory() {
		lpRemoteData = VirtualAllocEx(pi.hProcess, NULL, 0x1000, MEM_COMMIT, PAGE_READWRITE);
		return lpRemoteData;
	}
	LPVOID AllocCodeMemory() {
		lpRemoteCode = VirtualAllocEx(pi.hProcess, NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE);
		return lpRemoteCode;
	}
	HMODULE GetRemoteModuleHandle(wchar_t* szDllName) {
		MODULEENTRY32 me32 = { sizeof(me32) };
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, pi.dwProcessId);
		if (hSnap == INVALID_HANDLE_VALUE) {
			return false;
		}
		BOOL bRet = Module32First(hSnap, &me32);
		DWORD dwModeBase = 0, dwModeSize = 0;
		while (bRet) {
			if (!wcscmp(szDllName, (const wchar_t*)me32.szExePath) || !wcscmp(szDllName, (const wchar_t*)me32.szModule)) {
				hDllBase = (HMODULE)me32.modBaseAddr;
				dwDllSize = (DWORD)me32.modBaseSize;
				CloseHandle(hSnap);
				return hDllBase;
			}
			bRet = Module32Next(hSnap, &me32);
		}
		CloseHandle(hSnap);
		return 0;
	}
	BOOL CreateRemoteCode();
	BOOL CreateRemoteData(const char* szDllName);
	DWORD GetFunSize(void* lpFunStartAddr);
	static void WINAPI INJ_Code();
	static void WINAPI INJ_Code_RemoteThread();
	static DWORD WINAPI HOOK_Code(LPVOID arg);
private:
	PROCESS_INFORMATION pi;
	LONG_PTR lEntryPoint;
	_REMOTE_DATA stcRemoteData;
	LPVOID lpRemoteData;	//远程数据地址
	LPVOID lpRemoteCode;	//远程代码地址
	HMODULE hDllBase;
	DWORD dwDllSize;
};

