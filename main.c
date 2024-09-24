#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/user.h> // struct user_regs_struct location

int main(int argc, char *argv[]) {

    pid_t pid;
    int res;
    int fd;
    int i;
    int status;
    struct user_regs_struct regs;

    char *newargv[10];

    if (argc == 1) {
        printf("Usage: %s <prog> <prog args>\n", argv[0]);
        return 1;
    }

    if (argc >= 10) {
        printf("Too many args\n");
        return 2;
    }

    pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        return -3;
    }
    
    if (pid == 0) {
        // Child
        newargv[0] = argv[1];

        for (i = 0; i < argc-1; i++) {
            newargv[i+1] = argv[i+2];
        }

        res = ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        if (res < 0) {
            printf("Failed to traceme on target\n");
            return -1;
        }

        if (execv(newargv[0], newargv) == -1) {
            printf("Execv failed\n");
            return -2;
        }
        // Child is running
        exit(0);
    }

    // Adult used for analysis

    while (waitpid(pid, &status, 0) && !WIFEXITED(status)) {
        // Do stuff!
    }    

    return 0;
}
