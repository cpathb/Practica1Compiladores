#include <stdio.h>
#include <stdlib.h>
#include "GestionErrores.h"

// Función para leer bloque a bloque del fichero y guardarlo en el buffer centinela
struct centinela{
    char * cadena;
    int inicio;
    int fin;
};

void inicializarVariablesSistemaEntrada();
char * siguienteLexema();


