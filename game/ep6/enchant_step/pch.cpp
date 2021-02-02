#include "pch.h"

DWORD dwEnchantRetn = 0x4B8766;
void __declspec(naked) EnchantHook() {
	__asm {
		push ebx
		lea ebx,[aWeaponStep]
		movzx eax,word ptr[ebx+eax*2]
		pop ebx
		jmp dwEnchantRetn
	}
}

void Init() {
	Hook((LPVOID)0x4B8755, EnchantHook, 17);
}
