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

#include "headers/tuntap.h"
#include "headers/xiniparser.h"

/**
 * open the tun interface and configure it
 */
int	tun_alloc()
{
	struct ifreq	ifr;
	int	fd;
	int	err;
	char	*dev;

	if ((fd = open("/dev/net/tun",O_RDWR)) < 0)
	{
		perror("open");
		exit(errno);
	}
	memset(&ifr, 0, sizeof(ifr)); /* fills struct with 0 */

	ifr.ifr_flags = IFF_TUN;

	dev = xiniparser_getstring("Interfaces:tun_dev_name", "agrego0");
	if (*dev)
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	if( (err=ioctl(fd, TUNSETIFF, (void *)&ifr) < 0))
	{
		close(fd);
		return err;
	}
	return fd;
}
