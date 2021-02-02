#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, Main, 0, 0, 0);
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
