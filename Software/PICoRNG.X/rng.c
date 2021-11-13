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
    ? GitHub Copilot??????????????????????????

    ???????????????????

    ????????????????

    ????????????????GPL??????????????????????????
    ?????????????????
*/

#include "common.h"

uint8_t RNG_RandomByte() {
    uint8_t byte;
    
    for (uint8_t j=0; j<8; j++) {
                uint8_t cur_byte = (PORTCbits.RC5 << 0);
                cur_byte |= (PORTCbits.RC5 << 1);
                cur_byte |= (PORTCbits.RC5 << 2);
                cur_byte |= (PORTCbits.RC5 << 3);
                cur_byte |= (PORTCbits.RC5 << 4);
                cur_byte |= (PORTCbits.RC5 << 5);
                cur_byte |= (PORTCbits.RC5 << 6);
                cur_byte |= (PORTCbits.RC5 << 7);
                
                byte ^= cur_byte;
    }
    
    return byte;
}

void RNG_RandomBytes(uint8_t *buf, uint8_t len) {
	if (LATCbits.LATC4 == 0) {
		LATCbits.LATC4 = 1;
		__delay_ms(10);
	}
	
	for (uint8_t j=0; j<len; j++) {
		buf[j] = RNG_RandomByte();
	}
}