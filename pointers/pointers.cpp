#include <stdio.h>

int main()
{
    int x = 10;
 
    printf("Address of x: %p\n", &x);
    printf("Value of x: %d\n", x);
    puts("");

    int* ptr = &x;
    *ptr = 20;

    printf("Address of ptr: %p\n", &ptr);
    printf("Value of ptr: %d\n", *ptr);
    printf("Address of x: %p\n", &x);
    printf("Value of x: %d\n", x);
    puts("");

 
    int y = 100;

    int** ptr2 = &ptr;
    *ptr2 = &y;
    **ptr2 = 50;

    printf("Address of ptr2: %p\n", &ptr2);
    printf("Value of ptr2: %d\n", **ptr2);
    printf("Address of ptr: %p\n", &ptr);
    printf("Value of ptr: %d\n", *ptr);
    printf("Address of y: %p\n", &y);
    printf("Value of y: %d\n", y);
    printf("Address of x: %p\n", &x);
    printf("Value of x: %d\n", x);

    return 0;
}