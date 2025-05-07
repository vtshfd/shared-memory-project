//shm.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
void parent();
void child();
#define SHMKEY 859047
#define BUFF_SZ sizeof(int)
int main() {
switch (fork()){
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
if (shmid == -1){
perror("Parent: Error in shmget");
exit(1);
}
// Attach the shared memory segment to the process's address
space
int *pint = (int *)(shmat(shmid, 0, 0));
for (i = 0; i < 10; i++){
sleep(2);
*pint = 10 * i; /* Write into the shared area. */
printf("Parent: Written Val.: = %d\n", *pint);
}
Detach the shared memory segment when done
shmdt(pint);
//remove shared memory
shmctl(shmid, IPC_RMID, NULL);
}
void child() {
sleep(5); int i;
// Create or get a shared memory segment
int shmid = shmget(SHMKEY, BUFF_SZ, 0777);
if (shmid == -1){
perror("Child: Error in shmget");
exit(1);
}
// Attach the shared memory segment
int *cint = (int *)(shmat(shmid, 0, 0));
for (i = 0; i < 10; i++){
sleep(1);
printf("Child: Read Val. = %d\n", *cint);
}
}
=====================================================
*****************************************************
====================================================
//signalExample1.c
#include<stdio.h>
#include<signal.h>
//SIGINT, caused by Ctrl-C at keyboard
void handle_sigint(int sig){
printf(" Caught signal %d\n", sig);
}
int main() {
signal(SIGINT, handle_sigint);
printf();
while (1) ;
printf();
return 0;
}
=====================================================
*****************************************************
====================================================
//signalExample2.c
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include <stdlib.h>
Define the signal handler function
void handle_sigint(int signum) {
printf("Caught signal %d (Ctrl + C). Terminating...\n",
signum);
exit(signum);
}
int main() {
printf("Enter Ctrl + C to terminate\n");
signal(SIGINT, handle_sigint);
while (1) {
printf();
sleep(1);
}
return 0;
}
=====================================================
*****************************************************
====================================================
//child.c
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char** argv) {
printf("Hello from Child.c!\n");
printf(" I got %d arguments: \n", argc);
int i;
for (i =0; i < argc; i++){
printf("%s ", argv[i]);
sleep(2);
}
printf("\nChild is now ending.\n");
return EXIT_SUCCESS;
}
=====================================================
*****************************************************
====================================================
//parent.c
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char** argv) {
pid_t childPid = fork();
if (childPid == 0) {
printf("\n I am a child!");
printf("\n My parent's PID is: %d", getppid());
printf("\n My PID is: %d\n", getpid());
char* args[] = {"./child", "Hello", "students", "exec",
"using",NULL};
execv(args[0], args);
}
else {
printf("\n I am a parent!");
printf("\n My PID is: %d", getpid());
printf("\n My child's PID is: %d\n", childPid);
}
sleep(5);
//Send SIGINT signal to childPid
kill(childPid, SIGINT);
return EXIT_SUCCESS;
}
=====================================================
*****************************************************
====================================================

