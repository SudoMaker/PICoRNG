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

static bool RNG_QualityCheck(const EntropyCheckResult &res) {
	double montepi_error = fabs(M_PI - res.montepi) / M_PI;

	if (montepi_error > 0.01) {
		printf("warn: montepi_error is off the limits (%lf)\n", montepi_error);
		return false;
	}

	if (res.ent < 0.799) {
		printf("warn: ent is off the limits (%lf)\n", res.ent);
		return false;
	}

	return true;
}

int Command_RngD(const std::vector<std::string>& args) {
	auto *usbdev_handle = USB_OpenCurrentDevice();

	bool verify = true;

	bool rc_verify;

	if (args.size() == 2) {
		if (args[1] == "noverify") {
			verify = false;
		}
	}

	if (verify) {
		puts("Wait for verify");
		rc_verify = Verification_DoVerifyDevice(usbdev_handle, true);
	} else {
		rc_verify = true;
	}

	if (rc_verify) {
		puts("Verify OK");
	} else {
		puts("Verify FAILED");

	}

	int fd_rnd = open("/dev/urandom", O_WRONLY);
	assert(fd_rnd > 0);

	size_t blocks = 1024;
	uint8_t rnd_buf[32 * blocks];

	while (1) {
		memset(rnd_buf, 0, sizeof(rnd_buf));

		TRUSB_Packet pkt_tx, pkt_rx;
		pkt_tx.type = TRUSB_PacketType_RandomData_Request;

//		printf("Gathering %zu blocks (%zu bytes) of random data ...\n\n", blocks, blocks * 32);

		EntropyChecker ec;

		for (size_t i=0; i<blocks; i++){
			USB_SendPacket(usbdev_handle, &pkt_tx);
			USB_Recv(usbdev_handle, &pkt_rx, sizeof(pkt_rx));

			memcpy(rnd_buf + (32 * i), pkt_rx.payload.rnd_rsp.random_data, 32);
			ec.feed(pkt_rx.payload.rnd_rsp.random_data, 32);
		}

		auto results = ec.finalize();

		if (RNG_QualityCheck(results)) {
			printf("writing to rnd pool\n");
			assert(write(fd_rnd, rnd_buf, sizeof(rnd_buf)) == sizeof(rnd_buf));
		}
	}

	USB_CloseDevice(usbdev_handle);

	return 0;
}