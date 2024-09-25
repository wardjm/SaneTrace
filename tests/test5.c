#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char *mem = malloc(10);
    free(mem);

    strcpy(mem, argv[1]);

    return 0;
}
