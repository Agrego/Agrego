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
#include <getopt.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "headers/xiniparser.h"
#include "headers/log_save.h"
#include "headers/connexiontab.h"
#include "headers/tuntap.h"
#include "headers/agrego.h"
#include "headers/agregate.h"

/**
 * Handler to call when the procesus received a SIGCHLD
 * this function waits for a son process to die
 * It correspond to the pointer on function used by the signal system call
 * @param {signal} the received signal
 * @todo do it with sigaction
 */
void	sigchldHandler(int __attribute__ ((unused)) signal)
{
	siginfo_t infop;

	log_write(LOG_DEBUG, "SIGCHLD received, wait for a son process");
	if (waitid(P_ALL, 0, &infop, WEXITED | WSTOPPED) == -1)
	{
		log_write(LOG_CRIT, "error on waitid: %s", strerror(errno));
		return ;
	}
	log_write(LOG_INFO, "exiting son's pid: %d, status: %d, code: %d", infop.si_pid, infop.si_status, infop.si_code);
}

void 	freeAll(int __attribute__ ((unused)) sig, siginfo_t *si, void __attribute__ ((unused)) *ucontext)
{
	struct agrego *ag;

	ag = si->si_ptr;
	freeAgrego(ag);
}

/**
 * Adds all signal handler
 * @todo create a handler for sigaction in xlib
 */
void	addSigHandler()
{
	/*struct sigaction act;

	act.sa_sigaction = freeAll;
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGINT, &act, NULL) == -1)
		perror("sigaction");

	if (sigaction(SIGTERM, &act, NULL) == -1)
		perror("sigaction");
*/
	xsignal(SIGCHLD, sigchldHandler);
}

/**
 * display usage message and exits
 * @param {binName} he name of the binary currently argv[0]
 */
void	usage(const char *binName)
{
	fprintf(stderr, "usage: %s [-d|--daemon] [-p|--pid-file pidfile] ini_file\n", binName);
	fprintf(stderr, "where ini_file is the path to the agrego.ini file\n");
	fprintf(stderr, "\t--daemon: turn it as a daemon\n\t--pid-file: put it pid in the file\n");
	exit(EXIT_FAILURE);
}

/**
 * retrieve options from argv, results are put in the 3 last arguments of the function
 * this function uses the getopt_long function
 * @param {argc}	number of arguments
 * @param {argv}	array of arguments
 * @param {pidFile}	where --pid-file value will be put
 * @param {configFile}	where config_file value will be put
 * @param {daemon}	1 if -d options is set 0 otherwise
 */
void	getCmdOptions(const int argc, char **argv, char **pidFile, char **configFile, int *daemon)
{
	int	option_index = 0;
	int	c;
	static struct option long_options[] =
	{
		{"pid-file", 1, NULL, 'p'},
		{"daemon", 0, NULL, 'd'},
		{"help", 0, NULL, 'd'},
		{0, 0, 0, 0}
	};

	while (1)
	{
		c = getopt_long(argc, argv, "p:dh",
				long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'p':
				*pidFile = optarg;
				break;

			case 'd':
				*daemon = 1;
				break;

			default:
				usage(argv[0]);
		}
	}

	/* agrego.ini file is missing */
	if (optind != (argc - 1))
	{
		usage(argv[0]);
	}
	*configFile = argv[optind];
}

/**
 * write the pid of the current process in a file
 * @param {pidFile} the file where to write the ini file
 * @todo chmod and check read this file to check it contains our pid (see secure prgramming cookbook)
 */
void	writePidFile(const char *pidFile)
{
	FILE *file;

	log_write(LOG_DEBUG, "write %d in file %s\n", getpid(), pidFile);
	if (NULL == (file = fopen(pidFile, "w")))
	{
		log_write(LOG_ERR, "open pid file: %s", strerror(errno));
		return ;/* exit ? */
	}
	/* chmod */
	fprintf(file, "%d\n", getpid());
	fclose(file);
}

/**
 * Agrego's main function
 *
 * Daemonizes the process
 * Adds signal handler
 * Initialize all variables needed from ini file configuration
 * Runs the server
 *
 * @param {argc} the number of arguments
 * @param {argv} the arguments table
 * @return int the exiting status
 */
int	main(int argc, char **argv)
{
	struct	agrego ag;
	char	*pidFile;
	char	*configFile;
	int 	daemon;

	pidFile = NULL;
	configFile = NULL;
	daemon = 0;

	getCmdOptions(argc, argv, &pidFile, &configFile, &daemon);

	xiniparser_load(configFile);
	if (daemon)
	{
		xdaemon();
	}
	if (pidFile)
	{
		writePidFile(pidFile);
	}

	addSigHandler();

	initAgrego(&ag);
	runAgrego(&ag);
	freeAgrego(&ag);

	return EXIT_SUCCESS;
}
