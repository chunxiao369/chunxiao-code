#include <stdio.h>
#include "moduleA.h"
#include "moduleB.h"

int main()
{
	printf("in main\n");
	func_a();
	func_b();
	return 0;
}
