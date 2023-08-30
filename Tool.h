#pragma once

#include "pch.h"

#define PUT_PUT_DEBUG_BUF_LEN   1024
class Tool {
public:
	static void OutputDebugPrintf(const char* strOutputString, ...) {
		char strBuffer[PUT_PUT_DEBUG_BUF_LEN] = { 0 };
		va_list vlArgs;
		va_start(vlArgs, strOutputString);
		_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);  //_vsnprintf_s  _vsnprintf
		//vsprintf(strBuffer,strOutputString,vlArgs);
		va_end(vlArgs);
		OutputDebugStringA(strBuffer);  //OutputDebugString    // OutputDebugStringW
	}
	static void Dump(const char* pData, size_t nSize) {
		std::string strOut;
		for (size_t i = 0; i < nSize; i++) {
			char buf[8] = "";
			snprintf(buf, sizeof(buf), "%02X ", pData[i] & 0xff);
			strOut += buf;
			strOut += "\n";
		}
		OutputDebugStringA(strOut.c_str());
	}
// 	static void Dump(CPacket packet) {
// 		Dump((const char*)packet.getPacketData(), packet.getPacketSize());
// 	}
};