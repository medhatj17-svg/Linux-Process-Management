
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_N 1000000

int main(void) {
    int n;

    printf("Enter the last number (N): ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid input. Enter a number from 1 to 1000000.\n");
        return 1;
    }

    int *iA = malloc((size_t)n * sizeof(int));

    if (iA == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // Parent initializes the array
    for (int i = 0; i < n; i++) {
        iA[i] = i + 1;
    }

    printf("I am the Parent\n");
    fflush(stdout);

    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("Failed to create child process");
        free(iA);
        return 1;
    }

    else if (child_pid == 0) {

        // Child calculates the sum of odd numbers
        long long sum = 0;

        for (int i = 0; i < n; i++) {
            if (iA[i] % 2 != 0) {
                sum += iA[i];
            }
        }

        printf("The child says the sum of odd numbers is: %lld\n", sum);
        fflush(stdout);

        pid_t grandchild_pid = fork();

        if (grandchild_pid < 0) {
            perror("Failed to create grandchild process");
            free(iA);
            _exit(1);
        }

        else if (grandchild_pid == 0) {

            // Grandchild counts multiples of 3
            int count = 0;

            for (int i = 0; i < n; i++) {
                if (iA[i] % 3 == 0) {
                    count++;
                }
            }

            printf("The grandchild says the number of the products of three is: %d\n", count);
            fflush(stdout);

            free(iA);
            _exit(0);
        }

        else {
            // Child waits for Grandchild
            if (waitpid(grandchild_pid, NULL, 0) == -1) {
                perror("waitpid for grandchild");
                free(iA);
                _exit(1);
            }

            printf("I am the Child\n");
            fflush(stdout);

            free(iA);
            _exit(0);
        }
    }

    else {
        // Parent waits for Child
        if (waitpid(child_pid, NULL, 0) == -1) {
            perror("waitpid for child");
            free(iA);
            return 1;
        }
    }

    free(iA);
    return 0;
}
