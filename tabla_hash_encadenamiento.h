/* 
 * File:   tablahash.h
 * Author: puri
 *
 * Created on 24 de noviembre de 2011, 12:47
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

#define TamHash 503 // Número de entradas de la tabla Hash, primo para cuando se recoloque no entre en un bucle infinito
#define primo 89 // Número primo empleado para la funcion hash

#pragma once

typedef lista TablaHash[TamHash];

void InicializarTablaHash (TablaHash t);
void DestruirTablaHash (TablaHash t);
int Hash (char *cad);
int Busqueda(TablaHash t, char *clave, tipoelem *e);
int MiembroHash (TablaHash t, tipoelem e);
int MiembroHashClave(TablaHash t, char * clave);
void InsertarHash (TablaHash *t, tipoelem elemento);
void BorrarHash (TablaHash *t, tipoelem elemento);
