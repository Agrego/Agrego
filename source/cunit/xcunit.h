#ifndef _MY_CUNIT_H_
#define _MY_CUNIT_H_

#include <stdlib.h>
#include <CUnit/Basic.h>

CU_pSuite xCU_add_suite(const char *, CU_InitializeFunc , CU_CleanupFunc );
void xCU_add_test(CU_pSuite pSuite, const char *, CU_TestFunc );

#endif /* _MY_CUNIT_H_ */
