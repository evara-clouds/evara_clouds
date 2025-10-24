#include <stdio.h>
#include <string.h> 

//student:name,age,gpa
//Structs are user-defined data types that allow you to group different types of variables together under a single name. This is useful for organizing related data.
struct student {
    char name[50];
    int age;
    float mrks [100];
};
int main(void) {
    struct student s1;
    strcpy
   (s1.name ,"Evara_Clouds");
    s1.age = 20;
    s1.mrks[0] = 95.5;
    s1.mrks[1] = 55;
    s1.mrks[2] = 95;

    

    printf("Name: %s\n", s1.name);
    printf("Age: %d\n", s1.age);
    printf("Marks[0,1,2]:");
for (int i=0;i<3;i++){
    printf(" %.2f", s1.mrks[i]);
}
    return 0;
}