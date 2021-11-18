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

int Command_Quality(const std::vector<std::string>& args) {
	auto *usbdev_handle = USB_OpenCurrentDevice();

	TRUSB_Packet pkt_tx, pkt_rx;
	pkt_tx.type = TRUSB_PacketType_RandomData_Request;

	size_t blocks = 1024;

	if (args.size() == 2) {
		blocks = strtoul(args[1].c_str(), nullptr, 10);
	}

	printf("Gathering %zu blocks (%zu bytes) of random data ...\n\n", blocks, blocks * 32);

	EntropyChecker ec;

	for (size_t i=0; i<blocks; i++){
		USB_SendPacket(usbdev_handle, &pkt_tx);
		USB_Recv(usbdev_handle, &pkt_rx, sizeof(pkt_rx));

		ec.feed(pkt_rx.payload.rnd_rsp.random_data, 32);
	}

	auto results = ec.finalize();

	{
		printf("Entropy = %f bits per byte.\n", results.ent);

		printf("\nOptimum compression would reduce the size of this %" PRIu64 " bytes stream by %d percent.\n\n",
		       results.totalc, (short) ((100 * (8 - results.ent) / 8.0)));

		printf("Chi square distribution for %" PRIu64 " samples is %1.2f, and randomly\n",
		       results.totalc, results.chisq);
		if (results.pochisq < 0.0001) {
			printf("would exceed this value less than 0.01 percent of the times.\n\n");
		} else if (results.pochisq > 0.9999) {
			printf("would exceed this value more than than 99.99 percent of the times.\n\n");
		} else {
			printf("would exceed this value %1.2f percent of the times.\n\n",
			       results.pochisq * 100);
		}
		printf("Arithmetic mean value of data is %1.4f (%.1f = random).\n",
		       results.mean, 127.5);

		printf("Monte Carlo value for Pi is %1.9f (error %1.2f percent).\n",
		       results.montepi, 100.0 * (fabs(M_PI - results.montepi) / M_PI));
		printf("Serial correlation coefficient is ");
		if (results.scc >= -99999) {
			printf("%1.6f (totally uncorrelated = 0.0).\n", results.scc);
		} else {
			printf("undefined (all values equal!).\n");
		}
	}

	USB_CloseDevice(usbdev_handle);

	return 0;
}