#include <stdio.h>
#include <stdlib.h>
#include "gestionErrores.h"

void ImprimirError(int codigo){
     switch (codigo){
        case 1:
            printf("Error al abrir el fichero a compilar.\n");
            break;
        case 2:
            printf("No existe la estructura en la que estás buscando.\n");
            break;
        case 3:
            printf("El elemento no existe en la estructura.\n");
            break;
        case 4:
            printf("Error al abrir el fichero definiciones a introducir en la tabla de símbolos.\n");
            break;
        default:
            printf("Error no identificado.\n");
     }
}