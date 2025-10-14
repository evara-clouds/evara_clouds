#include <stdio.h>

typedef enum { ON, OFF } button;
typedef enum { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday } week;

button press(button current) {
    if (current == ON)
        return OFF;
    else
        return ON;
}

void print_button(button current) {
    if (current == ON)
        printf("Power is ON\n");
    else
        printf("Power is OFF\n");
}

int main(void) {
    week today;
    button power;

    // Let's simulate each day of the week
    for (today = Sunday; today <= Saturday; today++) {
        printf("Day %d: ", today + 1);

        // Power ON during weekdays, OFF during weekend
        if (today == Sunday || today == Saturday)
            power = OFF;
        else
            power = ON;

        print_button(power);
    }

    return 0;
}
