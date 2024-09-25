#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char *mem = malloc(10);

    strcpy(mem, argv[1]);

    free(mem);

    return 0;
}
