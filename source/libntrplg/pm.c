#include "global.h"

Handle hCurrentProcess = 0;
u32 currentPid = 0;


u32 getCurrentProcessId() {
	svcGetProcessId(&currentPid, 0xffff8001);
	return currentPid;
}

u32 getCurrentProcessHandle() {
	u32 handle = 0;
	u32 ret;

	if (hCurrentProcess != 0) {
		return hCurrentProcess;
	}
	svcGetProcessId(&currentPid, 0xffff8001);
	ret = svcOpenProcess(&handle, currentPid);
	if (ret != 0) {
		showDbg("openProcess failed, ret: %08x", ret, 0);
		return 0;
	}
	hCurrentProcess = handle;
	return hCurrentProcess;
}



u32 protectRemoteMemory(Handle hProcess, void* addr, u32 size) {
	u32 outAddr = 0;

	return svcControlProcessMemory(hProcess, addr, addr, size, 6, 7);
}

u32 protectMemory(void* addr, u32 size) {
	return protectRemoteMemory(getCurrentProcessHandle(), addr, size);
}



