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

#include "xcunit.h"
#include "../headers/handshake.h"
#include "../headers/xlib.h"

int initHandshake(void)
{
	return 0;
}

int cleanHandshake(void)
{
	return 0;
}

/**
 * Test that the length of the string put in publicKey is equal to 57
 * this test work with this public key but can failed with another I don't know why
 */
void test_getHash()
{
	unsigned char *buffer;

	buffer = xmalloc(PUBKEY_LENGTH + SHA_DIGEST_LENGTH + 1);

	strcmp((char *)buffer, "-=!Sun/';2009-05-10$22:12:09[&(May");
	getHash(buffer, buffer + PUBKEY_LENGTH);
	CU_ASSERT(SHA_DIGEST_LENGTH == strlen((char *)buffer + PUBKEY_LENGTH));
	xfree(buffer);
}

void test_checkHash_ok()
{
	CU_ASSERT(0 == checkHash((unsigned char *)"A beautiful public key of 32 len", (unsigned char *)"a bad hash :("));
}

void addHandshake()
{
	CU_pSuite pSuite;

	pSuite = xCU_add_suite("Handshake_suite", initHandshake, cleanHandshake);
	xCU_add_test(pSuite, "getHash", test_getHash);
	xCU_add_test(pSuite, "checkHash Ok", test_checkHash_ok);
}
