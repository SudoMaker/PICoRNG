/*
    This file is part of PICoRNG Utility.

    Copyright (C) 2021 ReimuNotMoe <reimu@sudomaker.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Commands.hpp"

void Command_ListDevices() {
	uint8_t path[8];

	for (size_t i=0; i<tr_usb_devices.size(); i++) {
		auto &dev = tr_usb_devices[i];

		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);

		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}


		printf("#%zu (bus %d, device %d", i, libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));

		if (r > 0) {
			printf(", path %d", path[0]);
			for (size_t j = 1; j < r; j++)
				printf(".%d", path[j]);
		}

		printf(") ");

		libusb_device_handle *handle;

		r = libusb_open(dev, &handle);

		if (r == 0) {
			uint8_t buf[32] = {0};

			libusb_get_string_descriptor_ascii(handle, 1, buf, 31);
			puts((char *) buf);

			libusb_close(handle);
		}

	}
}