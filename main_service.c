#include "main_service.h"

const char *path_to_my_shared_memory = "/home/jeremy/Documents/Upssitech_3A_BE_Multithreading/PID_S1.mem";
const char *path_to_my_shared_memory_increment = "/home/jeremy/Documents/Upssitech_3A_BE_Multithreading/INCREMENT_S1.mem";
const char *path_to_shared_memory_data_sensor = "/home/jeremy/Documents/Upssitech_3A_BE_Multithreading/data_capteur.mem";




// float mean (float* tab, int size){
//   int temp = 0;
//   for(int i=0; i<size; i++){
//     temp += tab[i];
//   }
//   return(temp/size)
// }

// fonction appelée dans le Thread_increment pour indiquer au watchdog qu'on est en vie
void *thread_increment_function() {
  for(int i=0; i < INT_MAX; i++){
    (*increment_test) = i;
    sleep(100);
  } 
}



int main(){

  pid_t* pid_serv2;
  printf("Hello, starting server 1");
  int cmp = 0;
  float tab[10];
  float mean_val = 0;
    // init de la file stockant les temperatures reçues
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

    while(1){
      // semaphore wait
      cmp = cmp%10;
      tab[cmp] = *sensor_data;
      mean_val = somme(tab)/10;
      cmp++;
      // semaphore 
      
    }
}
