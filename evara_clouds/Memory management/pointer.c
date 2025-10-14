//pointers and dynamic memory
#include <stdio.h>

int main ()
{//Pointers are variables that store the memory address of another variable. They are crucial for dynamic memory management and data structures
int age = 20;
printf("vvalue of Age: %d\n", age);
int *pAge = &age; //Pointer variable that stores the address of age variable
//The address-of operator (&) is used to get the address of a variable, while the dereference operator (*) is used to access the value at that address.
printf("vvalue of Age: %d\n", pAge);
printf("vvalue of Age: %d\n", &age);
printf("Address of age variable: %p\n", &age);
printf("Address of age variable: %p\n", pAge);
printf("size of age variable: %d bytes\n", sizeof(age));
printf("size of pAge variable: %d bytes\n", sizeof(pAge));
    return 0;
}