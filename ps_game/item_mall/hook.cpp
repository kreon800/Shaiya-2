#include "pch.h"

BOOL Hook(LPVOID lpAddress, LPVOID lpfn, INT nSize) {
	if (nSize < 5) {
		return FALSE;
	}

	DWORD lpflOldProtect;
	if (!VirtualProtect(lpAddress, nSize, PAGE_EXECUTE_READWRITE, &lpflOldProtect)) {
		return FALSE;
	}

	memset(lpAddress, 0x90, nSize);
	DWORD dwAddress = ((DWORD)lpfn - (DWORD)lpAddress) - 5;
	*(PBYTE)lpAddress = 0xE9;
	*(PDWORD)((DWORD)lpAddress + 1) = dwAddress;

	DWORD flNewProtect = lpflOldProtect;
	VirtualProtect(lpAddress, nSize, flNewProtect, &lpflOldProtect);
	return TRUE;
}