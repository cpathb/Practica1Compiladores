#include <stdio.h>
#include <stdlib.h>
#include "gestionErrores.h"

void ImprimirError(int codigo, int linea){
     switch (codigo){
        case 1:
            printf("Error al abrir el fichero a compilar en el sistema de entrada.\n");
            break;

        case 2:
            printf("No existe la estructura en la que estás buscando.\n");
            break;

        case 3:
            printf("El elemento no existe en la estructura.\n");
            break;

        case 4:
            printf("Error al abrir el fichero de palabras reservadas a introducir en la tabla de símbolos.\n");
            break;

        case 5:
            printf("Símbolo no esperado en la línea %d.\n",linea);
            break;

        case 6:
            printf("Se ha encontrado un caracter inesperado en un identificador en la línea %d.\n",linea);
            break;

        case 7:
            printf("No se ha encontrado el final de ' en la línea %d.\n",linea);
            break;

        case 8:
            printf("No se ha encontrado el final de \" en la línea %d.\n",linea);
            break;

         case 9:
             printf("No se ha encontrado el final del comentario, línea %d.\n",linea);
             break;

         case 10:
             printf("Se ha excedido el tamaño máximo de lexema, línea %d.\n",linea);
             break;

         case 11:
             printf("Se ha sobreescrito el contenido del buffer centinela.\n");
             break;

         default:
            printf("Error no identificado.\n");
     }
}
