#include "tabla_hash_encadenamiento.h"

/* TABLA HASH CON ENCADENAMIENTO */

void InicializarTablaHash (TablaHash t) {
   int i;
   for (i=0;i<Tam;i++)
      crea(&t[i]);
}

void DestruirTablaHash (TablaHash t){
   int i;
   for (i=0;i<Tam;i++)
      destruye(&t[i]);
}

//FUNCION HASH

int Hash (char *cad){
   int valor;
   unsigned char *c;

   for (c=cad,valor=0;*c;c++)
      valor+=(int)(*c);

   return(valor%Tam);
}


/* OTRA FUNCION HASH */
/*
 int Hash (char *cad){
     int i,suma=0;
     for (i=strlen(cad);i>=0;i--){
         suma=(suma*256+cad[i])%Tam;
     }
     return suma;
 }
*/

int Busqueda(TablaHash t, char *clave, tipoelem *e){
   posicion p;
   int enc;
   tipoelem ele;
   int pos=Hash(clave);
   p=primero(t[pos]);
   enc=0;
   while (p!=fin(t[pos]) && !enc) {
       recupera(t[pos],p,&ele);
      if (strcmp(ele.login,clave)==0){
         enc=1;
         *e=ele;
      }
      else
         p=siguiente(t[pos],p);
   }

   return enc;
}

int MiembroHash (TablaHash t, tipoelem e){
   posicion p;
   int enc;
   tipoelem elemclave;
   int pos=Hash(e.login);
   p=primero(t[pos]);
   enc=0;
   while (p!=fin(t[pos]) && !enc) {
      recupera(t[pos],p,&elemclave);
      if (strcmp(e.login,elemclave.login)==0)
         enc=1;
      else
         p=siguiente(t[pos],p);
   }

   return enc;
}

void InsertarHash (TablaHash *t, tipoelem elemento){
   int pos;

   if (MiembroHash(*t,elemento))
      return;

   pos=Hash(elemento.login);
   inserta(&(*t)[pos],primero((*t)[pos]),elemento);
}

void BorrarHash (TablaHash *t, char *cad){
   posicion p;
   tipoelem elemclave;
   int pos=Hash(cad);

   p=primero((*t)[pos]);
   recupera((*t)[pos],p,&elemclave);
   while (p!=fin((*t)[pos]) && strcmp(cad,elemclave.login))
   {
      p=siguiente((*t)[pos],p);
      recupera((*t)[pos],p,&elemclave);
   }
   if (p!=fin((*t)[pos]))
      suprime(&(*t)[pos],p);
}



