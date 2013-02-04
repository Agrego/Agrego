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

/**
 * add a suite to a CUnit register and manage error
 * @param {suite_name} 	the suite name
 * @param {pInit} 	an initialization function will be call before call each test function
 * @param {pClean} 	a cleanup function will be call after call each test function
 * @return a CUnit suite object created with the parameters
 */
CU_pSuite xCU_add_suite(const char *suite_name, CU_InitializeFunc pInit, CU_CleanupFunc pClean)
{
	CU_pSuite pSuite;

	pSuite = CU_add_suite(suite_name, pInit, pClean);
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		exit(CU_get_error());
	}
	return pSuite;
}

/**
 * add a test to a CUnit test and manage error
 * @param {pSuite} the suite to which the test will be added
 * @param {test_name} the test name
 * @param {pTestFunc} the test function to call
 */
void xCU_add_test(CU_pSuite pSuite, const char *test_name, CU_TestFunc pTestFunc)
{
	if ((NULL == CU_add_test(pSuite, test_name, pTestFunc)))
	{
		CU_cleanup_registry();
		exit(CU_get_error());
	}
}
