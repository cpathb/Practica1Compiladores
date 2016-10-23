#include "lista.h"

void crea(lista *l){
    (*l) = (lista)malloc(sizeof(struct l));
    (*l)->inicio = (posicion)malloc(sizeof(struct celda));
    (*l)->fin = (*l)->inicio;
    ((*l)->fin)->sig = NULL;
    (*l)->longitud = 0;
}

void destruye(lista *l){
    (*l)->fin = (*l)->inicio;
    while ((*l)->fin != NULL){
		(*l)->fin = ((*l)->fin)->sig;
		free((*l)->inicio);
		(*l)->inicio = (*l)->fin;
    }
    free(*l);
    *l = NULL;
}

posicion primero(lista l){
    return (l->inicio);
}

posicion siguiente(lista l, posicion p){
    return (p->sig);
}

posicion fin(lista l){
    return (l->fin);
}

posicion anterior(lista l, posicion p){
    posicion q;

    q = l->inicio;

    while (q->sig != p){
		q = q->sig;
    }
    return q;
}

unsigned existe(lista l){
    if (l != NULL){
		return 1;
	}
    return 0;
}

unsigned esvacia(lista l){
    if (l->longitud == 0){
		return 1;
	}
    return 0;
}

void recupera(lista l, posicion p, tipoelem *e){
    *e = (p->sig)->elemento;
}

unsigned longitud(lista l){
    return (l->longitud);
}

void inserta(lista *l, posicion p, tipoelem e){
    posicion q;
    q = p->sig;

    p->sig = (posicion)malloc(sizeof(struct celda));
    (p->sig)->elemento = e;
    (p->sig)->sig = q;
    if (q == NULL){
		(*l)->fin = p->sig;
	}
    (*l)->longitud = (*l)->longitud + 1;
}

void suprime(lista *l, posicion p){
    posicion q;

    q = p->sig;
    p->sig = q->sig;
    if (p->sig == NULL){
		(*l)->fin = p;
	}
    free(q);
    (*l)->longitud = (*l)->longitud - 1;
}

void modifica(lista *l, posicion p, tipoelem e){
    (p->sig)->elemento = e;
}

/* //SOBRA¿?
//Función para obtener la posición de un elemento en la lista
posicion obtenerPosicion(lista *l, tipoelem e){
	posicion actual;
	posicion final;
	if (existe(*l)){ // Comprobamos que existe la lista en la que vamos a buscar
		actual=(*l)->inicio;
		final=(*l)->fin;
		while(actual!=NULL){ // Comprobamos hasta que no lleguemos al final de la lista
			if((actual->elemento.lexema)==e.lexema && (actual->elemento.compLex)==e.compLex){ // comprobamos si el elemento en la posicion actual es el que buscamos
				return actual;
			}
			if(actual!=final){ // Si no estamos al final de la lista vamos al elemento siguiente
				actual= actual->sig;
			}
			else{
				actual=NULL;
			}
		}
	}
	else{
		ImprimirError(2);
		actual=NULL;
	}
	return actual;
}

int existeElemento(lista *l, tipoelem e){
	posicion actual;
	posicion final;
	int encontrado=0;
	if (existe(*l)){ // Comprobamos que existe la lista en la que vamos a buscar
		actual=(*l)->inicio;
		final=(*l)->fin;
		while(actual!=NULL && encontrado==0){ // Comprobamos hasta que no lleguemos al final de la lista
			if((actual->elemento.lexema)==e.lexema && (actual->elemento.compLex)==e.compLex){ // comprobamos si el elemento en la posicion actual es el que buscamos
				encontrado=1;
			}
			if(actual!=final){ // Si no estamos al final de la lista vamos al elemento siguiente
				actual= actual->sig;
			}
			else{
				actual=NULL;
			}
		}
	}
	else{
		ImprimirError(2);
	}
	return encontrado;
}
*/