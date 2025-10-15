#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include "segdef.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int shm_id=0;
int sem_id=0;
segment* seg;
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
    int sem_id = semget(cle, 3, 0);
    segment* seg = (segment*)shmat(shm_id, NULL, 0);
    init_rand();
}



int main(){
    init_client(); //step 0
    acq_sem(shm_id, seg_dispo); //step 1
    acq_sem(shm_id, seg_init); //step 2
    wait_sem(shm_id,res_ok); //step 3
    lib_sem(shm_id,seg_init); //step 4
    printf("results from servor is %ld\n",seg->result); 
    wait_sem(shm_id,res_ok);


    //6
    lib_sem(shm_id,seg_dispo);

    //7
    printf("results from servor is %ld\n",seg->result);
    printf("results from client is %ld\n",moyenne_client(seg->tab));

    //8
    //tqt ça boucle
    return 0;
}