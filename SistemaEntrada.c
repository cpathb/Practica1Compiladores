//En este archivo.c se leerá del código fuente bloque a bloque y se introducira en el buffer con doble centinela
#include <stdio.h>
#include <stdlib.h>
#include "SistemaEntrada.h"
#include "GestionErrores.h"

#define tamBloque 8

/*
int main( int argc, const char* argv[] ) {

}
*/

char* rellenarCentinela(char* buffer);

void dameBloqueFichero(char* nombreFichero) {
    FILE * source = fopen(nombreFichero,"rb");
    char * buffer = (char *) malloc(sizeof(char)*(tamBloque*2));
    char * centinela = (char *) malloc(sizeof(char)*((tamBloque*2)+2));
    if(source == NULL){
        ImprimirError(1);
        exit(-1);
    }
    // fread(dónde, tamaño de unidad, cuanto tamaño, origen);
    int i=1;
    while(!feof(source)){
        fread(buffer, sizeof(char), (tamBloque*2), source);
        centinela = rellenarCentinela(buffer);
        printf("Lectura %d: \n",i);
        printf("%s \n",centinela);
        i++;
    }
    fclose(source);
}

char sig_caracter(){ // MODIFICAR
    return 'p';
}

char* rellenarCentinela(char* buffer){
    //Llenamos el buffer centinela con el contenido del buffer original, añadiendo los EOF en la mitad del centinela y al final (tambloque + 1) y (tambloque*2)+1)
    char * centinela = (char *) malloc(sizeof(char)*((tamBloque*2)+2));
    int i=0, j=0;
    while(i<(tamBloque*2) || j<((tamBloque*2)+2)){
        if(j==((tamBloque))){
            centinela[j]='$';
            j++;
        }
        if(j==((tamBloque*2)+1)){
            centinela[j]='$';
            j++;
        }
        if(i<(tamBloque*2)){
            if(buffer[i]!=EOF){
                centinela[j]=buffer[i];
            }
            else{ //llegamos al final del fichero
                centinela[j]='$';
            }
            i++;
        }
        j++;
    }

    return centinela;
}