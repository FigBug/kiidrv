#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libwdi.h"
#include "util.h"

void _assert(int valid, char *msg)
{
	if (!valid)
	{
		fprintf(stderr, "Error: %s\n", msg);
		exit(1);
	}
}

int parseDriverStr(char *driverStr)
{
	if (_strcmpi(driverStr, "winusb") == 0)
		return WDI_WINUSB;
	else if (_strcmpi(driverStr, "libusb0") == 0)
		return WDI_LIBUSB0;
	else if (_strcmpi(driverStr, "libusbK") == 0)
		return WDI_LIBUSBK;
	else
		return -1;
}