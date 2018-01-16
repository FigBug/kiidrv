#pragma once
#include "libwdi.h"

struct wdi_device_info* parseConfigStr(char *json);
struct wdi_device_info* parseConfigFile(char *file);