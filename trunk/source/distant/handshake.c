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
#include "../headers/log_save.h"
#include "../headers/handshake.h"
#include "../headers/connexion.h"

/**
 * runs the handshake for the distant (it is in client mode)
 * this function will be different for the server and the client
 * @param {connex} the connexion to open
 * @return 0 if the handshake doesn't ok, 1 else
 */
int	checkMyHand(struct Connexion *connex)
{
	unsigned char *buffer;

	buffer = xmalloc(PUBKEY_LENGTH + SHA_DIGEST_LENGTH + 1);

 	genPublicKey(buffer);
	getHash(buffer, buffer + PUBKEY_LENGTH);
	log_write(LOG_DEBUG, "send public key and hash");
	writeOnConn(connex, (void *)buffer, PUBKEY_LENGTH + SHA_DIGEST_LENGTH + 1);

	if (waitForRead(connex->socket))
	{
		readOnConn(connex, (void *)buffer, PUBKEY_LENGTH + SHA_DIGEST_LENGTH + 1);
		log_write(LOG_DEBUG, "read public key and hash");

		if (checkHash(buffer, buffer + PUBKEY_LENGTH))
		{
			log_write(LOG_DEBUG, "good hash send HANDSHAKE_MSG_OK");
			writeOnConn(connex, (void *) HANDSHAKE_MSG_OK, strlen(HANDSHAKE_MSG_OK) * sizeof(char));

			free(buffer);
			return 1;
		}
		else
		{
			log_write(LOG_WARNING, "Bad check hash received returned false");
		}
	}
	else
	{
		log_write(LOG_WARNING, "Nothing to read stop to wait");
	}

	free(buffer);
	return 0;
}
