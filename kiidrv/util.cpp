#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libwdi.h"
#include "util.h"

FILE *output_file = NULL;

void _assert(int valid, char *msg)
{
	if (!valid)
	{
		fprintf(stderr, "Error: %s\n", msg);
		exit(1);
	}
}

void _printf(const char *format, ...)
{
	char buff[255];

	va_list args;
	va_start(args, format);
	vsnprintf(buff, 255, format, args);
	va_end(args);

	fputs(buff, stdout);
	if (output_file) {
		fputs(buff, output_file);
	}
}

int parseDriverStr(char *driverStr)
{
	if (!driverStr)
		return DRIVER_UNKNOWN;
	
	if (_strcmpi(driverStr, "winusb") == 0)
		return WDI_WINUSB;
	else if (_strcmpi(driverStr, "libusb0") == 0)
		return WDI_LIBUSB0;
	else if (_strcmpi(driverStr, "libusbK") == 0)
		return WDI_LIBUSBK;
	else if (_strcmpi(driverStr, "cdc") == 0)
		return WDI_CDC;
	else if (_strcmpi(driverStr, "user") == 0)
		return WDI_USER;
	else
		return DRIVER_UNKNOWN;
}

void deviceStr(char *dest, size_t size, struct wdi_device_info *device)
{
	if (device->is_composite)
	{
		snprintf(dest, size, "%04X:%04X-%d\t%s", device->vid, device->pid, device->mi, device->desc);
	}
	else
	{
		snprintf(dest, size, "%04X:%04X\t%s", device->vid, device->pid, device->desc);
	}
}