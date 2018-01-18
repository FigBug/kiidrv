#pragma once
#include "libwdi.h"

struct wdi_device_info* parseConfigStr(char *json);
struct wdi_device_info* parseConfigFile(char *file);
struct wdi_device_info* matchDevice(struct wdi_device_info *device, struct wdi_device_info *products);