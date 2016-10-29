#include "gestionErrores.h"
#include "lista.h"
#pragma once

// Función que devuelve el siguiente lexema del archivo, si es fin de archivo devuelve NULL
tipoelem * siguienteLexema();

// Función que inicializa el analizador léxico
void inicializarAnalizadorLexico();