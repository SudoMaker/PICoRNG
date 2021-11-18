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

int Command_Info(const std::vector<std::string>& args) {
	auto *usbdev_handle = USB_OpenCurrentDevice();

	TRUSB_Packet pkt_tx, pkt_rx;
	pkt_tx.type = TRUSB_PacketType_Info_Request;


	USB_SendPacket(usbdev_handle, &pkt_tx);
	USB_Recv(usbdev_handle, &pkt_rx, sizeof(pkt_rx));

	printf("Version: 0x%08x\n", pkt_rx.payload.info_rsp.version);
	puts("");
	printf("Configured: %s\n", pkt_rx.payload.info_rsp.status.flags.configured ? "true" : "false");


	USB_CloseDevice(usbdev_handle);

	return 0;
}