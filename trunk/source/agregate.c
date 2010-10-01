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

#include "headers/agregate.h"
#include "headers/connexiontab.h"
#include "headers/ping.h"

/**
 * Runs the aggregation system using the global ag->connArr
 * @param {ag} the agrego structure
 */
void	runAgrego(struct agrego *ag)
{
	/* add don't return compilator attribute */
	int	bufSize;
	void 	*buffer;
	nfds_t 	nbFds; /* might be in agrego struct with struct pollfd */
	int 	nbFdsReadEvent;

	bufSize = xiniparser_getint("Network:MTU", 1500);
	buffer = xmalloc(bufSize * sizeof(char));

	nbFds = ag->nbConn + 1;
	ag->read = xmalloc(nbFds * sizeof(struct pollfd)); /* use calloc ??? */
	ag->write = xmalloc(nbFds * sizeof(struct pollfd)); /* use calloc ??? */

	getPollFds(ag);
#ifdef FAILOVER
	addtimer(ag);
#endif
	while (42)
	{
		if ((nbFdsReadEvent = xpoll(ag->read, nbFds, -1))  == -1)
		{
			/* the error must be due to a signal, see xpoll in xlib */
			continue;
		}

		if (ag->read[ag->nbConn].revents & POLL_EVENTS_IN && xpoll(ag->write, nbFds, 0) > 0)
		{
			nbFdsReadEvent--;
			writeOnTabConn(ag, bufSize, buffer);
		}
		else
		{
			/* else log WARN ? */
		}

		/* poll on tun to know if it is writable */
		if (nbFdsReadEvent > 0)
		{
			readOnTabConn(ag, nbFdsReadEvent, bufSize, buffer);
		}
	}

	xfree(buffer); /* will be never used because the loop will never break we stop when receive signal */
	xfree(ag->write);
	xfree(ag->read);
}

/**
 * Reads on tun interface, and chooses the good interface to write to
 * @param {ag} 		the agrego structure
 * @param {bufSize} 	the buffer size
 * @param {buffer} 	the buffer
 */
void writeOnTabConn(struct agrego *ag, int bufSize, void *buffer)
{
	size_t	len;
	int	i;

	i = -1;

#ifdef _WEIGHTING_2
	do
	{
		if (ag->curConn == (ag->nbConn - 1))
		{
			ag->curConn = 0;
			ag->curWeight = (ag->curWeight == ag->maxWeight) ? 1 : ag->curWeight + 1;
		}
		else
		{
			(ag->curConn)++;
		}

#ifdef FAILLOVER
		connStillWritable(ag, ag->curConn); /*necessary ?? */
#endif /*FAILOVER*/
		i++;
	} while (i < ag->nbConn && (ag->curWeight > ag->connArr[ag->curConn]->ponderationLevel
				|| ag->connArr[ag->curConn]->active == 0
				|| !(ag->write[ag->curConn].revents & POLL_EVENTS_OUT)));
#else
	do
	{
#ifdef _WEIGHTING_1
		if (ag->curWeight == ag->connArr[ag->curConn]->ponderationLevel)
		{
			ag->curWeight = 1;
#endif /*_WEIGHTING_1 */
			ag->curConn = (ag->curConn == (ag->nbConn - 1)) ? 0 : ag->curConn + 1;
			i++;
#ifdef _WEIGHTING_1
		}
		else
		{
			(ag->curWeight)++;
		}
#else /* no weighting do this only for compilation put it out this while */
		(ag->curWeight) = 0;
#endif /*_WEIGHTING_1 */

#ifdef FAILLOVER
		connStillWritable(ag, ag->curConn);/*necessary ?? */
#endif /*FAILOVER*/
	} while (i < ag->nbConn && (ag->connArr[ag->curConn]->active == 0
				|| !(ag->write[ag->curConn].revents & POLL_EVENTS_OUT)));

#endif /*_WEIGHTING_2 */
	if (i == ag->nbConn)
	{
		/* log no conn available to write */
		log_write(LOG_WARNING, "No connection available to write on");
		return ;
	}

	log_write(LOG_DEBUG, "Read on Tun");
	len = xread(ag->tunFd, buffer, bufSize);
	log_write(LOG_DEBUG, "Write on conn: %d", ag->curConn);
	len = writeOnConn(ag->connArr[ag->curConn], buffer, len);
}

