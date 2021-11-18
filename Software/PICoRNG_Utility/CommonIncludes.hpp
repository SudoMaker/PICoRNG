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

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <functional>
#include <filesystem>

#include <cinttypes>
#include <cmath>
#include <cassert>

#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#include <cxxopts.hpp>
#include <libusb.h>
#include <EntropyCheck.hpp>

extern std::vector<libusb_device *> tr_usb_devices;
extern size_t usb_dev_num;
extern std::string config_dir;