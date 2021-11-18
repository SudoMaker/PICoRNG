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

#include "../tinyECDH/ecdh.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define ATTR_PACKED __attribute__((packed, gcc_struct))
#else
#define ATTR_PACKED __attribute__((packed))
#endif

enum {
	TRUSB_PacketType_None = 0,

	TRUSB_PacketType_Info_Request,
	TRUSB_PacketType_Info_Response,

	TRUSB_PacketType_RandomData_Request,
	TRUSB_PacketType_RandomData_Response,

	TRUSB_PacketType_Identity_ConfigureRequest,
	TRUSB_PacketType_Identity_ConfigureResponse,
	TRUSB_PacketType_Identity_VerifyRequest,
	TRUSB_PacketType_Identity_VerifyResponse,

};

typedef struct {
	uint32_t version;

	union {

		struct {
			uint8_t configured : 1;
		} flags;
		uint32_t value;
	} status;
} ATTR_PACKED TRUSB_Payload_Info_Response;

typedef struct {
	uint8_t ecc_priv_key[ECC_PRV_KEY_SIZE];
} ATTR_PACKED TRUSB_Payload_Identity_ConfigureRequest;

typedef struct {
	uint16_t status;
	uint8_t ecc_priv_key[ECC_PRV_KEY_SIZE];
} ATTR_PACKED TRUSB_Payload_Identity_ConfigureResponse;

typedef struct {
	uint8_t ecc_pub_key[ECC_PUB_KEY_SIZE];
} ATTR_PACKED TRUSB_Payload_Identity_VerifyRequest;

typedef struct {
	uint8_t ecc_shared_secret[ECC_PUB_KEY_SIZE];
} ATTR_PACKED TRUSB_Payload_Identity_VerifyResponse;

typedef struct {
	uint8_t random_data[32];
} ATTR_PACKED TRUSB_Payload_RandomData_Response;

typedef union {
	TRUSB_Payload_Info_Response info_rsp;
	TRUSB_Payload_Identity_ConfigureRequest ident_creq;
	TRUSB_Payload_Identity_ConfigureResponse ident_crsp;
	TRUSB_Payload_Identity_VerifyRequest ident_vreq;
	TRUSB_Payload_Identity_VerifyResponse ident_vrsp;
	TRUSB_Payload_RandomData_Response rnd_rsp;
} TRUSB_Payload;

typedef struct {
	uint8_t type;
	uint8_t rsvd;

	TRUSB_Payload payload;
} ATTR_PACKED TRUSB_Packet;

#ifdef __cplusplus
}
#endif