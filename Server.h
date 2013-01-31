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
 * Server.h
 *
 *  Created on: Jun 18, 2012
 *      Author: jmonk
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <vector>
#include <queue>
#include <map>
#include <string>
using namespace std;
#include <stdio.h>
#include <sys/select.h>

#include "Runnable.h"

#include <string>

namespace ImgProc {

class Server;

extern Server* mainServer;

class Server {
public:
    static Server* getServer() {
        return mainServer;
    }

	Server();
	virtual ~Server();

	void addListener(int fd, Runnable* readListener);
	void remListener(int fd);

    void stop() {
        isRunning = false;
    }
	void run();

	std::string getVersion() {
		return "0.1 Alpha";
	}

private:
	vector<int> fds;
	vector<Runnable*> readListeners;
	fd_set fdSet;
    bool isRunning;

};

} /* namespace dvs */

#endif /* SERVER_H_ */
