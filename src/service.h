//#include <string.h>
#include <stdlib.h>
#include "defines.h"

//fonction calcul de moyenne du tableau
double mean (double* temperature, int size){
    double somme = 0;
    for(int i=0; i<size; i++){
      somme += temperature[i];
    }
    return(somme/size);
}

//fonction reprise a partir de memoire stable
void read_from_stable_memory(double* tableau, int* i_fenetre,FILE* memoire_stable)
{
  char chaine[100];
  for(int i = 0;i<TAILLE_FENETRE;i++)
  {
    fscanf(memoire_stable,"%s",chaine);
    tableau[i] = strtod(chaine,NULL);
  }
  fscanf(memoire_stable,"%s",chaine);
  fscanf(memoire_stable,"%s",chaine);
  *i_fenetre = atoi(chaine)+1;
}