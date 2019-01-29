/* calculate.c */
#include <stdio.h>
#include "calculator.h"

int main() {
	int ret;

	printf("3 + 4 = %d\n", add(3, 4));
	printf("5 – 3 = %d\n", sub(5, 3));
	printf("8 * 2 = %d\n", mul(8, 2));
	printf("7 / 3 = %.1f\n", div(7, 3));

	return 0;
}

