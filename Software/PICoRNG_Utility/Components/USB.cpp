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

#include "USB.hpp"

#include "Utilities.hpp"

std::vector<libusb_device *> tr_usb_devices;

static libusb_device **usb_devs;

static void USB_ScanDevices() {
	libusb_device *dev;
	size_t i = 0;

	while ((dev = usb_devs[i++])) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);

		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		if (desc.idVendor == 0x04d8 && desc.idProduct == 0xe8b5) {
			tr_usb_devices.push_back(libusb_ref_device(dev));
		}
	}
}

void USB_Init() {
	int r = libusb_init(nullptr);

	if (r < 0) {
		std::cerr << "error: " << "libusb_init failed: " << r << "\n";
		abort();
	}

	ssize_t cnt = libusb_get_device_list(nullptr, &usb_devs);
	if (cnt < 0){
		libusb_exit(nullptr);
		std::cerr << "error: " << "libusb_get_device_list failed: " << r << "\n";
		abort();
	}

	USB_ScanDevices();

	libusb_free_device_list(usb_devs, 1);
}

void USB_DeInit() {

	libusb_exit(nullptr);
}

libusb_device_handle *USB_OpenCurrentDevice() {
	if ((usb_dev_num + 1) > tr_usb_devices.size()) {
		std::cerr << "error: " << "No such device number: " << usb_dev_num << "\n";
		exit(1);
	}

	libusb_device_handle *handle;

	int r = libusb_open(tr_usb_devices[usb_dev_num], &handle);

	if (r == 0) {
		libusb_claim_interface(handle, 0);
		return handle;
	} else {
		std::cerr << "error: " << "libusb_open failed: " << r << "\n";
		exit(2);
	}
}

void USB_CloseDevice(libusb_device_handle *handle) {
	libusb_release_interface(handle, 0);
	libusb_close(handle);
}

int USB_Send(libusb_device_handle *handle, void *buf, size_t len) {
//	printf("USB Send:\n");
//	hexdump(buf, len);

	int actual_length;

	int r = libusb_bulk_transfer(handle, 1 | LIBUSB_ENDPOINT_OUT, (unsigned char *)buf, len, &actual_length, 0);

	if (r < 0) {
		std::cerr << "USB_Send failed: " << r << ", aborting for safety\n";
		abort();
		return r;
	} else {
		return actual_length;
	}
}

bool USB_SendPacket(libusb_device_handle *handle, TRUSB_Packet *pkt) {
	size_t pld_len;

	switch (pkt->type) {
		case TRUSB_PacketType_RandomData_Request:
			pld_len = 0;
			break;
		case TRUSB_PacketType_RandomData_Response:
			pld_len = sizeof(TRUSB_Payload_RandomData_Response);
			break;
		default:
			pld_len = sizeof(TRUSB_Payload);
			break;
	}

	size_t len = sizeof(TRUSB_Packet) - sizeof(TRUSB_Payload) + pld_len;

	return USB_Send(handle, pkt, len) == len;
}

int USB_Recv(libusb_device_handle *handle, void *buf, size_t len) {
	int actual_length;

	int r = libusb_bulk_transfer(handle, 1 | LIBUSB_ENDPOINT_IN, (unsigned char *)buf, len, &actual_length, 0);

	if (r < 0) {
		std::cerr << "USB_Recv failed: " << r << ", aborting for safety\n";
		abort();
		return r;
	} else {
//		printf("USB Recv:\n");
//		hexdump(buf, len);

		return actual_length;
	}
}