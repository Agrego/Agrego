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
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 This project has begun as an "Epitech Innovative Project" in Epitech, France.
*/

#include "headers/endpoint.h"
#include "headers/xiniparser.h"
#include "headers/log_save.h"
#include "headers/xlib.h"
#include "headers/connexion.h"
#include "headers/connexiontab.h"

/**
 * Create a set of connection, according to the .ini file's options
 * and put it in the global ag->connArr (struct Connexion array)
 */
void	createInterfaces(struct agrego *ag)
{
	char	arg[50];
	char	*ipadress;
	int	port;
	char	*ipadress_dest;
	int	port_dest;
	int	ponderation;
	int	i;

	for (i = 0; i < ag->nbConn; i++)
	{
		sprintf(arg, "Link_%d:%s_ip_address_private", i+1, getEndpoint());
		ipadress = xiniparser_getstring(arg, NULL);
		if (ipadress == NULL || strlen(ipadress) == 0)
		{
			sprintf(arg, "Link_%d:%s_ip_address_public", i+1, getEndpoint());
			ipadress = xiniparser_getstring(arg, NULL);
			if (ipadress == NULL)
			{
				fprintf(stderr, "cannot read the %s_ip_address in the .ini file: %s\n", getEndpoint(), arg);
				exit(EXIT_FAILURE);
			}
		}

		sprintf(arg, "Link_%d:%s_port", i+1, getEndpoint());
		port = xiniparser_getint(arg, -1);
		if (port == -1)
		{
			fprintf(stderr, "cannot read the %s_port in the .ini file: %s\n", getEndpoint(), arg);
			exit(EXIT_FAILURE);
		}

		sprintf(arg, "Link_%d:%s_ip_address_public", i+1, getOtherEndpoint());
		ipadress_dest = xiniparser_getstring(arg, NULL);
		if (ipadress_dest == NULL)
		{
			fprintf(stderr, "cannot read the %s_ip_address in the .ini file: %s\n", getOtherEndpoint(), arg);
			exit(EXIT_FAILURE);
		}

		sprintf(arg, "Link_%d:%s_port", i+1, getOtherEndpoint());
		port_dest = xiniparser_getint(arg, -1);
		if (port_dest == -1)
		{
			fprintf(stderr, "cannot read the %s_port in the .ini file: %s\n", getOtherEndpoint(), arg);
			exit(EXIT_FAILURE);
		}

		sprintf(arg, "Link_%d:coefficient", i+1);
		ponderation = xiniparser_getint(arg, -1);
		ag->maxWeight = (ag->maxWeight < ponderation) ? ponderation : ag->maxWeight;
		if (ponderation == -1)
		{
			fprintf(stderr, "cannot read the coefficient in the .ini file: %s\n", arg);
			exit(EXIT_FAILURE);
		}
		ag->connArr[i] = initConnexion(port, ipadress, port_dest, ipadress_dest, ponderation);
	}
}

/**
 * free memory and close socket
 * @param {ag} agrego structure which contains the connexions to close
 */
void	closeInterfaces(struct agrego *ag)
{
	int cpt;

	for (cpt = 0; cpt < ag->nbConn; cpt++)
	{
		closeConnexion(ag->connArr[cpt]);
	}
}


