# include<stdio.h>
void printValue(void *ptr, char type) {
    // The void pointer can be cast to any other pointer type
    if (type == 'i') {
        printf("vvalue: %d\n", *(int *)ptr);
    } else if (type == 'f') {
        printf("vvalue: %.2f\n", *(float *)ptr);
    } else if (type == 'c') {
        printf("vvalue: %c\n", *(char *)ptr);
    } else {
        printf("Unknown type\n");
    }
}
int main ()
{
int  intvalue = 22;
float floatvalue = 22.5;
//print different data using void pointers
printValue(&intvalue, 'i');
printValue(&floatvalue, 'f');
printValue(&intvalue, 'c');
}