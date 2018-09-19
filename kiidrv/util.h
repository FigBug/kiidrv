#pragma once

#define DRIVER_UNKNOWN (WDI_NB_DRIVERS+1)

extern FILE *output_file;

void _assert(int valid, char *msg);
void _printf(const char *format, ...);
int parseDriverStr(char *driverStr);
void deviceStr(char *dest, size_t size, struct wdi_device_info *device);