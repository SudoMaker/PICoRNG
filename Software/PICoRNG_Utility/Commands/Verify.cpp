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

#include "../Components/Verification.hpp"

int Command_Verify(const std::vector<std::string>& args) {
	auto *usbdev_handle = USB_OpenCurrentDevice();

	puts("The verification process is going to take at least a minute.\n"
	     "You can get yourself a cup of coffee without moving your sight off the device ;)");

	bool rc = Verification_DoVerifyDevice(usbdev_handle, true);

	if (rc) {
		puts("Verify OK");
	} else {
		puts("Verify FAILED");
	}

	USB_CloseDevice(usbdev_handle);

	return 0;
}