#include <stdlib.h>
#include "GestionErrores.h"

typedef struct{
    char * lexema;
    int compLex;
}tipo;

typedef tipo tipoelem;

struct celda{
    tipoelem elemento;
    struct celda *sig;
};

typedef struct celda * posicion;

struct l{
    posicion inicio;
    unsigned longitud;
    posicion fin;
};

typedef struct l * lista;

void crea(lista *l);
void destruye(lista *l);
posicion primero(lista l);
posicion siguiente(lista l, posicion p);
posicion fin(lista l);
posicion anterior(lista l, posicion p);
unsigned existe(lista l);
unsigned esvacia(lista l);
void recupera(lista l, posicion p, tipoelem *e);
unsigned longitud(lista l);
void inserta (lista *l, posicion p, tipoelem e);
void suprime (lista *l, posicion p);
void modifica (lista *l, posicion p, tipoelem e);
posicion obtenerPosicion(lista *l, tipoelem e);
int existeElemento(lista *l, tipoelem e);
