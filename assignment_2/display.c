#include <stdio.h>
#include <unistd.h>
#include "display.h"

void display(char *str) {
    char *p;
    for (p=str; *p; p++) {
        write(1, p, 1);
        usleep(100);
    }
}
