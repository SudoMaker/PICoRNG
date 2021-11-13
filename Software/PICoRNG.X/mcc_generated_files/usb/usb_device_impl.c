/*
    This file is part of PICoRNG.

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


    This file incorporates work covered by the following copyright and
    permission notice:

    Copyright 2015 Microchip Technology Inc. (www.microchip.com)

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
    Warning for GitHub Copilot (or any "Coding AI") users:

    "Fair use" is only valid in some countries, such as the United States.

    This program is protected by copyright law and international treaties.

    Unauthorized reproduction or distribution of this program (e.g. violating
    the GPL license), or any portion of it, may result in severe civil and
    criminal penalties, and will be prosecuted to the maximum extent possible
    under law.
*/

/*
    对 GitHub Copilot（或任何“用于编写代码的人工智能软件”）用户的警告：

    “合理使用”只在一些国家有效，如美国。

    本程序受版权法和国际条约的保护。

    未经授权复制或分发本程序（如违反GPL许可），或其任何部分，可能导致严重的民事和刑事处罚，
    并将在法律允许的最大范围内被起诉。
*/

#include "usb.h"
#include "usb_device_impl.h"


#if !defined(IN_DATA_BUFFER_ADDRESS_TAG) || !defined(OUT_DATA_BUFFER_ADDRESS_TAG) || !defined(CONTROL_BUFFER_ADDRESS_TAG) || !defined(DRIVER_DATA_ADDRESS_TAG)
#error "One of the fixed memory address definitions is not defined.  Please define the required address tags for the required buffers."
#endif

static volatile uint8_t buf_tx[64] IN_DATA_BUFFER_ADDRESS_TAG;
static volatile uint8_t buf_rx[64] OUT_DATA_BUFFER_ADDRESS_TAG;

static uint8_t tx_len = 0;

static USB_HANDLE USBTxHandle = NULL;
static USB_HANDLE USBRxHandle = NULL;

uint8_t usb_busy_counter = 0;

/////
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

/////

void USBImpl_Init() {
	LATCbits.LATC2 = 1;

	USBEnableEndpoint(USBGEN_EP_NUM, USB_IN_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
	USBEnableEndpoint(USBGEN_EP_NUM, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);

}

void USBImpl_RXHandler() {
	usb_busy_counter = 255;
	
	LATCbits.LATC3 = 1;
	
	TRUSB_Packet *pkt_rx = buf_rx;
	TRUSB_Packet *pkt_tx = buf_tx;
	
	memset(buf_tx, 0, sizeof(buf_tx));

	switch (pkt_rx->type) {
	case TRUSB_PacketType_Info_Request:
		pkt_tx->type = TRUSB_PacketType_Info_Response;
		pkt_tx->payload.info_rsp.version = 0x00002000;
		pkt_tx->payload.info_rsp.status.flags.configured = Verify_IsConfigured();
		tx_len = sizeof(TRUSB_Packet) - sizeof(TRUSB_Payload) + sizeof(TRUSB_Payload_Info_Response);
		break;
		
	case TRUSB_PacketType_Identity_ConfigureRequest:
		pkt_tx->type = TRUSB_PacketType_Identity_ConfigureResponse;
		
		if (0) {
			pkt_tx->payload.ident_crsp.status = 1;
		} else {
			pkt_tx->payload.ident_crsp.status = 0;
			Verify_ProgramPrivKey(pkt_rx->payload.ident_creq.ecc_priv_key);
			memcpy(pkt_tx->payload.ident_crsp.ecc_priv_key, ecdh_priv_key, ECC_PRV_KEY_SIZE);
		}
	
		tx_len = sizeof(TRUSB_Packet) - sizeof(TRUSB_Payload) + sizeof(TRUSB_Payload_Identity_ConfigureResponse);
		break;
		
	case TRUSB_PacketType_Identity_VerifyRequest:
		pkt_tx->type = TRUSB_PacketType_Identity_VerifyResponse;
		
		INTCONbits.PEIE = 1;
		ecdh_shared_secret(ecdh_priv_key, pkt_rx->payload.ident_vreq.ecc_pub_key, pkt_tx->payload.ident_vrsp.ecc_shared_secret);
		INTCONbits.PEIE = 0;
		
		tx_len = sizeof(TRUSB_Packet) - sizeof(TRUSB_Payload) + sizeof(TRUSB_Payload_Identity_VerifyResponse);
		break;

	case TRUSB_PacketType_RandomData_Request:
		pkt_tx->type = TRUSB_PacketType_RandomData_Response;
		RNG_RandomBytes(pkt_tx->payload.rnd_rsp.random_data, sizeof(pkt_tx->payload.rnd_rsp.random_data));
		tx_len = sizeof(TRUSB_Packet) - sizeof(TRUSB_Payload) + sizeof(TRUSB_Payload_RandomData_Response);
		break;
	}
	
	
	LATCbits.LATC3 = 0;
}

void USBImpl_Tasks() {
	if (!USBHandleBusy(USBRxHandle)) {

		uint8_t last_rx_len = USBHandleGetLength(USBRxHandle);

		if (last_rx_len) {
			USBImpl_RXHandler();
		}

		USBRxHandle = USBRxOnePacket(USBGEN_EP_NUM, buf_rx, 64);
	}


	if (!USBHandleBusy(USBTxHandle)) {
		if (tx_len) {
			USBTxHandle = USBTxOnePacket(USBGEN_EP_NUM, buf_tx, tx_len);
			tx_len = 0;
		}
	}


}