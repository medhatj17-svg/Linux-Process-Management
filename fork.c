#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    printf("Before fork: PID = %ld, Parent PID = %ld\n",
           (long)getpid(), (long)getppid());

    fflush(stdout);

    pid_t state = fork();

    if (state < 0) {
        perror("Failed to create child");
        return 1;
    }
    else if (state == 0) {
        sleep(2);

        printf("This is the Child: PID = %ld, Parent PID = %ld\n",
               (long)getpid(), (long)getppid());
    }
    else {
        printf("This is the Parent: PID = %ld\n", (long)getpid());
        printf("The Child ID is: %ld\n", (long)state);

        waitpid(state, NULL, 0);
    }

    printf("Process %ld has finished its work.\n", (long)getpid());

    return 0;
}
