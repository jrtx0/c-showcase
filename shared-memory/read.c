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

    system("ipcs -m"); // check shared memory

    /*
        #include <sys/ipc.h>
        #include <sys/shm.h>

        int shmget(key_t key, size_t size, int shmflg); // create or open shared memory
    */
    // open shared memory
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
        exit(-1);
    }

    // read shard memory data
    printf("data = [%s]\n", shmadd);

    /*
        #include <sys/types.h>
        #include <sys/shm.h>

        int shmdt(const void *shamaddr); detach shared memory with process
    */
    // detach shared memory and current process
    ret = shmdt(shmadd);
    if (ret < 0)
    {
        perror("shmdt");
        exit(1);
    }
    else
    {
        printf("deleted shared-memory.\n");
    }

    /*
        #include <sys/ipc.h>
        #include <sys/shm.h>

        int shmctl(int shmid, int cmd, struct shmid_ds *buf); // control shared meory
    */
    // delete shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

/* compile command: gcc read.c -o read */