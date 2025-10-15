#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include "segdef.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

static int shm_id=0;
static int sem_id=0;
static segment* seg;
static int req = 0;
int result=0;
long tab[maxval];


long moyenne_client(long tab[]) {
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
    init_client(); //step 0
    acq_sem(sem_id, seg_dispo); //step 1
    seg->pid = getpid();
    seg->req = ++req;
    for (int i = 0; i < maxval; i++) seg->tab[i] = getrand();
    acq_sem(sem_id, seg_init); //step 2
    wait_sem(sem_id,res_ok); //step 3
    lib_sem(sem_id,seg_init); //step 4
    printf("results from servor is %ld\n",seg->result); 
    wait_sem(sem_id,res_ok);


    //6
    lib_sem(sem_id,seg_dispo);

    //7
    printf("results from servor is %ld\n",seg->result);
    printf("results from client is %ld\n",moyenne_client(seg->tab));

    //8
    //tqt ça boucle
    return 0;
}