#include "main_service.h"

const char *path_to_my_shared_memory = "/home/jeremy/Documents/Upssitech_3A_BE_Multithreading/PID_S1.mem";
const char *path_to_my_shared_memory_increment = "/home/jeremy/Documents/Upssitech_3A_BE_Multithreading/INCREMENT_S1.mem";
const char *path_to_shared_memory_data_sensor = "/home/jeremy/Documents/Upssitech_3A_BE_Multithreading/data_capteur.mem";


int main(){

  pid_t* pid_sensor;
  pid_t* pid_serv2;
  printf("Hello, starting server 1");
    // init de la file stockant les temperatures reçues
    File = INIT_FILE(File);
    //signal(nom_du_signal, read_data);

    // Accès à la mémoire partagée du capteur (1 *float)
    float* sensor_data = (float*)attach_memory_block(path_to_shared_memory_data_sensor, sizeof(float));

    // Creation memoire partagee PID pour chien de garde et le capteur
    int* pid_adr = (int*)attach_memory_block(path_to_my_shared_memory, sizeof(int));
    *pid_adr = (int)getpid();

    
    
    // Creation memoire Increment
    int* increment_test = (int*)attach_memory_block(path_to_my_shared_memory_increment, sizeof(int));
    pthread_create((unsigned long)thread_increment, NULL, thread_increment_function, NULL);


    // Ajout des semaphores hakim ou signal pour indiquer au capteur de publier 

    add_data_to_file(*sensor_data, File);
}

// fonction appelée dans le Thread_increment pour indiquer au watchdog qu'on est en vie
void *thread_increment_function() {
  for(int i=0; i < INT_MAX; i++){
    (*increment_test) = i;
    sleep(100);
  } 
}


void add_data_to_file(float value, FIFO File){
  File = ENFILER(File, value);
}

// --------------------------- Methode File Fenetre glissante --------------------------------- //

FIFO INIT_FILE (FIFO f){
    f.queue=NULL;
    f.tete=NULL;
    return f;
}

float FILE_EST_VIDE (FIFO f){
    return(f.tete==NULL);
}

FIFO ENFILER (FIFO f, float e){
    Cellule * aux;
    aux = (Cellule *) malloc(sizeof(Cellule));
    if (aux==NULL)return f;
    aux -> valelement=e;
    aux -> suiv =NULL;
    if(f.tete==NULL) f.tete=aux;
    else f.queue->suiv=aux;
    f.queue=aux;
    return f;
}

FIFO DEFILER (FIFO f){
    if (FILE_EST_VIDE(f))return f;
    Cellule * aux;
    aux=f.tete->suiv;
    if (aux==NULL){
        f.queue=NULL;
        free(f.tete);
        f.tete=aux;
        return f;
    }
}
// ---------------------------------------------------------------------------------------------//

// void read_data(){
//     float* value_adr = shmat((int)sm_id, 0, flag);
//     value = *value_adr;
// }


