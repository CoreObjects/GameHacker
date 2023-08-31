#include "pch.h"
#include "CMEMContext.h"
DATA_TYPE data_TYPE[12];
void initializeDataTypes() {
	// Initialize BOOL
	data_TYPE[T_BOOL].UName = L"bool";
	data_TYPE[T_BOOL].size = sizeof(bool);

	// Initialize CHAR
	data_TYPE[T_CHAR].UName = L"char";
	data_TYPE[T_CHAR].size = sizeof(char);

	// Initialize UNSIGNED_CHAR
	data_TYPE[T_UNSIGNED_CHAR].UName = L"unsigned char";
	data_TYPE[T_UNSIGNED_CHAR].size = sizeof(unsigned char);

	// Initialize SHORT
	data_TYPE[T_SHORT].UName = L"short";
	data_TYPE[T_SHORT].size = sizeof(short);

	// Initialize UNSIGNED_SHORT
	data_TYPE[T_UNSIGNED_SHORT].UName = L"unsigned short";
	data_TYPE[T_UNSIGNED_SHORT].size = sizeof(unsigned short);

	// Initialize INT
	data_TYPE[T_INT].UName = L"int";
	data_TYPE[T_INT].size = sizeof(int);

	// Initialize UNSIGNED_INT
	data_TYPE[T_UNSIGNED_INT].UName = L"unsigned int";
	data_TYPE[T_UNSIGNED_INT].size = sizeof(unsigned int);

	// Initialize FLOAT
	data_TYPE[T_FLOAT].UName = L"float";
	data_TYPE[T_FLOAT].size = sizeof(float);

	// Initialize DOUBLE
	data_TYPE[T_DOUBLE].UName = L"double";
	data_TYPE[T_DOUBLE].size = sizeof(double);

	// Initialize LONG_LONG
	data_TYPE[T_LONG_LONG].UName = L"long long";
	data_TYPE[T_LONG_LONG].size = sizeof(long long);

	// Initialize UNSIGNED_LONG_LONG
	data_TYPE[T_UNSIGNED_LONG_LONG].UName = L"unsigned long long";
	data_TYPE[T_UNSIGNED_LONG_LONG].size = sizeof(unsigned long long);

	data_TYPE[T_VOID_PTR].UName = L"void*";
	data_TYPE[T_VOID_PTR].size = sizeof(void*);
}