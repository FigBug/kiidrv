# Kiidrv

Kiidrv is a utility designed to install the libusbK driver to Input Club keyboards, although it is flexible enough to be used for other purposes.


# Usage

All commands must include the `--config` option with the name of a file.

## Installing drivers

The `kiidrv --install` command will attempt to install the correct driver for all devices in the config file.

The `--no-confirm` option can be added to automate this.

## Verifying

The `kiidrv --verify` command will inspect the active drivers against those specified in the config file and warn at any mismatches.

Tip: The --install and --verify commands can be combined. 

# Config

The config file is simply an array of json objects. Each object may specify a `vid` and `pid` (required) and an `interface` (optional) to match on.
Upon finding a match the specified `driver` will be installed (one of winusb, libusb0, libusbK) with the specified `vendor` string.

```
[
	{
		"vid":    "0x1234",
		"pid":    "0x5678",
		"interface": 0,
		"vendor": "Foo",
		"driver": "libusbK"
	},
	...
]
```

# Building

Please note that uninstalling drivers with devcon requires a build with matching architecture (ex: win32 will not work on x64).
This can be changed in Solution -> Configuration Manager. All referenced libraries such as getopt and libwdi must also be changed.