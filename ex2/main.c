#include <stdio.h>

void swap_pointer(int **pp , int **pq ){
    /*
        pp - ptr to p that points to a
        pq - ptr to q that points to q

        since we know it's an int we can use int *ptr
        but another solution would be to use type of and cast it
    */


    int *address_of_a = *pp;
    int *address_of_b = *pq;

    printf("inside of swap_pointer: address of a: %p and b: %p\n", address_of_a, address_of_b);

    *pp = address_of_b;
    *pq = address_of_a;

}


void swap_content(int **pp , int **pq ){
    /*
        pp - ptr to p that points to a
        pq - ptr to q that points to q

        since we know it's an int we can use int *ptr
        but another solution would be to use type of and cast it
    */

    int value_of_a = **pp;
    int value_of_b = **pq;

    printf("inside of swap_content: value of a: %d and b: %d\n", value_of_a, value_of_b);

    **pp = value_of_b;
    **pq = value_of_a;

}

int main(void){
    int a = 1;
    int b = 2;
    // for testing we use pointers to integers
    int *p = &a;
    int *q = &b;

    printf("content of p (address of a): %p and content of q (address of b): %p\n", p, q);
    printf("address of a: %p and b: %p\n", &a, &b);
    // *p goes to the address p stores and gets the value stored at that address
    printf("content of a: %d and content of b: %d\n", *p, *q);

    swap_content( &p , &q );
    printf("content of p (address of a): %p and content of q (address of b): %p\n", p, q);
    // prints p = &b and q = &a
    printf("content of a: %d and content of b: %d\n", *p, *q);


    printf("swap pointer function \n");


    printf("content of p (address of a): %p and content of q (address of b): %p\n", p, q);
    printf("content of a (*p): %d and content of b (*q): %d\n", *p, *q);

    swap_pointer( &p , &q );
    printf("content of p (address of a): %p and content of q (address of b): %p\n", p, q);
    // prints p = &b and q = &a
    printf("content of a: %d and content of b: %d\n", *p, *q);


    return 0;
}


