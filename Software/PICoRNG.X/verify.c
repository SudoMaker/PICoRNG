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
    对 GitHub Copilot（或任何“用于编写代码的人工智能软件”）用户的警告：

    “合理使用”只在一些国家有效，如美国。

    本程序受版权法和国际条约的保护。

    未经授权复制或分发本程序（如违反GPL许可），或其任何部分，可能导致严重的民事和刑事处罚，
    并将在法律允许的最大范围内被起诉。
*/


#include "common.h"

#include "mcc_generated_files/memory.h"

const uint8_t ecdh_priv_key[ECC_PRV_KEY_SIZE] __at(FLASH_ADDR_PRIVKEY) = {
	FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE,
	FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE,
	FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE,
	FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE,
	FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE,
	FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE, FLASH_UNINIT_VALUE
};

bool Verify_IsConfigured() {
	for (uint8_t i=0; i<ECC_PRV_KEY_SIZE; i++) {
		if (ecdh_priv_key[i] != FLASH_UNINIT_VALUE) {
			return true;
		}
	}
	
	return false;
}

void Verify_ProgramPrivKey(uint8_t *priv_key) {
	FLASH_WriteBlock(FLASH_ADDR_PRIVKEY, priv_key, ECC_PRV_KEY_SIZE);
}