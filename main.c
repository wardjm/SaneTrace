#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/user.h> // struct user_regs_struct location
#include <inttypes.h>
#include <string.h>

uint64_t find_lib_start(pid_t pid, char *lib) {

    char proc_str[32];
    FILE *f;
    char *line = NULL; // required for allocating in getline
    size_t line_len = 0; // required for allocating in getline
    char *p;
    uint64_t result = 0;

    snprintf(proc_str, sizeof(proc_str), "/proc/%d/maps", pid);
    
    f = fopen(proc_str, "r");

    if (!f) {
        printf("Failed to open %s\n", proc_str);
        return result;
    }

    printf("looking for %s\n", lib);
    while (getline(&line, &line_len, f) >= 0) {
        printf("%s\n", line);
        p = strstr(line, lib);
        if (p) {
            // Found libc, but need it to be executable
            printf("found\n");
            if (strstr(line, "r-xp")) {
                // Found executable libc
                result = strtol(line, NULL, 16);
                free(line);
                break;
            }
        }
        free(line);
        line = NULL;
        line_len = 0;
    }

    fclose(f);
    return result;
}

int main(int argc, char *argv[]) {

    pid_t pid;
    int res;
    int fd;
    int i;
    int status;
    struct user_regs_struct regs;
    uint64_t remote_libc_start;
    uint8_t inited = 0;
    pid_t ppid;
    char *newargv[10];

    if (argc == 1) {
        printf("Usage: %s <prog> <prog args>\n", argv[0]);
        return 1;
    }

    if (argc >= 10) {
        printf("Too many args\n");
        return 2;
    }

    ppid = getpid();
    printf("Parent pid = %d\n", ppid);

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

    printf("Parent malloc = %p\n", (void *)malloc);

    // Adult used for analysis
    printf("child pid = %d\n", pid);

    waitpid(pid, &status, 0);

    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    printf("Child loc = %p\n", (void *)regs.rip);

    // TODO: We are still in ld-linux.so in child, need to progress to main before starting analysis

    while (waitpid(pid, &status, 0) && !WIFEXITED(status)) {

        if (!inited) {
            // TODO: Validate is this good enough on other systems?
            // What about non-libc allocators?
            remote_libc_start = find_lib_start(pid, "libc");
            printf("libc = %"PRIx64"\n", remote_libc_start);
            inited = 1;
        }
        
        //break;
        // Do stuff!
    }    

    return 0;
}
