#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("Welcome to the Enchanted Fork and Exec Journey!\n");
    printf("Choose an element to transform into:\n");
    printf("1. Fire\n2. Water\n3. Earth\n4. Air\n");

    printf("Parent process: PID = %d\n", getpid());
    
    int choice; 

    scanf("%d", &choice);

    pid_t pid = fork();

    if (pid == 0) {
        printf("Child process: PID = %d\n", getpid());

        switch (choice) {
        case 1:
            execv("./fire_elemental", NULL);
            break;
        case 2:
            execv("./water_elemental", NULL);
            break;
        case 3:
            execv("./earth_elemental", NULL);
            break;
        case 4:
            execv("./air_elemental", NULL);
            break;
        default:
            printf("Invalid choice. Exiting.\n");
            exit(1);
        }
    }

    return 0;
}