/*
* Copyright (c) 2010-2016 Pete Batard <pete@akeo.ie>
* Copyright (c) 2015 PhracturedBlue <6xtc2h7upj@snkmail.com>
* Copyright (c) 2010 Joseph Marshall <jmarshall@gcdataconcepts.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/*
* WARNING: if any part of the resulting executable name contains "setup" or "instal(l)"
* it will require UAC elevation on Vista and later, and, when run from an MSYS shell,
* will produce a "sh: Bad file number" message.
* See the paragraph on Automatic Elevation at http://helpware.net/VistaCompat.htm
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#ifdef _MSC_VER
#include "getopt/getopt.h"
#else
#include <getopt.h>
#endif
#include "libwdi.h"
#include "devcon_cmds.h"
#include "cJSON.h"
#include "config_parser.h"
#include "util.h"


/// EDIT ME ///
#define DEFAULT_CONFIG "kiibohd.conf"
#define DEFAULT_VENDOR "Input Club"
#define INF_NAME "kiibohd.inf"
#define INF_PATH "driver"

#define CMD_BASENAME TEXT("kiidrv")
#define TIMEOUT_S 60
#define LOG_LEVEL_DEFAULT WDI_LOG_LEVEL_WARNING
#define LOG_LEVEL_VERBOSE WDI_LOG_LEVEL_DEBUG
///////


// from http://support.microsoft.com/kb/124103/
HWND GetConsoleHwnd(void)
{
	HWND hwndFound;
	char pszNewWindowTitle[128];
	char pszOldWindowTitle[128];

	GetConsoleTitleA(pszOldWindowTitle, 128);
	wsprintfA(pszNewWindowTitle, "%d/%d", GetTickCount(), GetCurrentProcessId());
	SetConsoleTitleA(pszNewWindowTitle);
	Sleep(40);
	hwndFound = FindWindowA(NULL, pszNewWindowTitle);
	SetConsoleTitleA(pszOldWindowTitle);
	return hwndFound;
}

void wdiAssert(int rc)
{
	if (rc != WDI_SUCCESS) {
		printf("Error %d: %s\n", rc, wdi_strerror(rc));
		exit(rc);
	}
}

void listDevices(struct wdi_device_info **devices)
{
	static struct wdi_options_create_list cl_options = { 0 };
	cl_options.list_all = TRUE;
	cl_options.trim_whitespaces = TRUE;

	int rc = wdi_create_list(devices, &cl_options);
	if (rc == WDI_ERROR_NO_DEVICE)
	{
		printf("No devices found. Double check your connection and try again.\n");
		exit(rc);
	}
	
	wdiAssert(rc);
}

void installDriver(struct wdi_device_info *device, int driver, char *vendor)
{
	VS_FIXEDFILEINFO driver_info;
	if (!wdi_is_driver_supported(driver, &driver_info)) {
		printf("Driver unsupported!\n");
		return;
	}
	
	static struct wdi_options_prepare_driver pd_options = { 0 };
	pd_options.driver_type = driver;
	pd_options.vendor_name = vendor ? vendor : DEFAULT_VENDOR;
	wdiAssert(wdi_prepare_driver(device, INF_PATH, INF_NAME, &pd_options));

	static struct wdi_options_install_driver id_options = { 0 };
	id_options.hWnd = GetConsoleHwnd();
	id_options.pending_install_timeout = TIMEOUT_S * 1000;
	wdiAssert(wdi_install_driver(device, INF_PATH, INF_NAME, NULL));
	printf("Done!\n");
}

void removeDriver(struct wdi_device_info *device)
{
	char devstr[256];
	sprintf_s(devstr, "*VID_%04X&PID_%04X&MI_%02X*", device->vid, device->pid, device->mi);

	size_t newsize = strlen(devstr) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, devstr, _TRUNCATE);

	int argc = 1;
	PTSTR argv[] = { wcstring };
	int rc = cmdRemove(CMD_BASENAME, NULL, NULL, argc, argv);
	_assert(rc == EXIT_OK, "Remove driver failed");

	delete wcstring;
}

void rescan()
{
	int rc = cmdRescan(CMD_BASENAME, NULL, NULL, NULL, NULL);
	_assert(rc == EXIT_OK, "Rescan failed");
	Sleep(1000); //wait for drivers to be selected
}

bool confirm(char *msg, int no_prompt)
{
	if (no_prompt) {
		return TRUE;
	}

	printf(msg);
	int c = (char)getchar();
	while (getchar() != 0x0A); //FLUSH
	return (c != 'n') && (c != 'N');
}

bool verifyDevices(struct wdi_device_info *products)
{
	struct wdi_device_info *list;
	listDevices(&list);

	bool invalid = 0;
	struct wdi_device_info *device;
	char device_s[256];

	for (device = list; device != NULL; device = device->next) {
		struct wdi_device_info *product = matchDevice(device, products);
		if (product == NULL) continue;

		int correct = device->driver && (_strcmpi(device->driver, product->driver) == 0);
		if (!correct) invalid++;
		
		deviceStr(device_s, sizeof(device_s), device);
		printf("%s%c \t%s\n", device->driver, correct ? ' ' : '*', device_s);
	}

	if (invalid)
	{
		printf("\nWarning: Possibly misconfigured devices detected!\n");
	}
	else {
		printf("\nAll devices appear to be configured correctly\n");
	}

	wdi_destroy_list(list);
	return invalid == 0;
}

void fixDevices(struct wdi_device_info *products, int no_prompt)
{
	struct wdi_device_info *list;
	listDevices(&list);

	int installed = 0;
	int removed = 0;
	struct wdi_device_info *device;
	char device_s[256];

	for (device = list; device != NULL; device = device->next) {
		struct wdi_device_info *product = matchDevice(device, products);
		if (product == NULL) continue;

		int correct = device->driver && (_strcmpi(device->driver, product->driver) == 0);
		if (correct) continue;

		deviceStr(device_s, sizeof(device_s), device);
		printf("%s has %s but should be %s\n", device_s, device->driver, product->driver);

		int currentDriver = parseDriverStr(device->driver);
		int newDriver = parseDriverStr(product->driver);
		if (newDriver != DRIVER_UNKNOWN)
		{
			// unknown -> {libusbK, libusb0, ...}
			// misconfigured device, try to use libwdi to install the correct driver
			if (confirm(" * Do you want to install the driver for this device (Y/n)? ", no_prompt))
			{
				printf("Installing %s driver... ", product->driver);
				installDriver(device, newDriver, product->desc);
				installed++;
			}
			else {
				printf("Skipping\n");
			}
		}
		else
		{
			if (currentDriver != DRIVER_UNKNOWN) {
				// {libusbK, libusb0, ...} -> unknown
				// this tool was likely incorrectly used in the past, revert to the default driver hoping the 'unknown' one will be reinstalled
				if (confirm(" * Do you want to reset the driver for this device (Y/n)? ", no_prompt))
				{
					removeDriver(device);
					removed++;
				}
				else
				{
					printf("Skipping\n");
				}
			}
			else
			{
				// unknown -> unknown
				// user may have purposely installed another driver, don't not mess with it to be safe
				printf("Situation not able to be automatically solved.\n");
			}
		}

		printf("\n");
	}

	if (!removed && !installed) {
		printf("Nothing to do.\n\n");
	}

	if (removed)
	{	
		rescan();
		printf("\n");
	}

	wdi_destroy_list(list);
}

int main(int argc, char *argv[])
{
	int c = -1;
	int option_index = 0;
	static int verify = 0;
	static int install = 0;
	static int no_prompt = 0;
	static int verbose = 0;
	char *config = DEFAULT_CONFIG;

	// Parse command-line options
	while (1)
	{
		static struct option long_options[] = {
			{"config", required_argument, 0, 'c'},
			{"verify", no_argument, &verify, 'v'},
			{"install", no_argument, &install, 'i'},
			{"no-confirm", no_argument, &no_prompt, 'p'},
			{"help", no_argument, 0, 'h'},
			{"verbose", no_argument, &verbose, 'd'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, ":clivhd", long_options, &option_index);
		//  Detect the end of the options.
		if (c == -1)
			break;

		switch (c)
		{
		case 0:
			// If this option set a flag, do nothing else now.
			if (long_options[option_index].flag != 0) {
				break;
			}
			printf("option %s", long_options[option_index].name);
			if (optarg) {
				printf(" with arg %s", optarg);
			}
			printf("\n");
			break;
		case 'c':
			config = optarg;
			break;
		case 'h': // getopt_long already printed an error message.
		default:
			printf("--config  FILE     json file specifiying the correct drivers for devices\n");
			printf("--verify           verifies the correct drivers are installed for the configured devices\n");
			printf("--install          installs drivers to the configured devices\n");
			printf("\n");
			printf("Options:\n");
			printf("--no-confirm       does not prompt to install\n");
			printf("--verbose          increases logging\n");
			printf("--help             this message\n");
			printf("\n");
			return 1;
		}
	}

	if (!verify && !install)
	{
		printf("Please specify --verify or --install. See --help for more info.\n");
		return 1;
	}

	if (!config)
	{
		printf("Please specify --config <file>. See --help for more info.\n");
		return 1;
	}

	struct wdi_device_info *products = parseConfigFile(config);

	wdi_set_log_level(verbose ? LOG_LEVEL_VERBOSE : LOG_LEVEL_DEFAULT);

	if (install)
	{
		fixDevices(products, no_prompt);
	}
	if (verify)
	{
		verifyDevices(products);
	}

	return 0;
}
