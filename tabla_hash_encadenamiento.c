#include "tabla_hash_encadenamiento.h"

// Función para inicializar la tabla Hash
void InicializarTablaHash (TablaHash t) {
    int i=0;
    while(i<TamHash){
        crea(&t[i]);
        i++;
    }
}

// Función para destruir la tabla hash
void DestruirTablaHash (TablaHash t){
    int i=0;
    while(i<TamHash){
        destruye(&t[i]);
        i++;
    }
}

/* Función Hash que utiliza un número primo para calcularse y asi disminuir el posibke número de colisiones */

 int Hash (char *cad){
    int i=strlen(cad),hash=0;
    while(i>=0){
        hash = ((primo * hash + cad[i]) % TamHash);
        i--;
    }

    return hash;
 }
 
//Busca un elemento en la tabla por la clave hash y mete el valor de ese elemento en el puntero e
int Busqueda(TablaHash t, char *clave, tipoelem *e){
    posicion p;
    int enc;
    tipoelem ele;
    int pos=Hash(clave);
    p=primero(t[pos]);
    enc=0;
    while (p!=fin(t[pos]) && !enc) {
        recupera(t[pos],p,&ele);
        if (strcmp(ele.lexema,clave)==0){
            enc=1;
            *e=ele;
        }
        else{
            p=siguiente(t[pos],p);
        }
    }
    return enc;
}

// Función para comprobar si la cadena ya está insertada en la tabla hash
int MiembroHashClave(TablaHash t, char * clave){
    posicion p;
    int enc;
    tipoelem elemclave;
    int pos=Hash(clave);
    p=primero(t[pos]);
    enc=0;

    while (p!=fin(t[pos]) && !enc){
        recupera(t[pos],p,&elemclave);
        if(strcmp(clave,elemclave.lexema)==0){
            enc=1;
         }
        else{
             p=siguiente(t[pos],p);
        }
    }
    return enc;
}

// Función para comprobar si el elemento es miembro de la tabla hash
int MiembroHash(TablaHash t, tipoelem e){
    posicion p;
    int enc;
    tipoelem elemclave;
    int pos=Hash(e.lexema);
    p=primero(t[pos]);
    enc=0;

    while (p!=fin(t[pos]) && !enc){
        recupera(t[pos],p,&elemclave);
        if(strcmp(e.lexema,elemclave.lexema)==0 && e.compLex==elemclave.compLex){
            enc=1;
         }
        else{
             p=siguiente(t[pos],p);
        }
    }
   return enc;
}

// Función para insertar en la tabla
void InsertarHash (TablaHash *t, tipoelem elemento){
    int pos;

    if (MiembroHash(*t,elemento)){
        return;
    }
    pos=Hash(elemento.lexema);
    inserta(&(*t)[pos],primero((*t)[pos]),elemento);
}

// Funcion para borrar en la tabla
void BorrarHash (TablaHash *t, tipoelem elemento){
    posicion p;
    tipoelem elemclave;
    int pos=Hash(elemento.lexema);

    p=primero((*t)[pos]);
    recupera((*t)[pos],p,&elemclave);
    while (p!=fin((*t)[pos]) && strcmp(elemento.lexema,elemclave.lexema) && (elemento.compLex==elemclave.compLex)){
        p=siguiente((*t)[pos],p);
        recupera((*t)[pos],p,&elemclave);
    }
    if (p!=fin((*t)[pos])){
        suprime(&(*t)[pos],p);
    }
}