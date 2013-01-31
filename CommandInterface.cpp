/**
 * Pennyworth - A new smarthome protocol.
 * Copyright (C) 2012  Dream-Crusher Labs LLC
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
/*
 * CommandInterface.cpp
 *
 *  Created on: Jun 22, 2012
 *      Author: jmonk
 */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <iostream>
#include <fstream>
//#include <utility>
#include <algorithm>

#include "StringVar.h"
#include "CommandInterface.h"
#include "Util.h"
#include "Server.h"
#include "Scripting.h"

using namespace std;

namespace ImgProc {

CommandInterface* instance;
char* print_buffer;
int print_index;
int buffer_length;

#include <stdarg.h>

void commandHandleGlobal(char* line) {
	instance->commandHandle(line);
}

CommandInterface::CommandInterface(const char* prompt) {
	instance = this;
	rl_callback_handler_install(prompt, (rl_vcpfunc_t*) commandHandleGlobal);
    Server::getServer()->addListener(0, this);
}

CommandInterface::CommandInterface(int argc, char* argv[]) {
	instance = this;
    for (int i = 1; i < argc; i++) {
        char s[30];
        sprintf(s, "arg%d", i - 1);
        std::string str(s);
        std::string value(argv[i]);
       
        setVar(str, new StringVar(value));
    }
}

CommandInterface::~CommandInterface() {
	rl_callback_handler_remove();
    for (std::map<std::string,Variable*>::iterator it=vars.begin(); it!=vars.end(); ++it) {
        delete it->second;
    }
}

void CommandInterface::run() {
	rl_callback_read_char();
}

void CommandInterface::runScript() {
    std::string var0("arg0");
    StringVar* file = dynamic_cast<StringVar*>(getVar(var0));
    if (file == NULL) {
        printf("Problem getting filename...\n");
        exit(-1);
    }
    string filename = file->str;
    if (string::npos == filename.find(".i")) {
        filename = filename + ".i";
    }
    std::ifstream f(filename.c_str());
    if (!f.good()) {
        f.open(("scripts/" + filename).c_str());
    }
    char line[256];

    while (f.good()) {
        f.getline(line, 256);
        commandHandle(line);
    }
}

void CommandInterface::setVar(std::string var, Variable* value) {
    if (vars.find(var) != vars.end()) {
        delete vars[var];
    }
    vars[var] = value;
}

Variable* CommandInterface::getVar(std::string var) {
    return vars[var];
}

bool CommandInterface::hasVar(std::string var) {
    return vars.find(var) != vars.end();
}

void CommandInterface::commandHandle(char* line) {
	string l(line);
	if (l.size() > 0) {
		vector<string>* args = Util::split(l, ' ');
		int i;
		for (i = 0; i < NFUNCS; i++) {
			if (*(functions[i]->first) == (*args)[0]) {
				functions[i]->second(args, this);
				break;
			}
		}
		delete args;
		if (i == NFUNCS) {
			printf("Invalid Command: %s\n", line);
		}
		add_history(line);
	}
}

void CommandInterface::setPromptI(char* prompt) {
	rl_set_prompt(prompt);
}

} /* namespace dvs */
