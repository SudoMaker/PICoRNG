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

#pragma once

#include "../CommonIncludes.hpp"

#include "Protocol.h"

extern void USB_Init();
extern libusb_device_handle *USB_OpenCurrentDevice();
extern void USB_CloseDevice(libusb_device_handle *handle);
extern int USB_Send(libusb_device_handle *handle, void *buf, size_t len);
extern bool USB_SendPacket(libusb_device_handle *handle, TRUSB_Packet *pkt);
extern int USB_Recv(libusb_device_handle *handle, void *buf, size_t len);