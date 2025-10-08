#include <stdio.h>
int main() {
int time = 20;
if (time < 18) {
  
  printf("Goodbye!");
} else {
  printf("Good evening!");
}
// Output -> "Good evening!"
int time = 22;
if (time < 10) {
  printf("Good morning!");
} else if (time < 20) {
  printf("Goodbye!");
} else {
  printf("Good evening!");
}
// Output -> "Good evening!"
return 0;}
