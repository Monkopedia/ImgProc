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
 * Server.cpp
 *
 *  Created on: Jun 18, 2012
 *      Author: jmonk
 */

#include "Server.h"
#include <errno.h>
#include <fcntl.h>

namespace ImgProc {

Server* mainServer;

Server::Server() {
    mainServer = this;
}

Server::~Server() {
    mainServer = NULL;
}

void Server::addListener(int fd, Runnable* listener) {
	fcntl(fd, F_SETFL, O_NONBLOCK);
	fds.push_back(fd);
	readListeners.push_back(listener);
}

void Server::remListener(int fd) {
	for (unsigned int i = 0; i < fds.size(); i++) {
		if (fds[i] == fd) {
//			printf("Removing FD: %d\n", fds[i]);
			fds.erase(fds.begin() + i);
			delete readListeners[i];
			readListeners.erase(readListeners.begin() + i);
			return;
		}
	}
}

int is_valid_fd(int fd) {
	return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
}

void Server::run() {
    isRunning = true;
	while (isRunning) {
		int max = 0;
		FD_ZERO(&fdSet);

		for (unsigned int i = 0; i < fds.size(); i++) {
			FD_SET(fds[i], &fdSet);
			if (fds[i] + 1 > max) {
				max = fds[i] + 1;
			}
		}

		int ret = select(max, &fdSet, NULL, NULL, NULL);

		if (ret <= 0) {
			perror("Error with Select");
			if (errno == EPIPE) {
				for (unsigned int i = 0; i < fds.size(); i++) {
					if (FD_ISSET(fds[i], &fdSet)) {
						printf("Removing FD: %d\n", fds[i]);
						delete readListeners[i];
						fds.erase(fds.begin() + i);
						readListeners.erase(readListeners.begin() + i);
						break;
					}
				}
			}
		}
		for (unsigned int i = 0; i < fds.size(); i++) {
			if (FD_ISSET(fds[i], &fdSet)) {
				readListeners[i]->run();
			}
		}
	}
}

} /* namespace dvs */
