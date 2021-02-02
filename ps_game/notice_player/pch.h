#pragma once
#include <windows.h>
#include <string>

BOOL Hook(LPVOID lpAddress, LPVOID lpfn, INT nSize);
void Init();
