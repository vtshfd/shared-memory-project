#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMKEY 859047
#define BUFF_SZ sizeof(int)
//2
void parent();
void child();

int main() {
    switch (fork()) {
        case -1:
            perror("Failed to fork");
            return 1;
        case 0:
            child();
            break;
        default:
            parent();
            break;
    }
    return 0;
}

void parent() {
    int i;
    // Create or get a shared memory segment
    int shmid = shmget(SHMKEY, BUFF_SZ, 0777 | IPC_CREAT);
    if (shmid == -1) {
        perror("Parent: Error in shmget");
        exit(1);
    }
    // Attach the shared memory segment to the process's address space
    int *pint = (int *)(shmat(shmid, 0, 0));
    for (i = 0; i < 10; i++) {
        sleep(2);
        *pint = 10 * i; /* Write into the shared area */
        printf("Parent: Written Val.: = %d\n", *pint);
    }
    // Detach and remove the shared memory
    shmdt(pint);
    shmctl(shmid, IPC_RMID, NULL);
}

void child() {
    sleep(5);
    int i;
    // Get the shared memory segment
    int shmid = shmget(SHMKEY, BUFF_SZ, 0777);
    if (shmid == -1) {
        perror("Child: Error in shmget");
        exit(1);
    }
    // Attach the shared memory segment
    int *cint = (int *)(shmat(shmid, 0, 0));
    for (i = 0; i < 10; i++) {
        sleep(1);
        printf("Child: Read Val. = %d\n", *cint);
    }
}

