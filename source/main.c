#include <stdbool.h>
#include "global.h"

Handle fsUserHandle;

u32 sayHello() {
	showDbg("hello world!", 0, 0);
	return 0;
}

int main()
{
    initSharedFunc();

    nsDbgPrint("initializing helloworld plugin\n");
	plgGetSharedServiceHandle("fs:USER", &fsUserHandle);
	plgRegisterMenuEntry(1, "Say Hello", sayHello);
}