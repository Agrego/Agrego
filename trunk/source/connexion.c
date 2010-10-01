/*
   Copyright (c) 2010, The AgreGo Team <team@agrego.net>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE

 This project has begun as an "Epitech Innovative Project" in Epitech, France.
*/
#include <stdio.h>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

#include "headers/log_save.h"
#include "headers/handshake.h"
#include "headers/connexion.h"

/**
 * write on a connexion structure
 * @param {connex} a valid connexion structure which contains a valid socket to write
 * @param {buffout} the buffer to write
 * @param {len} the size to write
 */
int	writeOnConn(struct Connexion *connex, const void *buffout, int len)
{
	ssize_t write;

	if ((write = sendto(connex->socket, buffout, len, 0, (struct sockaddr *) &connex->address, sizeof(struct sockaddr_in))) < 0)
	{
		perror("sending did not work correctly");
		return 0;
	}
	return write;
}

/**
 * write on a connexion structure
 * @param {connex} a valid connexion structure which contains a valid socket to read
 * @param {buffin} the buffer to put that have been read
 * @param {len}	   the size to read
 */
int	readOnConn(struct Connexion *connex, void *buffin, int len)
{
	socklen_t	addrLen;
	int 	read;

	addrLen = sizeof(struct sockaddr_in);
	if ((read = recvfrom(connex->socket, buffin, len, 0, (struct sockaddr *) &connex->address, &addrLen)) < 0)
	{
		return 0;
	}

	if ((connex->lastRead = time(NULL)) == -1)
	{
		perror("time");
		log_write(LOG_CRIT, "can't use the fail detection feature error when using time: %s", strerror(errno));
	}

	return read;
}

/**
 * initialize a connexion and do a handshake
 * @param {port} 	the source port
 * @param {address} 	the source ip address
 * @param {port_dest} 	the destination port
 * @param {address_dest} the destination address
 * @param {ponderation} the weighting
 */
struct Connexion	*initConnexion(int port, char *address, int port_dest, char *address_dest, int ponderation)
{
	int	sock;
	struct protoent	*proto;
	struct sockaddr_in	addr;
	struct Connexion	*connex;

	log_write(LOG_NOTICE, "create a socket listenning on: %s:%d and writing on %s:%d", address, port, address_dest, port_dest);

	proto = getprotobyname("udp");
	if ((sock = socket(AF_INET, SOCK_DGRAM, proto->p_proto)) == -1)
	{
		/* log */
		perror("can't create the socket");
		exit(EXIT_FAILURE);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(address);
	if (bind(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0)
	{
		/* log */
		perror("can't bind the socket");
		exit(EXIT_FAILURE);
	}

	connex = xmalloc(sizeof(struct Connexion));
	connex->address.sin_family = AF_INET;
	connex->address.sin_port = htons(port_dest);
	connex->address.sin_addr.s_addr = inet_addr(address_dest);
	connex->socket = sock;
	connex->ponderationLevel = ponderation;
	connex->active = 0;
	connex->pid = 0;
	connex->lastRead = 0;

	doHandshake(connex);

	return(connex);
}

/**
 * enable a connection
 * @param {conn} the connexion to enable
 */
void enableConn(struct Connexion *conn)
{
	/* add addr */
	log_write(LOG_NOTICE, "Enable connection with pid %d", conn->pid);
	conn->pid = 0;
	conn->active = 1;
	xclose(conn->pipe);
	if ((conn->lastRead = time(NULL)) == -1)
	{
		perror("time");
		log_write(LOG_CRIT, "can't use the fail detection feature error when using time: %s", strerror(errno));
	}
}

/**
 * if the connexion don't received something since more than Failure_detection.time (see the ini file)
 * disable it and run the handshake.
 * @param {conn} the connexion to check
 * @param {failure_detection_time} write time above which a link is marked as unactive
 */
inline int	checkIfWritable(struct Connexion *conn, const double failure_detection_time)
{
	double diff = difftime(time(NULL), conn->lastRead);

	/* for difftime see man difftime (if not created we can do it with a macro - if posix system) */
	if (conn->active == 0 || diff < failure_detection_time)
	{
		return 0;
	}

	log_write(LOG_INFO, "Disable connection last read: %d seconds", diff);
	conn->active = 0;
	doHandshake(conn);
	return -1;
}

/**
 * close properly a connexion
 * it free the memory allocated for his structure
 * @param {connex} the connexion to close
 */
void	closeConnexion(struct Connexion *connex)
{
/*	if ( connex->pid != 0)
	{
		if (kill(connex->pid, SIGTERM) == -1)
		{
			perror("kill");
		}
		else
		{
			waitpid(connex->pid, 0);
		}
	}
*/
	close(connex->socket);
	xfree(connex);
}
