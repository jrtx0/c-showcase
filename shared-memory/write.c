#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFSZ 512

int main(int argc, char *argv[])
{
    int shmid;
    int ret;
    key_t key;
    char *shmadd;

    // create key value
    key = ftok("../", 2015);
    if (key == -1)
    {
        perror("ftok");
    }

    /* 
        #include <sys/ipc.h>
        #include <sys/shm.h>

        int shmget(key_t key, size_t size, int shmflg); // create or open shared memory
    */
    // create shared memory
    shmid = shmget(key, BUFSZ, IPC_CREAT|0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(-1);
    }

    /*
        #include <sys/types.h>
        #include <sys/shm.h>

        void *shmat(int shmid, const void *shmaddr, int shmflg); // map shared memory with process
    */
    // mapping
    shmadd = shmat(shmid, NULL, 0);
    if (shmadd < 0)
    {
        perror("shmat");
        _exit(-1);
    }

    // copy data to shared memory
    printf("copy data to shared memory.\n");
    bzero(shmadd, BUFSZ); // clear shared memory
    strcpy(shmadd, "how are you?");

    return 0;
}

/* compile command: gcc write.c -o write */