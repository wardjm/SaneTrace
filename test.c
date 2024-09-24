#include <stdio.h>

int main(int argc, char *argv[]) {

    int i;

    printf("\nTest arguments:\n");

    for (i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    printf("I'm being traced\n");



    return 0;
}
