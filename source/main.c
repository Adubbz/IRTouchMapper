#include <stdbool.h>
#include <3ds/result.h>
#include <3ds/allocator/mappable.h>
#include <3ds/ipc.h>
#include <3ds/types.h>
#include <3ds/svc.h>
#include <3ds/env.h>
#include <3ds/os.h>
#include <3ds/services/irrst.h>
#include "global.h"

#define SHARED_MEM_LOC 0x26B000

u32 threadStack[0x1000];

Handle irrstHandle;
Handle irrstMemHandle;

Result IRRST_GetMemHandle(Handle* outMemHandle)
{
    u32* cmdbuf = getThreadCommandBuffer();
    cmdbuf[0] = IPC_MakeHeader(0x1, 0, 0); // 0x10000

    Result ret = 0;
    if (R_FAILED(ret = svcSendSyncRequest(irrstHandle)))return ret;

    if (outMemHandle)*outMemHandle = cmdbuf[3];

    return cmdbuf[1];
}

void gamePluginEntry()
{
    u32 heldButtons = 0;
    circlePosition cStickPos;

    nsDbgPrint("[IRTM] Checking for input...");

    while (true)
    {
        u32 Id = 0;

        memset(&cStickPos, 0, sizeof(circlePosition));

        Id = ((vu32*)SHARED_MEM_LOC)[4]; //PAD / circle-pad
        if (Id>7)Id = 7;
        if (irrstCheckSectionUpdateTime((vu32*)SHARED_MEM_LOC, Id) == 0)
        {
            heldButtons = ((vu32*)SHARED_MEM_LOC)[6 + Id * 4];
            cStickPos = *(circlePosition*)&((vu32*)SHARED_MEM_LOC)[6 + Id * 4 + 3];
        }

        if (heldButtons & KEY_ZL)
        {
            nsDbgPrint("ZL Pressed");
            showDbg("ZL Pressed", 0, 0);
        }
        if (heldButtons & KEY_ZR)
        {
            nsDbgPrint("ZR Pressed");
            showDbg("ZR Pressed", 0, 0);
        }
    }
}

void setupIRMem()
{
    u32 ret;
    if (R_FAILED(ret = srvInit()))
    {
        nsDbgPrint("[IRTM] Failed to initialize srv with error code %08x\n", ret);
        return;
    }
    if (R_FAILED(ret = srvGetServiceHandle(&irrstHandle, "ir:rst")))
    {
        nsDbgPrint("[IRTM] Failed to obtain ir:rst handle with error code %08x\n", ret);
        return;
    }
    nsDbgPrint("[IRTM] ir:rst handle: %08x\n", irrstHandle);

    if (R_FAILED(ret = IRRST_GetMemHandle(&irrstMemHandle)))
    {
        nsDbgPrint("[IRTM] Failed to get ir:rst shared memory handle with error code %08x\n", ret);
        goto cleanupHandle;
    }
    nsDbgPrint("[IRTM] ir:rst shared memory handle: %08x\n", irrstMemHandle);

    if (R_FAILED(ret = svcMapMemoryBlock(irrstMemHandle, (u32)SHARED_MEM_LOC, MEMPERM_READ, 0x10000000)))
    {
        nsDbgPrint("[IRTM] Failed to map ir:rst shared memory with error code %08x\n", ret);
        goto cleanupMem;
    }
    nsDbgPrint("[IRTM] Successfully mapped ir:rst shared memory!");

    Handle threadHandle;
    svcCreateThread(&threadHandle, gamePluginEntry, 0, &threadStack[0x1000], 0x3F, 0xFFFFFFFE);
    return;

    // Cleanup
cleanupMem:
    svcUnmapMemoryBlock(irrstMemHandle, (u32)SHARED_MEM_LOC);
cleanupMemHandle:
    svcCloseHandle(irrstMemHandle);
cleanupHandle:
    svcCloseHandle(irrstHandle);
    srvExit();
}

int main()
{
   initSharedFunc();
   plgRegisterMenuEntry(1, "Setup IR Mem", setupIRMem);
}







