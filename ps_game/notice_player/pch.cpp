#include "pch.h"

DWORD SConsoleAddCommand = 0x4F5BE0;
LPCSTR szNoticeCharName = "/ntcn";
LPCSTR szNoticeCharId = "/ntcid";
//hooks CServerApp::InitConsoleCommand
void __declspec(naked) CommandHook() {
	__asm {
		push 0x0
		push -0x1
		push 0x2 //args
		push szNoticeCharName
		push 0xD //case
		mov ecx,esi
		call SConsoleAddCommand
		push 0x0
		push -0x1
		push 0x2 //args
		push szNoticeCharId
		push 0xE //case
		mov ecx,esi
		call SConsoleAddCommand
		//original code
		pop esi
		retn
	}
}

int NoticeError(PCHAR pBuffer) {
	sprintf_s(pBuffer, 48, "cmd send notice failed");
	return 0; 
}

DWORD dwErrorRetn = 0x4097C0;
DWORD dwErrorJump = 0x40B3D4;
//hooks CServerApp::OnConsoleCommand
void __declspec(naked) ErrorHook() {
	__asm {
		movzx eax,word ptr ds:[0x109E6F8]
		cmp eax,0x0 //users
		je _error
		mov edi,[ebx]
		cmp edi,0x80 //length
		ja _error
		//original code
		lea edx,[ebx+0x4]
		jmp dwErrorRetn

		_error:
		mov eax,[esp+0x3C]
		push eax //buffer
		call NoticeError
		add esp,0x4
		jmp dwErrorJump
	}
}

int NoticeCharNameSuccess(PCHAR pBuffer) {
	sprintf_s(pBuffer, 48, "cmd send notice by char name ok!");
	return 0;
}

int NoticeCharNameError(PCHAR pBuffer, LPCSTR szCharName) {
	sprintf_s(pBuffer, 48, "cmd send notice by char name(%s) failed", szCharName);
	return 0;
}

DWORD dwNoticeJump = 0x40B3D4;
DWORD SConnectionSend = 0x4ED0E0;
DWORD dwSomeFunction = 0x414CE0;
//hooks CServerApp::OnConsoleCommand
void __declspec(naked) NoticeCharNameHook() {
	PBYTE pPacket[256];
	__asm {
		cmp dword ptr[ebp+0xC],0x2 //args
		jne _fail

		add ebx,0x4
		mov eax,ebx //ebx = CharName
		call dwSomeFunction
		mov esi,eax
		test esi,esi
		je _error

		mov eax,[ebx+0x100]
		cmp eax,0x80 //length
		ja _error

		lea edx,[ebx+0x104]

		mov word ptr[pPacket],0xF90B
		mov byte ptr[pPacket+0x2],al
		//write message bytes
		xor edi,edi
		_loop:
		cmp edi,eax
		je _end
		mov ecx,[edx+edi]
		mov [pPacket+edi+0x3],ecx
		inc ecx
		inc edi
		jmp _loop

		_end:
		lea edx,[pPacket]
		add eax,0x3

		mov ecx,esi //esi = pUser
		push eax //length
		push edx //packet
		call SConnectionSend

		mov eax,[esp+0x3C]
		push eax //buffer
		call NoticeCharNameSuccess
		add esp,0x4
		jmp dwNoticeJump

		_error:
		push ebx //CharName
		push edi //buffer
		call NoticeCharNameError
		add esp,0x8
		jmp dwNoticeJump

		_fail:
		jmp dwNoticeJump
	}
}

int NoticeCharIdSuccess(PCHAR pBuffer) {
	sprintf_s(pBuffer, 48, "cmd send notice by char id ok!");
	return 0;
}

int NoticeCharIdError(PCHAR pBuffer, LPCSTR szCharId) {
	sprintf_s(pBuffer, 48, "cmd send notice by char id(%s) failed", szCharId);
	return 0;
}

int CharIdToInt(LPCSTR szCharId) {
	UINT32 uCharId = std::atoi(szCharId);
	return uCharId;
}

DWORD CWorldFindUser = 0x414CC0;
//hooks CServerApp::OnConsoleCommand
void __declspec(naked) NoticeCharIdHook() {
	PBYTE pPacket[256];
	__asm {
		cmp dword ptr[ebp+0xC],0x2 //args
		jne _error

		add ebx,0x4
		push ebx //CharId
		call CharIdToInt
		add esp,0x4

		mov ecx,eax
		test ecx,ecx
		je _fail

		push ecx //CharId
		call CWorldFindUser
		mov esi,eax
		test esi,esi
		je _fail

		mov eax,[ebx+0x100]
		cmp eax,0x80 //length
		ja _fail

		lea edx,[ebx+0x104]

		mov word ptr[pPacket],0xF90B
		mov byte ptr[pPacket+0x2],al
		//write message bytes
		xor edi,edi
		_loop:
		cmp edi,eax
		je _end
		mov ecx,[edx+edi]
		mov [pPacket+edi+0x3],ecx
		inc ecx
		inc edi
		jmp _loop
		
		_end:
		lea edx,[pPacket]
		add eax,0x3

		mov ecx,esi //esi = pUser
		push eax //length
		push edx //packet
		call SConnectionSend

		mov eax,[esp+0x3C]
		push eax //buffer
		call NoticeCharIdSuccess
		add esp,0x4
		jmp dwNoticeJump

		_fail:
		push ebx //CharId
		push edi //buffer
		call NoticeCharIdError
		add esp,0x8
		jmp dwNoticeJump

		_error:
		jmp dwNoticeJump
	}
}

void Init() {
	Hook((LPVOID)0x409459, CommandHook, 5);
	Hook((LPVOID)0x4097BB, ErrorHook, 5);
	Hook((LPVOID)0x409A7A, NoticeCharNameHook, 5);
	Hook((LPVOID)0x409A94, NoticeCharIdHook, 5);
}