#include <stdio.h>
#include "cJSON.h"
#include "util.h"
#include "config_parser.h"

void jsonAssert(int valid)
{
	if (!valid)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error parsing JSON: %s\n", error_ptr);
		}
		else {
			printf("Error parsing JSON\n");
		}
		exit(2);
	}
}

int parseId(char *str)
{
	errno = 0;
	int val = strtol(str, NULL, 0);
	if (!val || errno) {
		printf("Invalid id: '%s'\n", str);
		exit(errno);
	}
	return val;
}

struct wdi_device_info* parseConfigStr(char *json)
{
	cJSON *item, *list;
	list = cJSON_Parse(json);
	jsonAssert(list != NULL);
	jsonAssert(cJSON_IsArray(list));

	struct wdi_device_info *first_product = NULL;
	struct wdi_device_info *prev_product = NULL;

	cJSON_ArrayForEach(item, list)
	{
		cJSON *vidObj = cJSON_GetObjectItem(item, "vid");
		_assert(cJSON_IsString(vidObj), "Invalid vid");
		cJSON *pidObj = cJSON_GetObjectItem(item, "pid");
		_assert(cJSON_IsString(pidObj), "Invalid pid");

		cJSON *interfaceObj = cJSON_GetObjectItem(item, "interface");
		if (interfaceObj) {
			_assert(cJSON_IsNumber(interfaceObj), "Invalid interface");
		}

		cJSON *vendorObj = cJSON_GetObjectItem(item, "vendor");
		if (vendorObj) {
			_assert(cJSON_IsString(vendorObj), "Invalid vendor");
		}
		
		cJSON *driverObj = cJSON_GetObjectItem(item, "driver");
		_assert(cJSON_IsString(driverObj), "Invalid driver");

		struct wdi_device_info *product = (struct wdi_device_info*)calloc(1, sizeof(struct wdi_device_info));
		product->vid = parseId(vidObj->valuestring);
		product->pid = parseId(pidObj->valuestring);
		product->mi = interfaceObj ? interfaceObj->valueint : 0;
		product->is_composite = interfaceObj != NULL;
		product->desc = vendorObj ? vendorObj->valuestring : NULL;
		product->driver = driverObj->valuestring;
		product->next = NULL;

		if (!first_product) {
			first_product = product;
		}
		if (prev_product) {
			prev_product->next = product;
		}
		prev_product = product;
	}
	return first_product;
}

struct wdi_device_info* parseConfigFile(char *file)
{
	FILE *f = NULL;
	fopen_s(&f, file, "r");
	if (!f)
	{
		printf("ERROR: Could not open config %s", file);
		exit(1);
	}

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	char *data = (char*)malloc(size + 1);
	_assert(data != NULL, "malloc");

	fseek(f, 0, SEEK_SET);
	size_t bytes_read = fread(data, 1, size, f);
	data[bytes_read] = '\0';

	fclose(f);
	return parseConfigStr(data);
}

struct wdi_device_info* matchDevice(struct wdi_device_info *device, struct wdi_device_info *products)
{
	struct wdi_device_info *product;
	for (product = products; product != NULL; product = product->next) {
		if ((device->vid == product->vid) &&
			(device->pid == product->pid) &&
			(!device->is_composite || (device->mi == product->mi))) {
			return product;
		}
	}
	return NULL;
}