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
#include "headers/xiniparser.h"
#include "headers/log_save.h"

/**
 * @param {iniDict} the dictionnary that will contain all configuration variables
 */
static dictionary	*iniDict;

/**
 * loads the agrego.ini file in the dictionnary iniDict
 * @param {filename} the agrego.ini file name
 *
 * @todo put all default value in macros
 */
void	xiniparser_load(const char *filename)
{
	iniDict = iniparser_load(filename);
	if (iniDict == NULL)
	{
		fprintf(stderr, "cannot parse file: %s\n", filename);
		exit(EXIT_FAILURE);
	}
	log_write(LOG_NOTICE, "configuration file has been parsed");
}

/**
 * Gets the string associated to a key using an already initialized and global dictionnary
 * @param    key     Key string to look for
 * @param    def     Default value to return if key not found.
 * @return   pointer to statically allocated character string
 */
char	*xiniparser_getstring(const char *key, char *def)
{
	return iniparser_getstring(iniDict, key, def);
}

/**
 * Gets the string associated to a key, convert to an int using an already initialized and global dictionnary
 * @param    key Key string to look for
 * @param    def Value to return in case of error
 * @return   integer
 */
int	xiniparser_getint(const char *key, int def)
{
	return iniparser_getint(iniDict, key, def);
}

/**
 * Gets the string associated to a key, convert to an float using an already initialized and global dictionnary
 * @param    key Key string to look for
 * @param    def Value to return in case of error
 * @return   double
 */
double	xiniparser_getdouble(const char * key, double def)
{
	return iniparser_getdouble(iniDict, key, def);
}
