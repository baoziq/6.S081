#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void my_prime(int fd) {
    int prime;
    int num;
    int new_fd[2];
    if (read(fd, &prime, sizeof(prime)) == 0) {
        close(fd);
        exit(0);
    }
    printf("prime %d\n", prime);
    if (pipe(new_fd) == -1) {
        fprintf(2, "pipe error\n");
    }
    int pid = fork();
    if (pid == -1) {
        fprintf(2, "fork error\n");
    } else if (pid == 0) {
        // child
        close(new_fd[1]);
        my_prime(new_fd[0]);
        close(new_fd[0]);
    } else {
        // parent
        close(new_fd[0]);
        while (read(fd, &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(new_fd[1], &num, sizeof(num));
            }
        }
        close(new_fd[1]);
        wait(0);
    }
    exit(0);
}   

int main(int argc, char* argv[]) {
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(2, "pipe error\n");
        exit(1);
    }
    int pid = fork();
    if (pid == -1) {
        fprintf(2, "fork error\n");
    } else if (pid == 0) {
        // child
        close(fd[1]);
        my_prime(fd[0]);
        close(fd[0]);
    } else {
        // parent
        close(fd[0]);
        for (int i = 2; i <= 35; i++) {
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);
}   