/**
 * read on all connection (in the global ag->connArr) and write that on the tun interface
 * @param {ag}		the agrego structure
 * @param {nbFdsReadEvent} the number of event that the raise the poll
 * @param {bufSize} 	buffer size
 * @param {buffer} 	the buffer should be already malloc'd, will not be free'd
 */
void	readOnTabConn(struct agrego *ag, int nbFdsReadEvent, int bufSize, void *buffer)
{
	int	len;
	int	cpt;
	char	c;
	/* add break if we have found all socket that can be readed */

	for (cpt = 0; cpt < ag->nbConn && nbFdsReadEvent; cpt++)
	{
		if (!(ag->read[cpt].revents & POLL_EVENTS_IN))
		{
			continue;
		}
		nbFdsReadEvent--;

		if (ag->connArr[cpt]->active == 0)
		{
			/* it's a pipe */
			len = xread(ag->read[cpt].fd, (void *)&c, 1);
			if (len != 1 || c != 'a')
			{
				continue;
			}
			enableConn(ag->connArr[cpt]);
			/* modify or add event */
			ag->read[cpt].fd = ag->connArr[cpt]->socket;
			ag->read[cpt].events = POLL_EVENTS_IN;
			ag->write[cpt].events = POLL_EVENTS_OUT;

			if (ag->nbConnEnable == 0)
			{
				/* we can read on tun now */
				log_write(LOG_WARNING, "New connection is available, start to look at the tun");
				ag->read[ag->nbConn].events = POLL_EVENTS_IN;
/*				ag->write[ag->nbConn].events = POLL_EVENTS_OUT; add poll before readonconn */
			}
			ag->nbConnEnable++;
			continue;
		}

		log_write(LOG_DEBUG, "Read on conn: %d", cpt);
		len = readOnConn(ag->connArr[cpt], buffer, bufSize);
#ifdef FAILOVER
		if (len == PING_SIZE && !strncmp(buffer, PING_MSG, PING_SIZE))
		{
			log_write(LOG_DEBUG, "ping message have been received, don't write it on TUN: %s", buffer);
			continue;
		}
#endif
		log_write(LOG_DEBUG, "Write on tun %d", len);
		xwrite(ag->tunFd, buffer, len);
	}
}

/**
 * initialize structure needed for poll
 * @param {ag}	the agrego structure
 */
void	getPollFds(struct agrego *ag)
{
	int i;

	for (i = 0; i < ag->nbConn; i++)
	{
		ag->write[i].fd = ag->connArr[i]->socket;
		/* must be useless */
		if (ag->connArr[i]->active == 1)
		{ /* watch socket */
			ag->read[i].fd = ag->connArr[i]->socket;
			ag->read[i].events = POLL_EVENTS_IN;

			ag->write[i].events = POLL_EVENTS_OUT;
		}
		else
		{ /* watch pipe */
			ag->read[i].fd = ag->connArr[i]->pipe;
			ag->read[i].events = POLL_EVENTS_IN;

			ag->write[i].events = 0;
		}
	}

	ag->read[ag->nbConn].fd = ag->tunFd;
	ag->write[ag->nbConn].fd = ag->tunFd;
	ag->read[ag->nbConn].events = 0;
	ag->write[ag->nbConn].events = 0;
}

/**
 * check if a connexion is still writable, if not it's disabled and if no connection is available, agrego is disabled
 *
 * @param {ag} the agrego structure used to modify the pollfd struct in it
 * @param {connIndex} the index of the connexion to check
 */
inline void connStillWritable(struct agrego *ag, int connIndex)
{
	if (checkIfWritable(ag->connArr[connIndex], ag->failure_detection_time) == 0)
	{
		return ;
	}

	/* look at the pipe */
	log_write(LOG_WARNING, "Connection: %d, is disabled, we don't use it until son do the handshake", connIndex);
	ag->read[connIndex].fd = ag->connArr[connIndex]->pipe;
	ag->read[connIndex].events = POLL_EVENTS_IN;

	ag->write[connIndex].events = 0;

	if (ag->nbConnEnable == 0)
	{
		/* we stop read on tun now */
		log_write(LOG_WARNING, "No connection is available, stop to look at the tun");
		ag->read[ag->nbConn].events = 0;
/*		ag->write[ag->nbConn].events = 0; not needed yet (uncomment when poll had been added before readOnConn) */
	}
}
