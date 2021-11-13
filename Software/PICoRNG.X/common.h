/* 
 * File:   common.h
 * Author: root
 *
 * Created on October 24, 2021, 4:20 AM
 */

#pragma once

#define _XTAL_FREQ 48000000

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "tinyECDH/ecdh.h"

#define FLASH_ADDR_PRIVKEY      0x1fe0

#define FLASH_UNINIT_VALUE      0xef

extern const uint8_t ecdh_priv_key[ECC_PRV_KEY_SIZE];
extern uint8_t usb_busy_counter;

extern uint8_t RNG_RandomByte();
extern void RNG_RandomBytes(uint8_t *buf, uint8_t len);


extern bool Verify_IsConfigured();
extern void Verify_ProgramPrivKey(uint8_t *priv_key);
