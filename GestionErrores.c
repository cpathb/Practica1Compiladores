#include <stdio.h>
#include <stdlib.h>
#include "GestionErrores.h"

void ImprimirError(int codigo){
     switch (codigo){
         case 1:
             printf("Error al abrir el ichero a compilar.\n");
             break;
         default:
             printf("Error no identificado.\n");
     }
}