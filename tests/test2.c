#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int i;
    char *mem = malloc(10);

    for (i = 0; i < 11; i++) {

        printf("%d\n", i);

        // Invalid access
        *(mem + i) = i;
    }

    // Memory leak

    return 0;
}
