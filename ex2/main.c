#include<stdio.h>

int main(){
	char    a = 'A';
	int     b = 12;
	float   c = 12.0f;
	double  d = 12.0;
	int*	g ;

	printf("\nSize of char a: %ld",sizeof(a));
	printf("\nSize of int b: %ld",sizeof(b));
	printf("\nSize of float c: %ld",sizeof(c));
	printf("\nSize of double d: %ld",sizeof(d));
        printf("\nSize of void: %ld",sizeof(void));
        printf("\nSize of pointer g: %ld \n",sizeof(g));
	// printf("\nSize of str: %ld\n",sizeof(str));

	return 0;
}
