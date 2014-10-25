/** @file Tests_Assert.c
 * Test the assert() macro.
 * @author Adrien RICCIARDI
 * @version 1.0 : 12/06/2014
 */
#include <assert.h>
#include <stdio.h>

void TestsAssert(void)
{
	printf("Trying valid assertions...\n");
	assert(1 == 1);
	assert(3 > 2);

	printf("Trying false assertion (the program must crash to make the test succeed)...\n");
	assert(0 == 1);
}
