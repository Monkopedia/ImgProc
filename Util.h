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
 * Util.h
 *
 *  Created on: Jun 22, 2012
 *      Author: jmonk
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

namespace ImgProc {

class Util {
public:
	static vector<string>* split(string s, char tok) {
		vector<string>* ret = new vector<string>;
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, tok)) {
			ret->push_back(item);
		}
		return ret;
	}

	static bool toBool(string val) {
		if ((val.find('t') != string::npos) || (val.find('T') != string::npos)) {
			return true;
		} else if ((val.find('f') != string::npos) || (val.find('F') != string::npos)) {
			return false;
		} else {
			int v = atoi(val.c_str());
			return v != 0;
		}
	}
};

} /* namespace dvs */
#endif /* UTIL_H_ */
