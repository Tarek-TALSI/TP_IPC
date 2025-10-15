#include "segdef.h"
#include "stdlib.h"
#include "time.h"



void init_rand() {
    srand(time(NULL));
}

long getrand(){
    return (long) rand()%100;

}

void wait_sem(int semid,int sem){
    
    while(sem<){

    }
}
void acq_sem(int semid,int sem){

}
void lib_sem(int semid,int sem){

}

