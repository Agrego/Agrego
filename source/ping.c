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
#include "headers/ping.h"
#include "headers/log_save.h"
#include "headers/connexion.h"
#include "headers/xlib.h"
#include "headers/xiniparser.h"
#include "headers/agregate.h"

#ifdef FAILOVER

/**
 * Initialize and arm the timer which will launch a ping on all connection
 *
 * The timer created raise a signal SIGUSR1, so this function a signal handler for this signal.
 * @todo create and use variable from ini files for timer inteval
 * @param {ag} a pointer on the struct agrego, which will be used by the timer
 *
 * @TODO check what append when the timer value is 0
 */
void addtimer(struct agrego *ag)
{
	struct sigaction act;
	struct sigevent evp;
	timer_t timerid;
	struct itimerspec ts;
	double interval;

	/* catch SIGUSR1 signal */
	act.sa_sigaction = sigusr1Handler;
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		perror("sigaction");

	/* create the timer */
	memset(&evp, 0, sizeof(struct sigevent));
	evp.sigev_value.sival_ptr = ag;
	evp.sigev_notify = SIGEV_SIGNAL;
	evp.sigev_signo = SIGUSR1;
	if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1)
		perror("timer_create");

	/* arm the timer */
	interval = xiniparser_getdouble("Failure_detection:ping_interval", 0.5);

	/* first expiration time */
	ts.it_interval.tv_sec = (int)interval;
	ts.it_interval.tv_nsec = SEC2NANOSEC(interval - ts.it_interval.tv_sec);

	/* next expiration time */
	ts.it_value.tv_sec = ts.it_interval.tv_sec;
	ts.it_value.tv_nsec = ts.it_interval.tv_nsec;
	if (timer_settime(timerid, 0, &ts, NULL) == -1)
		perror("timer_settimer");
}

/**
 * Should be called when a signal SIGUSR1 is raised. It correspond to the pointer on function: struct sigaction.sa_sigaction.
 *
 * The function write the PING_MSG string on all link enabled and writable
 */
void sigusr1Handler(int __attribute__ ((unused)) sig, siginfo_t *si, void __attribute__ ((unused)) *ucontext)
{
	int cpt;
	struct agrego *ag;

	ag = si->si_ptr;

	if (ag->nbConnEnable == 0)
	{
		return ;
	}

	if (xpoll(ag->write, ag->nbConn + 1, 0) == -1)
	{
		log_write(LOG_DEBUG, "poll on write fds error, don't ping");
		return ;
	}

	for (cpt = 0; cpt < ag->nbConn; cpt++)
	{
		/* if we read there is less time than  timer_raise/2  don't write ? */
		connStillWritable(ag, cpt);

		if (ag->connArr[cpt]->active == 0
				|| !(ag->write[cpt].revents & POLL_EVENTS_OUT))
		{
			continue;
		}
		log_write(LOG_DEBUG, "ping on conn: %d", cpt);
		writeOnConn(ag->connArr[cpt], PING_MSG, PING_SIZE);
	}
}
#endif
