#pragma once
/*
0 bool
1 char
2 unsigned char
3 short
4 unsigned short
5 int
6 unsigned int
7 float
8 double
9 long long
10 unsigned long long
11 void *
*/
enum DataType {
	T_BOOL = 0,
	T_CHAR,
	T_UNSIGNED_CHAR,
	T_SHORT,
	T_UNSIGNED_SHORT,
	T_INT,
	T_UNSIGNED_INT,
	T_FLOAT,
	T_DOUBLE,
	T_LONG_LONG,
	T_UNSIGNED_LONG_LONG,
	T_VOID_PTR
};
struct DATA_TYPE {
	wchar_t* UName;
	unsigned size;
};
extern DATA_TYPE data_TYPE[12];

// 	/*
// 		m_DataList.InsertColumn(0, L"VA", 0, 100);
// 		m_DataList.InsertColumn(1, L"Offset", 0, 100);
// 		m_DataList.InsertColumn(2, L"Type", 0, 100);
// 		m_DataList.InsertColumn(3, L"Name", 0, 100);
// 		m_DataList.InsertColumn(4, L"Value", 0, 100);
// 		m_DataList.InsertColumn(5, L"Comment", 0, 100);
// 	*/
typedef struct CMEMContextNode {
public:
	CMEMContextNode(int TypeId, DWORD _len, const wchar_t* _name, const wchar_t* _txt, CMEMContextNode* _front) {
		nTypeId = TypeId;
		pDataType = &data_TYPE[TypeId];
		strValName = _name;
		strNote = _txt;
		dwLength = _len;
		front = _front;
		if (front == nullptr) {
			dwOffset = 0;
		}
		else {
			dwOffset = front->dwOffset + front->dwLength * front->pDataType->size;
		}
	}
	
	int					nTypeId;
	DATA_TYPE*			pDataType;
	CString				strValName;
	DWORD				dwLength;	//多少个数据
	CString				strNote;	//注释
	DWORD				dwOffset;
	CMEMContextNode* front;
	CMEMContextNode* next;
}CMEMContext, * PCMEMContext;

class CMEMContextList {
public:
	~CMEMContextList() {
		auto ite = m_pHeader;
		while (ite) {
			auto jte = ite;
			ite = ite->next;
			delete jte;
		}
	}
	CMEMContextList() {
		m_pHeader = nullptr;
	}
	CMEMContextList(const char* szData, DWORD dwSize, DWORD dwAlign = 4) {
		DWORD dwNum = dwSize / dwAlign;
		DWORD dwRemain = dwSize % dwAlign;
		m_szData.resize(dwSize);
		RtlCopyMemory(m_szData.data(), szData, dwSize);
		m_pHeader = new CMEMContext(T_INT, dwNum, L"Unknown", L"Unknown", nullptr);
		switch (dwRemain) {
		case 0: {
			m_pHeader->next = nullptr;
			break;
		}
		case 1: {
			m_pHeader->next = new CMEMContextNode(T_CHAR, 1, L"Unknown", L"Unknown",m_pHeader);
			m_pHeader->next->next = nullptr;
			break;
		};
		case 2: {
			m_pHeader->next = new CMEMContextNode(T_SHORT, 1, L"Unknown", L"Unknown",m_pHeader);
			m_pHeader->next->next = nullptr;
			break;
		}
		case 3: {
			m_pHeader->next = new CMEMContextNode(T_SHORT, 1, L"Unknown", L"Unknown",m_pHeader);
			m_pHeader->next->next = new CMEMContextNode(T_CHAR, 1, L"Unknown", L"Unknown", m_pHeader->next);
			m_pHeader->next->next->next = nullptr;
			break;
		}
		}
	}
	CString ReadVal(PCMEMContext pMemNode,DWORD dwIndex) {
		CString strRet;
		auto ite = m_pHeader;
		while (ite) {
			if (ite == pMemNode) {
				break;
			}
			ite = ite->next;
		}
		switch (pMemNode->nTypeId) {
		case T_INT: {
			int* pINT = (int*)&m_szData[ite->dwOffset + dwIndex * ite->pDataType->size];
			strRet.Format(L"%d", *pINT);
			break;
		}
		}
		return strRet;
	}
	std::vector<char>	m_szData;
	PCMEMContext		m_pHeader;
};

void initializeDataTypes();













// 
// typedef class CMEMContext {
// public:
// 	CMEMContext(int TypeId,int _len, const wchar_t* _name, const wchar_t* _txt,CMEMContext * _front) {
// 		ulength = _len;
// 		pType = &data_TYPE[TypeId];
// 		strName = _name; 
// 		strName = _txt;
// 		front = _front;
// 		if (front) {
// 			unsigned align = 4;
// 			unsigned allsize = front->dwOffset + front->pType->size * front->ulength; 
// 			unsigned m = allsize % align;
// 			unsigned free = align - m; 
// 			unsigned fix;
// 			if (free < pType->size) {
// 				fix = free;
// 			}
// 			else
// 				fix = pType->size - m % pType->size;
// 			dwOffset = allsize + fix;
// 		}
// 	}
// 	bool			Used;
// 	DATA_TYPE*		pType;
// 	unsigned		ulength;
// 	unsigned		dwOffset;
// 	CString			strName; //hp int hp;
// 	CString			strNote;
// 	CMEMContext*	front;
// 	CMEMContext*	next;
// }*PMEMContext;
