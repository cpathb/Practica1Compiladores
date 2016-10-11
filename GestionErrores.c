#include <stdio.h>
#include <stdlib.h>
#include "GestionErrores.h"

void ImprimirError(int codigo){
     switch (codigo){
        case 1:
            printf("Error al abrir el fichero a compilar.\n");
            break;
        case 2:
            printf("No existe la lista en la que estás buscando.\n");
            break;
        case 3:
            printf("El elemento no existe en la lista.\n");
            break;
        default:
            printf("Error no identificado.\n");
     }
}