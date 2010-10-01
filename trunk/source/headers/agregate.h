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
#ifndef _AGREGATE_H_
#define _AGREGATE_H_

#include "unistd.h"
#include "sys/poll.h"

#include "agrego.h"
#include "connexion.h"
#include "xiniparser.h"
#include "xlib.h"
#include "log_save.h"

#define POLL_EVENTS_IN (POLLIN | POLLPRI)
#define POLL_EVENTS_OUT POLLOUT

#define _WEIGHTING_1
/* #define _WEIGHTING_2 */

void runAgrego(struct agrego *);
void writeOnTabConn(struct agrego * , int , void *);
void readOnTabConn(struct agrego * , int , int , void *);
void getPollFds(struct agrego *);
inline void connStillWritable(struct agrego *, int);

#endif /* _AGREGATE_H_ */
