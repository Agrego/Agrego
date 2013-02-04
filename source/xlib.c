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

#include "headers/xlib.h"
#include "headers/log_save.h"

/**
 * Does a malloc, with some basic checks
 * it waits until no memory is available
 * @param  {size} the size to allocate
 * @return A pointer on the allocated memory address
 */
void	*xmalloc(size_t size)
{
	void	*ptr;

	if (size == 0)
		return NULL;

	ptr = NULL;
	while (ptr == NULL)
	{
		ptr = malloc(size);
	}
	return ptr;
}

/**
 * Frees var's resources, if the parameter is NULL, doesn't do anything
 * @param {ptr} a pointer
 */
void	xfree(void *ptr)
{
	if (ptr != NULL)
		free(ptr);

	ptr = NULL;
}

/**
 * Adds a signal handler
 * On error, adds a log LOG_EMERG, and exits
 * @param {signum}          the signal to handle
 * @param {handler} the callback function to call
 */
void	xsignal(int signum, __sighandler_t handler)
{
	if (signal(signum, handler) == SIG_ERR)
	{
		log_write(LOG_EMERG, "Error when adding handler to a signal: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

/**
 * Writes a buffer on a file descriptor
 * On error, it adds a log LOG_ERR
 * @param {fd} 	the file descritor where write
 * @param {buf} the buffer to write
 * @param {len} the size to write
 * @return {int}   the size writed
 */
int	xwrite(int fd, const void *buf, size_t len)
{
	int ret;

	ret = write(fd, buf, len);
	if (ret == -1)
	{
		log_write(LOG_ERR, "write: %s", strerror(errno));
		return 0;
	}
	return ret;
}

/**
 * Reads a buffer on a file descriptor
 * On error, it adds a log LOG_ERR
 * @param {fd}    the file descritor where read
 * @param {buf} the buffer to read
 * @param {len} the size to read
 * @return {int}   the size read
 */
int	xread(int fd, void *buf, size_t len)
{
	int ret;

	ret = read(fd, buf, len);
	if (ret == -1)
	{
		log_write(LOG_ERR, "read: %s", strerror(errno));
		return 0;
	}
	return ret;
}

/**
 * Closes a file descriptor
 * On error, it add a log LOG_ERR
 * @param {fd} the file to close
 */
void	xclose(int fd)
{
	if (close(fd) == -1)
	{
		log_write(LOG_WARNING, "Can't close filedescriptor :  %s", strerror(errno));
	}
}

/**
 * Closes a file
 * On error, it adds a log LOG_ERR
 * @param {file} the file to close
 */
void	xfclose(FILE *file)
{
	if (fclose(file) == EOF)
	{
		log_write(LOG_WARNING, "Can't close file %s", strerror(errno));
	}
}

/**
 * Daemonizes the current processus
 *
 * On error, this procedure adds a log LOG_EMERG and exits with EXIT_FAILURE status
 * Otherwise, it logs LOG_INFO.
 */
void	xdaemon()
{
	if (daemon(0, 0) == -1)
	{
		log_write(LOG_EMERG, "impossible to daemonize: %s. God what are you doing?", strerror(errno));
		exit(EXIT_FAILURE);
	}
	log_write(LOG_INFO, "AgreGo is now in Hellish mode ! [Daemonized]");
}

/**
 * run poll.
 * If there is an error it log on CRITICAL level if the erro is not due to a signal
 * @return {int} the integer returned by poll
 */
int	xpoll(struct pollfd *fds, nfds_t nfds, int timeout)
{
	int ret;

	if ((ret = poll(fds, nfds, timeout))  == -1)
	{
		if (errno != EINTR) /* we have received a signal */
		{
			log_write(LOG_CRIT, "poll error: %s", strerror(errno));
		}
	}
	return ret;
}
