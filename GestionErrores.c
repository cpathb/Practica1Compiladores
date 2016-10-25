#include <stdio.h>
#include <stdlib.h>
#include "gestionErrores.h"

void ImprimirError(int codigo, int linea){
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
        case 5:
            printf("Símbolo no esperado en la línea %d.\n",linea);
            break;
        case 6:
            printf("Se ha encontrado un caracter inesperado en el identificador.\n",linea);
            break;
        default:
            printf("Error no identificado.\n");
     }
}