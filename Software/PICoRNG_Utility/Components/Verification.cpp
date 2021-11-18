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

#include "Verification.hpp"

#include "Utilities.hpp"
#include "../Components/USB.hpp"

void Verification_NewKeyPair(uint8_t *priv_key, uint8_t *pub_key) {
	int fd = open("/dev/urandom", O_RDONLY);
	assert(fd > 0);

	assert(read(fd, priv_key, ECC_PRV_KEY_SIZE) == ECC_PRV_KEY_SIZE);

//	for (size_t i=0; i<ECC_PRV_KEY_SIZE; i++) {
//		priv_key[i] = 0xef;
//	}

	ecdh_generate_keys(pub_key, priv_key);
}

void Verification_SavePubKey(uint8_t *pub_key) {
	std::string fp = config_dir + bin2hex(pub_key, ECC_PUB_KEY_SIZE);

	int fd = open(fp.c_str(), O_WRONLY | O_CREAT, 0600);
	assert(fd > 0);

	assert(write(fd, pub_key, ECC_PUB_KEY_SIZE) == ECC_PUB_KEY_SIZE);

	close(fd);
}

std::vector<std::vector<uint8_t>> Verification_LoadAllPubKeys() {
	std::vector<std::vector<uint8_t>> ret;

	std::vector<uint8_t> buf(ECC_PUB_KEY_SIZE);

	for (const auto &dir_entry : std::filesystem::recursive_directory_iterator(config_dir)) {
		if (dir_entry.is_regular_file() && dir_entry.file_size() == ECC_PUB_KEY_SIZE) {
			int fd = open(dir_entry.path().c_str(), O_RDONLY);
			assert(fd > 0);
			assert(read(fd, buf.data(), ECC_PUB_KEY_SIZE) == ECC_PUB_KEY_SIZE);
			ret.push_back(buf);
		}
	}

	return ret;
}

bool Verification_DoConfigureDevice(libusb_device_handle *usb_device, bool verbose) {
	// 1. Generate new random key pair
	uint8_t priv_key[ECC_PRV_KEY_SIZE], pub_key[ECC_PUB_KEY_SIZE];
	mlock(priv_key, ECC_PRV_KEY_SIZE);

	Verification_NewKeyPair(priv_key, pub_key);

	auto priv_key_hex = bin2hex(priv_key, ECC_PRV_KEY_SIZE);

	if (verbose) {
		printf("%% [%u] %s\n", ECC_PRV_KEY_SIZE, priv_key_hex.c_str());
	}

	// 2. Send to device
	if (verbose) {
		std::cout << "ConfigureDevice: " << "Sending request" << "\n";
	}

	TRUSB_Packet pkt_send;
	pkt_send.type = TRUSB_PacketType_Identity_ConfigureRequest;
	memcpy(&pkt_send.payload.ident_creq, priv_key, ECC_PRV_KEY_SIZE);
	assert(USB_SendPacket(usb_device, &pkt_send));

	// 3. Receive and compare result
	if (verbose) {
		std::cout << "ConfigureDevice: " << "Waiting response" << "\n";
	}

	TRUSB_Packet pkt_recv;
	assert(USB_Recv(usb_device, &pkt_recv, sizeof(TRUSB_Packet)));
	if (pkt_recv.type != TRUSB_PacketType_Identity_ConfigureResponse) {
		std::cout << "ConfigureDevice: " << "Bad packet type" << "\n";
		return false;
	}

	if (pkt_recv.payload.ident_crsp.status != 0) {
		std::cout << "ConfigureDevice: " << "Device already configured" << "\n";
		return false;
	}

	if (memcmp(pkt_recv.payload.ident_crsp.ecc_priv_key, priv_key, ECC_PRV_KEY_SIZE) != 0) {
		auto data_out = bin2hex(priv_key, ECC_PRV_KEY_SIZE);
		auto data_in = bin2hex(pkt_recv.payload.ident_crsp.ecc_priv_key, ECC_PRV_KEY_SIZE);

		if (verbose) {
			printf("> [%u] %s\n", ECC_PRV_KEY_SIZE, data_out.c_str());
			printf("< [%u] %s\n", ECC_PRV_KEY_SIZE, data_in.c_str());
		}

		std::cout << "ConfigureDevice: " << "Data verification failed" << "\n";
		return false;
	}

	// 4. Save pubkey
	Verification_SavePubKey(pub_key);

	std::cout << "ConfigureDevice: " << "Success" << "\n";

	memset(priv_key, 0, ECC_PRV_KEY_SIZE);
	munlock(priv_key, ECC_PRV_KEY_SIZE);

	return true;
}

bool Verification_DoVerifyDevice(libusb_device_handle *usb_device, bool verbose) {
	// 1. Load all device pubkeys
	auto device_pubkeys = Verification_LoadAllPubKeys();

	if (device_pubkeys.empty()) {
		return false;
	}

	// 2. Generate new random key pair
	uint8_t rnd_priv_key[ECC_PRV_KEY_SIZE], rnd_pub_key[ECC_PUB_KEY_SIZE];
	Verification_NewKeyPair(rnd_priv_key, rnd_pub_key);

	// 3. Send random pubkey to device
	TRUSB_Packet pkt_send;
	pkt_send.type = TRUSB_PacketType_Identity_VerifyRequest;
	memcpy(&pkt_send.payload.ident_vreq.ecc_pub_key, rnd_pub_key, ECC_PUB_KEY_SIZE);

	assert(USB_SendPacket(usb_device, &pkt_send));

	// 4. Calculate expected shared secret for each device pubkey
	std::vector<std::vector<uint8_t>> shared_secrets;

	for (auto &it : device_pubkeys) {
		std::vector<uint8_t> ssec(ECC_PUB_KEY_SIZE);
		ecdh_shared_secret(rnd_priv_key, it.data(), ssec.data());
		shared_secrets.emplace_back(std::move(ssec));
	}

	sleep(5);

	// 5. Receive device calculated shared secret
	TRUSB_Packet pkt_recv;
	assert(USB_Recv(usb_device, &pkt_recv, sizeof(TRUSB_Packet)) == sizeof(TRUSB_Packet));
	if (pkt_recv.type != TRUSB_PacketType_Identity_VerifyResponse) {
		std::cout << "VerifyDevice: " << "Bad packet type" << "\n";
		return false;
	}

	if (verbose) {
		std::cerr << "VerifyDevice: " << "Device returned SS: " << bin2hex(pkt_recv.payload.ident_vrsp.ecc_shared_secret, ECC_PUB_KEY_SIZE) << "\n";
	}

	// 6. Check if there are matches
	for (auto &it : shared_secrets) {
		auto ss_hexstr = bin2hex(it.data(), it.size());

		if (verbose) {
			printf("VerifyDevice: Checking SS %s:", ss_hexstr.data());
		}

		if (memcmp(it.data(), pkt_recv.payload.ident_vrsp.ecc_shared_secret, ECC_PUB_KEY_SIZE) == 0) {
			if (verbose) {
				printf(" - Match!\n");
			}
			return true;
		}

		if (verbose) {
			printf(" - Nope\n");
		}
	}

	return false;
}