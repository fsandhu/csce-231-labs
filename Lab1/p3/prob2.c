#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct x{
	int i1;
	int i2;
	char ch1[8];
	char ch2[8];
};

struct y{
	long long int f1;
	char f2[18];
};

int main(void)
{
	struct x * myX;
	struct y * myY;
	myX = malloc(sizeof(struct x));
	myX->i1 = 4096;
	myX->i2 = 4096;
	strcpy(myX->ch1,"Witawas ");
	strcpy(myX->ch2,"Srisaan");
	myY = (struct y *) myX;
	printf("myY->f1 = %llx\n", myY->f1);
	printf("myY->f2 = %s\n", myY->f2);
}
