#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include "segdef.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

static int shm_id=0;
static int sem_id=0;
static segment* seg;
static int req = 0;
int result=0;
long tab[maxval];


long result_client(long tab[]) {
    long somme = 0;

    for (int i = 0; i < maxval; i++) {
        somme += tab[i];
    }

    return somme / maxval;
}


static void init_client(void){
    shm_id = shmget(cle, segsize, 0);
    if (shm_id == -1) {
        perror("Erreur shmget");
        _exit(1);
    }

    sem_id = semget(cle, 3, 0);
    if (sem_id == -1) {
        perror("Erreur semget");
        _exit(1);
    }

    seg = (segment*)shmat(shm_id, NULL, 0);
    if (seg == (void*)-1) {
        perror("Erreur shmat");
        _exit(1);
    }
    init_rand();
}



int main(){
    int counter_failed = 0;
    long local_tab[maxval];
    init_client(); //step 0
    printf("intialisation done \r\n");
    int n_clients = 5; 
    for (int i = 0; i < n_clients - 1; i++) {
        if (fork() == 0) break;
    }

    while(1){
        acq_sem(sem_id, seg_dispo); //step 1
        seg->pid = getpid();
        seg->req = ++req;
        for (int i = 0; i < maxval; i++) seg->tab[i] = getrand();
        acq_sem(sem_id, seg_init); //step 2
        wait_sem(sem_id,res_ok); //step 3
        lib_sem(sem_id,seg_init); //step 4
        //printf("results from servor is %ld\n",seg->result);
        wait_sem(sem_id,res_ok);
    
        long local_result = seg->result;
        memcpy(local_tab, seg->tab, sizeof(long) * maxval);

        //6
        lib_sem(sem_id,seg_dispo);

        //7
        printf("%ld\r\n", local_result);
        printf("%ld\r\n", result_client(local_tab));
        if(local_result == result_client(local_tab)){
        printf("TEST PASSED \r\n");
        } else {
            printf("TEST FAILED \r\n");
            counter_failed++;
        }
        printf("number of test failed is : %d\r\n", counter_failed);
        printf("----------------\r\n");
    }
    return 0;
}