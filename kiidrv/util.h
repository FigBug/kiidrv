#pragma once

#define DRIVER_UNKNOWN (WDI_NB_DRIVERS+1)

void _assert(int valid, char *msg);
int parseDriverStr(char *driverStr);
void deviceStr(char *dest, size_t size, struct wdi_device_info *device);