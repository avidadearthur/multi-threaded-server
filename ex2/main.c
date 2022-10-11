#include<stdio.h>

typedef enum{false, true} bool;

int main(){
	char    a = 'A';
	int     b = 12;
	short int sb = 12;
	long  int lb = 12;
	float   c = 12.0f;
	double  d = 12.0;
	int*	g ;
	const bool char_is_signed = (char)-1 < 0; // (char) type cast 
						  // in C you can store anything in any datatype 

	printf("\nSize of char a: %ld",sizeof(a));
	printf("\nSize of int b: %ld",sizeof(b));
	printf("\nSize of short int b: %ld",sizeof(sb));
	printf("\nSize of long int b: %ld",sizeof(lb));
	printf("\nSize of float c: %ld",sizeof(c));
	printf("\nSize of double d: %ld",sizeof(d));
        printf("\nSize of void: %ld",sizeof(void));
	printf("\nIs char unsigned?: %s", char_is_signed ? "true" : "false");
        printf("\nSize of pointer g: %ld \n",sizeof(g));
	// printf("\nSize of str: %ld\n",sizeof(str));

	return 0;
}
