#include<stdio.h>
#include<stdlib.h>

int * function1(int);
int * function2(int);

int main(void)
{
	volatile int * result1,* result2;
	int val = 1000;
	result1 = function1(val);
	//printf("val: %d\n result1: %d\n *result1: %d\n", val, result1, *result1);
	fprintf(stderr,"result1 = %d\n", *result1);
	result2 = function2(val);
	fprintf(stderr,"result2 = %d\n", *result2);
	fprintf(stderr,"result1 = %d\n", *result1);
}

int * function1(int val)
{
	int result = val + 1500;
	return &result;
}

int * function2(int val)
{
	int result = val - 1500;
	return &result;
}
