    #include "kernel/types.h"
    #include "kernel/stat.h"
    #include "user/user.h"
    #include "kernel/param.h"

    #define MAXSIZE 512

    int main(int argc, char** argv) {
        sleep(10);
        char buf[MAXSIZE];
        int new_argc = argc - 1;
        char** new_argv = malloc(MAXARG * sizeof(char*));
        
        for (int i = 0; i < new_argc; i++) {
            new_argv[i] = argv[i + 1];
        }
        new_argv[new_argc] = '\0';
        if (read(0, buf, sizeof(buf)) < 0) {
            fprintf(2, "read error\n");
            exit(1);
        }
        int len = 0;
        for (int i = 0; i < MAXSIZE; i++) {
            if (buf[i] == '\0') {
                break;
            } else if (buf[i] == '\n') {
                new_argv[new_argc][len] = '\0';
                len = 0;
                new_argc++;
            } else {
                if (!new_argv[new_argc]) {
                    new_argv[new_argc] = malloc(512);
                }
                // printf("第%d个字符:%c,对应的new_argc= %d\n", i, buf[i], new_argc);
                new_argv[new_argc][len] = buf[i];
                len++;
            }
        }
        int pid = fork();
        if (pid < 0) {
            fprintf(2, "fork error\n");
            exit(1);
        } else if (pid == 0) {
            //child
            exec(new_argv[0], new_argv);
        } else {
            // parent
            wait(0);
        }
        
        exit(0);
    }