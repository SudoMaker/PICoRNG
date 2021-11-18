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

#include "CommonIncludes.hpp"
#include "Commands/Commands.hpp"

void ShowHelpExtra() {
	puts(
		"\n"
		" Commands:\n"
		"  info                      Show device info\n"
		"  pair                      Pair device\n"
		"  verify                    Verify device\n"
		"  cat [block count]         Read random data into stdout\n"
		"  quality [block count]     Check random data quality\n"
		"  rngd [noverify]           Feed random data to system\n"
	);
}

size_t usb_dev_num = 0;

std::string config_dir;

int main(int argc, char **argv) {
	cxxopts::Options options("PICoRNG", "PICoRNG Utility");

	std::vector<std::string> positional_args;

	options.add_options("Main")
		("h,help", "Show this help")
		("l,list-devices", "List all devices")
		("n,device-number", "Specify device number", cxxopts::value<size_t>(usb_dev_num)->default_value("0"))
		("c,config-dir", "Specify configuration directory", cxxopts::value<std::string>(config_dir)->default_value("~/.picorng/"))
		;

	options.add_options("positional")
		("positional", "Positional parameters: The file to write/read", cxxopts::value<std::vector<std::string>>(positional_args))
		;

	options.parse_positional("positional");
	options.positional_help("COMMAND...").show_positional_help();

	USB_Init();

	try {
		auto cmd = options.parse(argc, argv);

		if (cmd.count("list-devices")) {
			Command_ListDevices();
			return 0;
		}

		if (cmd.count("help") || positional_args.empty()) {
			std::cout << options.help({"Main"});
			ShowHelpExtra();
			return 0;
		}
	} catch (std::exception &e) {
		std::cout << "Error: " << e.what() << "\n";
		std::cout << options.help({"Main"});
		ShowHelpExtra();
		return 1;
	}

	if (geteuid()) {
		printf("Warning: You may encounter problems without root permissions\n");
	}

	if (!config_dir.empty()) {
		if (config_dir[0] == '~') {
			auto *env_home = getenv("HOME");

			if (!env_home) {
				printf("error: `~' specified in config dir path but $HOME is undefined\n");
				return 2;
			}

			std::string newstr = env_home;
			newstr.insert(newstr.end(), config_dir.begin()+1,  config_dir.end());
			std::swap(newstr, config_dir);
		}

		if (config_dir[config_dir.size()-1] != '/') {
			config_dir.push_back('/');
		}
	}

	if (-1 == mkdir(config_dir.c_str(), 0700)) {
		if (errno != EEXIST) {
			perror("error: Failed to create config directory");
			return 2;
		}
	}

	fprintf(stderr, "Config directory: %s\n", config_dir.c_str());

	std::unordered_map<std::string, std::function<int(const std::vector<std::string>&)>> command_map = {
		{"cat", [](auto &arg){return Command_Cat(arg);}},
		{"quality", [](auto &arg){return Command_Quality(arg);}},
		{"info", [](auto &arg){return Command_Info(arg);}},
		{"pair", [](auto &arg){return Command_Pair(arg);}},
		{"verify", [](auto &arg){return Command_Verify(arg);}},
		{"rngd", [](auto &arg){return Command_RngD(arg);}},
	};

	auto it = command_map.find(positional_args[0]);

	if (it != command_map.end()) {
		return it->second(positional_args);
	} else {
		printf("error: no such command: %s, read the help!\n", positional_args[0].c_str());
	}

	return 2;
}
