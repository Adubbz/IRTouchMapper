#define IS_PLUGIN 1
#define USE_SOCKET 0

#include "main.h"
#include "math.h"
#include "3dstypes.h"
#include "constants.h"
#include "xprintf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <3ds/types.h>
#include <3ds/srv.h>
#include <3ds/services/apt.h>
//#include <3ds/GSP.h>
#include <3ds/gpu/gx.h>
#include <3ds/services/hid.h>
#include <3ds/services/fs.h>
#include <3ds/svc.h>
#include <3ds/services/soc.h>

#include "func.h"
#include "sharedfunc.h"
#include "ns/ns.h"
//#include <sys/socket.h>