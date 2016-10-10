#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

#define Tam ****** /*DEFINIR NUMERO de entradas en la tabla.*/

typedef lista TablaHash[Tam];

void InicializarTablaHash (TablaHash t);
void DestruirTablaHash (TablaHash t);
int Hash (char *cad);
int Busqueda(TablaHash t, char *clave, tipoelem *e);
int MiembroHash (TablaHash t, tipoelem e);
void InsertarHash (TablaHash *t, tipoelem elemento);
void BorrarHash (TablaHash *t, char *cad);
